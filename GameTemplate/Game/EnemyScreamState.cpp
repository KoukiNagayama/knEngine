#include "stdafx.h"
#include "EnemyScreamState.h"
#include "Enemy.h"

namespace
{
	const float MINIMUM_CHASE_TIME = 20.0f;						// 最低限追跡する時間
	const int	ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER = 4;		// エネミーの咆哮の音源の登録番号
	const float SCREAM_VOLUME = 1.5f;							// 咆哮の音量
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;		// アニメーション補間時間
}

void EnemyScreamState::ManageState()
{
	// 咆哮が終了していないならば
	if (m_enemy->m_isScream == true) {
		return;
	}
	m_enemy->m_isMove = true;
	// 最低限追跡する時間を指定
	m_enemy->m_chaseTime = MINIMUM_CHASE_TIME;
	// ステートを追跡状態にする
	m_enemy->ChangeState(m_enemy->enEnemyState_Chase);
	// 追跡しているように記録。
	m_enemy->m_isChase = true;
}

void EnemyScreamState::ProcessByState()
{
	Scream();
}

void EnemyScreamState::Scream()
{
	// 咆哮の音源を鳴らす
	if (m_enemy->m_screamSound == nullptr) {
		m_enemy->m_screamSound = NewGO<SoundSource>(0);
		m_enemy->m_screamSound->Init(ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER);
		m_enemy->m_screamSound->SetVolume(SCREAM_VOLUME);
		m_enemy->m_screamSound->Play(false);
		m_enemy->m_isScream = true;
	}

	// タイマー。
	m_enemy->m_screamEndTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_enemy->m_screamEndTimer <= 0.0f) {
		m_enemy->m_isScream = false;
		m_enemy->m_screamSound = nullptr;
	}
}

void EnemyScreamState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Scream,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}
