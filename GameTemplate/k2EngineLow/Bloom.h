#pragma once
namespace nsK2EngineLow {
	class Bloom
	{
	public:
		Bloom() {};
		~Bloom() {};

		/// <summary>
		/// ブルーム実行に必要な全ての要素の初期化
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		/// <summary>
		/// 輝度抽出用のレンダリングターゲットを初期化
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void InitLuminanceRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 輝度抽出用のスプライトを初期化
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void InitSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// ガウシアンブラーを初期化
		/// </summary>
		void InitBlur();
		/// <summary>
		/// ボケ画像を加算合成するスプライトを初期化
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void InitFinalSprite(RenderTarget& mainRenderTarget);
	private:
		RenderTarget		m_luminanceRenderTarget;			// 輝度抽出用のレンダリングターゲット
		GaussianBlur		m_gaussianBlur[4];					// ガウシアンブラー
		Sprite				m_luminanceSprite;					// 輝度抽出用のスプライト
		Sprite				m_finalSprite;						// ボケ画像を加算合成するスプライト
	};
}


