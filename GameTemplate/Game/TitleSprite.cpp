#include "stdafx.h"
#include "TitleSprite.h"

namespace
{
	const float TEXT_SPRITE_W = 1920.0f;							// スプライトの横幅
	const float TEXT_SPRITE_H = 1080.0f;							// スプライトの縦幅
	const Vector3 TEXT_SPRITE_POSITION = { 7.0f, -20.8f, 0.0f };	// スプライトを表示する座標	
	const Vector3 TEXT_SPRITE_SCALE = { 0.9f, 0.9f, 0.9f };			// スプライトの拡大率
	const float ALPHA_VALUE_TO_STOP_DRAWING = 0.0f;					// 描画をやめるα値
	const float FADE_OUT_ALPHA_MULTIPLIER = 0.3f;					// フェードアウト時にα値に乗算する値
}

bool TitleSprite::Start()
{
	// スプライトを初期化する。
	m_pressBToStartSprite.Init(
		"Assets/sprite/PRESS_B_TO_START.DDS",
		TEXT_SPRITE_W,
		TEXT_SPRITE_H
	);

	// スプライトの座標を設定する。
	m_pressBToStartSprite.SetPosition(TEXT_SPRITE_POSITION);

	// スプライトの拡大率を設定する。
	m_pressBToStartSprite.SetScale(TEXT_SPRITE_SCALE);

	return true;
}

void TitleSprite::Update()
{
	// スプライトを更新する。
	m_pressBToStartSprite.Update();
}

void TitleSprite::Render(RenderContext& rc)
{
	// スプライトを描画する。
	m_pressBToStartSprite.Draw(rc);
}
