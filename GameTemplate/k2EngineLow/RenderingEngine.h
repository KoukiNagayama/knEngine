#pragma once
#include "Bloom.h"
#include "EdgeControl.h"
namespace nsK2EngineLow {

	const int MAX_TEXT_SIZE = 256;

	class RenderingEngine
	{
	public:
		/// <summary>
		/// レンダリングパイプラインを初期化
		/// </summary>
		void Init();
		/// <summary>
		/// レンダリングパイプラインを実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Execute(RenderContext& rc);
		/// <summary>
		/// 準備用の描画パスにモデルを追加
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToRenderReservePass(Model& model)
		{
			m_renderToReservePassModels.push_back(&model);
		}
		/// <summary>
		/// フォワードレンダリングの描画パスにモデルを追加
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
		/// <summary>
		/// スプライト描画パスにスプライトを追加
		/// </summary>
		/// <param name="sprite"></param>
		void AddSpriteToSpriteRenderPass(Sprite& sprite)
		{
			m_sprites.push_back(&sprite);
		}
		/// <summary>
		/// 深度値記録用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>深度値記録用レンダリングターゲット</returns>
		RenderTarget& GetDepthValue()
		{
			return m_reserveRenderTarget[enReserveDepth];
		}
		/// <summary>
		/// ワールド座標記録用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>ワールド座標記録用のレンダリングターゲット</returns>
		RenderTarget& GetWorldPos()
		{
			return m_reserveRenderTarget[enReserveWorldPos];
		}
		/// <summary>
		/// 法線記録用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>法線記録用のレンダリングターゲット</returns>
		RenderTarget& GetNormal()
		{
			return m_reserveRenderTarget[enReserveNormal];
		}
	private:
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容をフレームバッファにコピーするためのスプライトを初期化
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// 準備用レンダリングターゲットを初期化
		/// </summary>
		void InitReserveRenderTarget();
		/// <summary>
		/// 2D描画用レンダリングターゲットを初期化
		/// </summary>
		void Init2DRenderTarget();
		/// <summary>
		/// 準備用パスへの描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderToReservePass(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリング
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// スプライト描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderSprite(RenderContext& rc);
		/// <summary>
		/// フォント描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderFont(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピー
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
	private:
		// Reserveパスのレンダリングターゲットの種類
		enum EnReserve
		{
			enReserveDepth,				// 深度
			enReserveNormal,			// 法線
			enReserveWorldPos,			// ワールド座標
			enReserveNum,				// G-Bufferの数
		};

		std::vector<Model*>		m_renderToReservePassModels;			// Reserveの描画パスで描画されるモデル
		std::vector<Model*>		m_forwardRenderModels;					// フォワードレンダリングの描画パスで描画されるモデル
		std::vector<Sprite*>	m_sprites;								// スプライトの描画パスで描画するスプライト
		RenderTarget			m_mainRenderTarget;						// メインレンダリングターゲット
		RenderTarget			m_reserveRenderTarget[enReserveNum];	// G-Buffer用のレンダリングターゲット
		RenderTarget			m_2DRenderTarget;						// 2D描画用のレンダリングターゲット
		Sprite					m_copyMainRtToFrameBufferSprite;		// メインレンダリングターゲットの内容をフレームバッファにコピーする用のスプライト
		Sprite					m_2DSprite;								// 2D合成用スプライト
		Sprite					m_mainSprite;
		Bloom					m_bloom;
		EdgeControl				m_edgeControl;
	};
	extern RenderingEngine g_renderingEngine;
}
 

