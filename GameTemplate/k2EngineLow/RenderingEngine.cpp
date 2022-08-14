#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		InitReserveRenderTarget();
		Init2DRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
		m_bloom.Init(m_mainRenderTarget);
	}
	
	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitReserveRenderTarget()
	{
		// フレームバッファの縦幅
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();
		// フレームバッファの横幅
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();

		// 深度値出力用のレンダリングターゲットを初期化する
		m_reserveRenderTarget[enReserveDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		// 法線出力用のレンダリングターゲットを初期化する
		m_reserveRenderTarget[enReserveNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		// レンダリングターゲットのテクスチャを埋める値 
		float clearColor[4] = { 10000.0f, 10000.0f, 10000.0f, 1.0f };
		// ワールド座標出力用のレンダリングターゲットを初期化する
		m_reserveRenderTarget[enReserveWorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		
		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		// 最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダリングターゲット。
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// 解像度はmainRenderTargetの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		// 上書き。
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		// レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		// テクスチャはメインレンダリングターゲット。
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// 解像度は2D用レンダリングターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		// レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		// スプライトの初期化オブジェクト
		SpriteInitData spriteInitData;
		// テクスチャ
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		// 解像度
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// シェーダーのファイルパスを指定
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		// 初期化オブジェクトをもとにスプライトを初期化
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{

		// 準備用パスへのレンダリング
		RenderToReservePass(rc);

		// 輪郭線の制御
		m_edgeControl.Update();

		// フォワードレンダリング
		ForwardRendering(rc);

		// ブルーム
		m_bloom.Render(rc, m_mainRenderTarget);

		// スプライト描画
		RenderSprite(rc);

		// メインレンダリングターゲットの内容をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);


		// 登録されているオブジェクトをクリア
		m_renderToReservePassModels.clear();
		m_forwardRenderModels.clear();
		m_sprites.clear();
	}

	void RenderingEngine::RenderToReservePass(RenderContext& rc)
	{
		RenderTarget* rts[enReserveNum] = {
			&m_reserveRenderTarget[enReserveDepth],			// 0番目のレンダリングターゲット
			& m_reserveRenderTarget[enReserveNormal],		// 1番目のレンダリングターゲット
			& m_reserveRenderTarget[enReserveWorldPos]		// 2番目のレンダリングターゲット
		};

		// レンダリングターゲットとして利用可能になるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		// レンダリングターゲットを設定する
		rc.SetRenderTargetsAndViewport(ARRAYSIZE(rts), rts);
		// レンダリングターゲットをクリアする
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& model : m_renderToReservePassModels) {
			// モデルを描画
			model->Draw(rc);
		}

		// 書き込み完了待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		// レンダリングターゲットとして利用可能になるまで待つ
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		// レンダリングターゲットを設定する
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// レンダリングターゲットをクリアする
		rc.ClearRenderTargetView(m_mainRenderTarget);

		for (auto& model : m_forwardRenderModels)
		{
			// モデルを描画
			model->Draw(rc);
		}
		// 書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::RenderSprite(RenderContext& rc)
	{
		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		for (auto& sprite : m_sprites) {
			// スプライトを描画
			sprite->Draw(rc);
		}

		// 2D描画用レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_2DSprite.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// ビューポートを指定する
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}

	RenderingEngine g_renderingEngine;
}
