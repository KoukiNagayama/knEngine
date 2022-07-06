#include "stdafx.h"
#include "GameOver.h"

namespace
{
	const Vector3 SPRITE_POSITION = Vector3::Zero;			// スプライトを表示する座標。
	const Vector3 SPRITE_SCALE = Vector3::One;				// スプライトの拡大率。
	const float	  GAME_OVER_SPRITE_W = 1920.0f;				// スプライトの横幅。
	const float   GAME_OVER_SPRITE_H = 1080.0f;				// スプライトの縦幅。
	const float	  FADE_IN_ALPHA_MULTIPLIER = 0.5f;			// フェードインのαの倍率。
	const float   FADE_OUT_ALPHA_MULTIPLIER = 0.4f;			// フェードアウトのαの倍率。
	const float   FADE_OUT_TIMER_MAX_VALUE_PER_SEC = 2.2f;	// フェードアウトに移行するタイマーの初期化用最大時間
	const float	  MAX_ALPHA_VALUE = 1.0f;
	const float   MIN_ALPHA_VALUE = 0.0f;

	const float	  GAME_OVER_SPRITE_TEXT_W = 1600.0f;		// スプライトの横幅。
	const float   GAME_OVER_SPRITE_TEXT_H = 600.0f;			// スプライトの縦幅。
	const Vector3 SPRITE_SHAKE_POSITION[14] = {				// ゲームオーバー時のシェイク量（ランダムにしたくないので固定）
		{300.0f,400.0f,0.0f},
		{260.0f,380.0f,0.0f},
		{180.0f,340.0f,0.0f},
		{90.0f,110.0f,0.0f},
		{40.0f,20.0f,0.0f},
		{-40.0f,-20.0f,0.0f},
		{-60.0f,-40.0f,0.0f},
		{-45.0f,-25.0f,0.0f},
		{-30.0f,-15.0f,0.0f},
		{10.0f,10.0f,0.0f},
		{8.0f,8.0f,0.0f},
		{5.0f,5.0f,0.0f},
		{-2.0f,-2.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	const float   BLOOD_START_TIME = 1.2f;	// 血飛沫が出始める時間
	const float   BLOOD_WAIT_TIME = 0.06f;	// 血飛沫の更新時間

}

bool GameOver::Start()
{
	// スプライトの初期化
	m_gameOverSprite_BG.Init(
		"Assets/sprite/GameOver/GameOver_BG.DDS", 
		GAME_OVER_SPRITE_W, 
		GAME_OVER_SPRITE_H
	);

	// スプライトの初期化
	m_gameOverSprite_Text.Init(
		"Assets/sprite/GameOver/GameOver_Text.DDS",
		GAME_OVER_SPRITE_TEXT_W,
		GAME_OVER_SPRITE_TEXT_H
	);

	// スプライトの初期化
	m_gameOverSprite_Blood.Init(
		"Assets/sprite/GameOver/GameOver_Blood1.DDS",
		GAME_OVER_SPRITE_W,
		GAME_OVER_SPRITE_H
	);

	// スプライトの座標を設定。
	m_gameOverSprite_BG.SetPosition(SPRITE_POSITION);
	// スプライトの拡大率を設定。
	m_gameOverSprite_BG.SetScale(SPRITE_SCALE);
	// タイマーを初期化。
	m_timerToFadeOut = FADE_OUT_TIMER_MAX_VALUE_PER_SEC;
	m_bloodTimer_Start = BLOOD_START_TIME;

	// シェイク番号を念のため初期化
	m_spriteShakeNo = 0;

	// 要素数を計算
	m_shakeNum = sizeof(SPRITE_SHAKE_POSITION) / sizeof(Vector3);

	return true;
}

void GameOver::Update()
{
	// シェイク
	if (m_spriteShakeNo < m_shakeNum) {

		m_gameOverSprite_Text.SetPosition(SPRITE_SHAKE_POSITION[m_spriteShakeNo]);
		m_gameOverSprite_Text.Update();

		m_spriteShakeNo++;
	}

	// 血飛沫画像
	if (m_bloodTimer_Start > 0.0f) {
		m_bloodTimer_Start -= g_gameTime->GetFrameDeltaTime();
	}
	else {
		if (m_isBloodPlaying == false) {
			// 血飛沫開始
			m_isBloodPlaying = true;
			m_bloodTimer_Wait = BLOOD_WAIT_TIME;
		}
		else {
			m_bloodTimer_Wait -= g_gameTime->GetFrameDeltaTime();
			if (m_bloodTimer_Wait <= 0.0f) {
				// 血飛沫画像を更新
				BloodInit();
				m_bloodTimer_Wait = BLOOD_WAIT_TIME;
				m_bloodNo++;
			}
		}
	}

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
			m_isDrawText = false;		// ゲームオーバーテキストの描画を停止
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

void GameOver::BloodInit() {

	switch (m_bloodNo)
	{
	case 0:
		m_gameOverSprite_Blood.Init(
			"Assets/sprite/GameOver/GameOver_Blood2.DDS",
			GAME_OVER_SPRITE_W,
			GAME_OVER_SPRITE_H
		);
		break;
	case 1:
		m_gameOverSprite_Blood.Init(
			"Assets/sprite/GameOver/GameOver_Blood3.DDS",
			GAME_OVER_SPRITE_W,
			GAME_OVER_SPRITE_H
		);
		break;
	case 2:
		m_gameOverSprite_Blood.Init(
			"Assets/sprite/GameOver/GameOver_Blood4.DDS",
			GAME_OVER_SPRITE_W,
			GAME_OVER_SPRITE_H
		);
		break;
	case 3:
		m_gameOverSprite_Blood.Init(
			"Assets/sprite/GameOver/GameOver_Blood5.DDS",
			GAME_OVER_SPRITE_W,
			GAME_OVER_SPRITE_H
		);
		break;
	case 4:
		m_gameOverSprite_Blood.Init(
			"Assets/sprite/GameOver/GameOver_Blood6.DDS",
			GAME_OVER_SPRITE_W,
			GAME_OVER_SPRITE_H
		);
		break;
	}

}

void GameOver::Render(RenderContext& rc)
{
	if (m_currentAlpha >= MIN_ALPHA_VALUE) {
		m_gameOverSprite_BG.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_gameOverSprite_BG.Draw(rc);
	}

	if (m_isDrawText) {
		m_gameOverSprite_Text.Draw(rc);
	}

	if (m_isBloodPlaying) {
		m_gameOverSprite_Blood.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_gameOverSprite_Blood.Draw(rc);
	}

}

