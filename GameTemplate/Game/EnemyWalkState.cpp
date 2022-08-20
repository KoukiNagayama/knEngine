#include "stdafx.h"
#include "Enemy.h"
#include "EnemyWalkState.h"

namespace
{
	const float TIME_TO_END_SCREAM_PER_SEC = 3.0f;			// 咆哮が終了するまでの時間
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;	// 歩き中の目標座標への到達判定をとる最大距離
	const float WALK_SPEED = 6.5f * 0.7f;					// 歩く速さ
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// アニメーション補間時間
}

void EnemyWalkState::ManageState() 
{
	// 敵を発見していないならば
	if (m_enemy->m_isFound == false) {
		m_enemy->m_isMove = true;
		return;
	}
	// 咆哮をリセット
	m_enemy->m_screamRateByTime = 0.0f;
	m_enemy->m_isScream = true;
	m_enemy->m_isMove = false;
	// 咆哮が終了するまでのタイマーをリセット。
	m_enemy->m_screamEndTimer = TIME_TO_END_SCREAM_PER_SEC;
	// ステートを咆哮状態にする
	m_enemy->ChangeState(m_enemy->enEnemyState_Scream);
}

void EnemyWalkState::ProcessByState()
{
	// 歩き
	Walk();
	// プレイヤーの音を捜索
	m_enemy->SearchSoundOfPlayer();
}

void EnemyWalkState::Walk()
{
	// 目的地までの距離
	Vector3 distance = m_enemy->m_point->s_position - m_enemy->m_position;

	// 目的地までの距離が近ければ
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
		// 現在最後のポイントにいるならば
		if (m_enemy->m_point->s_number == m_enemy->m_enemyPath.GetPointListSize() - 1) {
			// 最初のポイント情報を取得
			m_enemy->m_point = m_enemy->m_enemyPath.GetFirstPoint();
		}
		// 最後以外のポイントにいるならば
		else {
			// 次のポイント情報を取得
			m_enemy->m_point = m_enemy->m_enemyPath.GetNextPoint(m_enemy->m_point->s_number);
		}

	}

	// 移動する方向を設定
	m_enemy->m_moveVector = distance;
	m_enemy->m_moveVector.Normalize();

	// 歩き時の移動速度を設定
	m_enemy->m_moveSpeed = m_enemy->m_moveVector * WALK_SPEED;

	// 座標に移動速度を加算
	m_enemy->m_position += m_enemy->m_moveSpeed;
}

void EnemyWalkState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Walk,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}