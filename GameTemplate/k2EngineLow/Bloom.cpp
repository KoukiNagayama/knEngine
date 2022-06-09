#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		InitLuminanceRenderTarget(mainRenderTarget);

		InitSprite(mainRenderTarget);

		InitBlur();

		InitFinalSprite(mainRenderTarget);
	}

	void Bloom::InitLuminanceRenderTarget(RenderTarget& mainRenderTarget)
	{
		m_luminanceRenderTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			mainRenderTarget.GetColorBufferFormat(),
			DXGI_FORMAT_UNKNOWN
		);
	}

	void Bloom::InitSprite(RenderTarget& mainRenderTarget)
	{
		// 輝度抽出用のスプライトの初期化データ
		SpriteInitData luminanceSpriteInitData;
		// 輝度抽出用のシェーダーのファイルパスを指定する。
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/bloom.fx";
		// 頂点シェーダーのエントリーポイントを指定する。
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		// ピクセルシェーダーのエントリーポイントを指定する。
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		// スプライトの幅と高さはluminnceRenderTargetと同じ。
		luminanceSpriteInitData.m_width = mainRenderTarget.GetWidth();
		luminanceSpriteInitData.m_height = mainRenderTarget.GetHeight();
		// テクスチャはメインレンダリングターゲットのカラーバッファ。
		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		// 描き込むレンダリングターゲットのフォーマットを指定する。
		luminanceSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
		// 作成した初期化データをもとにスプライトを初期化
		m_luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitBlur()
	{
		// gaussianBlur[0]は輝度テクスチャにガウシアンブラーをかける
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		// gaussianBlur[1]はgaussianBlur[0]のテクスチャにガウシアンブラーをかける
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		// gaussianBlur[2]はgaussianBlur[1]のテクスチャにガウシアンブラーをかける
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		// gaussianBlur[3]はgaussianBlur[2]のテクスチャにガウシアンブラーをかける
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}

	void Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
	{
		SpriteInitData finalspriteInitData;
		// ボケテクスチャを4枚指定する
		finalspriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalspriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalspriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalspriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
		// 解像度はmainRenderTargetの幅と高さ
		finalspriteInitData.m_width = mainRenderTarget.GetWidth();
		finalspriteInitData.m_height = mainRenderTarget.GetHeight();
		// ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
		// 2D用のシェーダーを使用する
		finalspriteInitData.m_fxFilePath = "Assets/shader/bloom.fx";
		finalspriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		// ただし、加算合成で描画するので、アルファブレンディングモードを加算にする
		finalspriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		finalspriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

		m_finalSprite.Init(finalspriteInitData);
	}

	void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//レンダリングターゲットへの書き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
		// 輝度抽出
		// 輝度抽出用のレンダリングターゲットに変更
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_luminanceRenderTarget);
		// 輝度抽出を行う
		m_luminanceSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

		// ガウシアンブラーを4回実行する
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);

		// 4枚のボケ画像を合成してメインレンダリングターゲットに加算合成
		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// 最終合成
		m_finalSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
}