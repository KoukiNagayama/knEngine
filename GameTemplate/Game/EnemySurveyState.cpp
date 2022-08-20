#include "stdafx.h"
#include "EnemySurveyState.h"
#include "Enemy.h"

namespace
{
	const float MINIMUM_CHASE_TIME = 20.0f;					// 最低限追跡する時間
	const float TIME_TO_LOSE_SIGHT = 5.0f;					// プレイヤーを見失った時間
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;		// 強制的にステートを遷移させるまでの時間
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// アニメーション補間時間
}

void EnemySurveyState::ManageState()
{
	// 一定時間以内にプレイヤーを発見
	if (m_enemy->m_isFound == true) {
		// 最低限追跡する時間を指定
		m_enemy->m_chaseTime = MINIMUM_CHASE_TIME;
		// 追跡を開始
		m_enemy->ChangeState(m_enemy->enEnemyState_Chase);
		// 追跡する。
		m_enemy->m_isChase = true;

		m_enemy->m_isMove = true;
	}
	// 一定時間プレイヤーを見失っていたら
	if (m_enemy->m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// 帰還状態の時間を指定
		m_enemy->m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// ステートをパスへの帰還状態にする
		m_enemy->ChangeState(m_enemy->enEnemyState_ReturnToPath);

		m_enemy->m_isMove = true;
	}
}

void EnemySurveyState::ProcessByState()
{
	// 見回し
	Survey();
	// プレイヤーの音を捜索
	m_enemy->SearchSoundOfPlayer();
}

void EnemySurveyState::Survey()
{
	// 見回した時間を加算
	m_enemy->m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void EnemySurveyState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Survey, 
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}
