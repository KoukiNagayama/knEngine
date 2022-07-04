#include "stdafx.h"
#include "GameOver.h"

namespace
{
	const Vector3 SPRITE_POSITION = Vector3::Zero;			// スプライトを表示する座標。
	const Vector3 SPRITE_SCALE = Vector3::One;				// スプライトの拡大率。
	const float	  GAME_OVER_SPRITE_W = 1920.0f;				// スプライトの横幅。
	const float   GAME_OVER_SPRITE_H = 1080.0f;				// スプライトの縦幅。
	const float	  FADE_IN_ALPHA_MULTIPLIER = 0.5f;			// フェードインのαの倍率。
	const float   FADE_OUT_ALPHA_MULTIPLIER = 0.2f;			// フェードアウトのαの倍率。
	const float   FADE_OUT_TIMER_MAX_VALUE_PER_SEC = 1.5f;	// フェードアウトに移行するタイマーの初期化用最大時間
	const float	  MAX_ALPHA_VALUE = 1.0f;
	const float   MIN_ALPHA_VALUE = 0.0f;
}

bool GameOver::Start()
{
	// スプライトの初期化
	m_gameOverSprite.Init(
		"Assets/sprite/Game_Over.DDS", 
		GAME_OVER_SPRITE_W, 
		GAME_OVER_SPRITE_H
	);
	// スプライトの座標を設定。
	m_gameOverSprite.SetPosition(SPRITE_POSITION);
	// スプライトの拡大率を設定。
	m_gameOverSprite.SetScale(SPRITE_SCALE);
	// タイマーを初期化。
	m_timerToFadeOut = FADE_OUT_TIMER_MAX_VALUE_PER_SEC;

	return true;
}

void GameOver::Update()
{
	// α値が1.0より低いならば
	if (m_isFadeOut == false) {
		FadeIn();
	}
	else {
		// フェードアウトするまでの時間を計算
		m_timerToFadeOut -=  g_gameTime->GetFrameDeltaTime();
		if (m_timerToFadeOut <= MIN_ALPHA_VALUE) {
			m_timerToFadeOut = MIN_ALPHA_VALUE;
			FadeOut();
		}
	}
}

void GameOver::FadeIn()
{
	// α値を増やす
	m_currentAlpha += FADE_IN_ALPHA_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
	// α値が直前の式で1.0より高くなったならば
	if (m_currentAlpha >= MAX_ALPHA_VALUE) {
		m_isFadeOut = true;
	}
}

void GameOver::FadeOut()
{
	// α値を減らす
	m_currentAlpha -= FADE_OUT_ALPHA_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
	// α値が直前の式で0.0より低くなったならば
	if (m_currentAlpha <= MIN_ALPHA_VALUE) {
		m_isEndGameOverProcess = true;
	}
}

void GameOver::Render(RenderContext& rc)
{
	if (m_currentAlpha >= MIN_ALPHA_VALUE) {
		m_gameOverSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_gameOverSprite.Draw(rc);
	}
}

