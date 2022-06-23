#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

namespace nsK2EngineLow {
	void SpriteRender::Init(const char* filePath,
		const float w,
		const float h,
		AlphaBlendMode alphaBlendMode
	)
	{
		SpriteInitData initData;
		// DDSファイル(画像データ)のファイルパスを指定する。
		initData.m_ddsFilePath[0] = filePath;
		// Sprite表示用のシェーダーのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		// スプライトの幅と高さを指定する。
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		// 使用するアルファブレンディングモードを指定する。
		initData.m_alphaBlendMode = alphaBlendMode;

		m_sprite.Init(initData);
	}

	void SpriteRender::Update()
	{
		m_sprite.Update(
			m_position,
			m_rotation,
			m_scale,
			m_pivot
		);
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		g_renderingEngine.AddSpriteToSpriteRenderPass(m_sprite);
	}
}
