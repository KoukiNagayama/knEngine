#include "stdafx.h"
#include "ScreenEffect.h"
#include "Enemy.h"
#include "Player.h"

namespace
{
	const float SPRITE_W = 1920.0f;					// スプライトの横幅
	const float SPRITE_H = 1080.0f;					// スプライトの縦幅
	const float INIT_MIN_DIST_TO_ENEMY = 10000.0f;	// エネミーとの最短距離の初期化値
	const float MAX_ALPHA_VALUE = 1.0f;				// α値の最大値
	const float MIN_ALPHA_VALUE = 0.0f;				// α値の最小値
	const float MAX_RANGE_TO_ENEMY = 1500.0f;		// エネミーとの最大距離
	const int IDENTIFICATION_OF_ENEMY1 = 0;			// エネミー1の識別する値
	const int IDENTIFICATION_OF_ENEMY2 = 1;			// エネミー2の識別する値
	const float MULTIPLY_FADE_OUT = 0.3f;			// フェードアウトに乗算する値
	const float RGB_VALUE = 1.0f;					// RGBの値

}

bool ScreenEffect::Start()
{
	// スクリーンエフェクトの初期化。
	m_screenEffect.Init("Assets/sprite/screenEffect_test.DDS", SPRITE_W, SPRITE_H);
	m_screenEffect.SetPosition(Vector3::Zero);

	// エネミーを検索
	const auto& enemys = FindGOs<Enemy>("enemy");
	// エネミーの数
	const int size = enemys.size();
	
	// エネミーのオブジェクトのアドレスを振り分ける。
	for (int i = 0; i < size; i++) {
		switch (i) {
		case IDENTIFICATION_OF_ENEMY1:
			m_enemy1 = enemys[i];
			break;
		case IDENTIFICATION_OF_ENEMY2:
			m_enemy2 = enemys[i];
			break;
		}
	}

	// プレイヤーを検索
	m_player = FindGO<Player>("player");


	return true;
}

void ScreenEffect::Update()
{

	// ステート管理。
	ManageState();

	if (m_enScreenEffectState == enState_Idle) {
		return;
	}

	// α値を変更する。
	ChangeAlphaValue();
}

void ScreenEffect::ChangeAlphaValue()
{
	// プレイヤーの座標を取得
	m_playerPos = m_player->GetPosition();

	// エネミー1
	if (m_enemy1->IsChase()) {
		// エネミーの座標
		Vector3 enemyPos = m_enemy1->GetPosition();
		// プレイヤーとの距離
		Vector3 distToPlayer = m_playerPos - enemyPos;

		if (distToPlayer.Length() <= m_shortestDistanceToEnemy) {
			// 最短距離を更新。
			m_shortestDistanceToEnemy = distToPlayer.Length();
		}
	}

	// エネミー2
	if (m_enemy2->IsChase()) {
		// エネミーの座標
		Vector3 enemyPos = m_enemy2->GetPosition();
		// プレイヤーとの距離
		Vector3 distToPlayer = m_playerPos - enemyPos;

		if (distToPlayer.Length() <= m_shortestDistanceToEnemy) {
			// 最短距離を更新。
			m_shortestDistanceToEnemy = distToPlayer.Length();
		}
	}

	// 最短距離が更新されているならば。
	if (m_shortestDistanceToEnemy != INIT_MIN_DIST_TO_ENEMY) {
		// 現在のα値を更新。
		m_currentAlpha = MAX_ALPHA_VALUE - (m_shortestDistanceToEnemy / MAX_RANGE_TO_ENEMY * MAX_ALPHA_VALUE);
	}

	// α値に乗算してフェードを表現する。
	m_currentAlpha *= m_alphaValueMultiplier;

	// 最短距離を初期化。
	m_shortestDistanceToEnemy = INIT_MIN_DIST_TO_ENEMY;

}

void ScreenEffect::ManageState()
{
	switch (m_enScreenEffectState) {
	case enState_FadeIn:
		// フェードイン。
		FadeIn();
		// α値に乗算する値が1.0を超えたならば。
		if (m_alphaValueMultiplier >= MAX_ALPHA_VALUE) {
			// 値を固定する。
			m_enScreenEffectState = enState_Fix;
		}
		break;
	case enState_Fix:
		// 追跡状態が終わったならば。
		if (m_enemy1->IsChase() == false && m_enemy2->IsChase() == false) {
			// フェードアウトさせる。
			m_enScreenEffectState = enState_FadeOut;
		}
		break;
	case enState_FadeOut:
		// フェードアウト。
		FadeOut();
		// α値に乗算する値が0.0以下になったならば。
		if (m_alphaValueMultiplier <= MIN_ALPHA_VALUE) {
			m_enScreenEffectState = enState_Idle;
		}
		break;
	case enState_Idle:
		// エネミーが追跡状態に移行したならば。
		if (m_enemy1->IsChase() || m_enemy2->IsChase()) {
			m_enScreenEffectState = enState_FadeIn;
		}
		break;
	}
}

void ScreenEffect::FadeIn()
{
	m_alphaValueMultiplier += g_gameTime->GetFrameDeltaTime();
}

void ScreenEffect::FadeOut()
{
	m_alphaValueMultiplier -= MULTIPLY_FADE_OUT * g_gameTime->GetFrameDeltaTime();
}

void ScreenEffect::Render(RenderContext& rc)
{
	// α値を乗算。
	m_screenEffect.SetMulColor({ RGB_VALUE,RGB_VALUE,RGB_VALUE,m_currentAlpha });
	// 描画。
	m_screenEffect.Draw(rc);
}