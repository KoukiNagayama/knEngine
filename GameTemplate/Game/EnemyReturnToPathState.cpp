#include "stdafx.h"
#include "EnemyReturnToPathState.h"
#include "Enemy.h"

namespace
{
	const float TIME_TO_END_SCREAM_PER_SEC = 3.0f;			// 咆哮が終了するまでの時間
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;	// 帰還時に目標地点への到達判定を取る最大距離
	const float ENEMY_RADIUS = 30.0f;						// エネミーの半径
	const float ENEMY_HEIGHT = 200.0f;						// エネミーの高さ
	const float WALK_SPEED = 6.5f * 0.7f;					// 歩く速さ
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// アニメーション補間時間
}

void EnemyReturnToPathState::ManageState()
{
	// 目標の座標と現在の座標の距離を測る
	Vector3 distance = m_enemy->m_point->s_position - m_enemy->m_position;

	m_enemy->m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// プレイヤーを発見したら
	if (m_enemy->m_isFound == true) {
		// 咆哮をリセット
		m_enemy->m_screamRateByTime = 0.0f;
		m_enemy->m_isScream = true;
		// 咆哮が終了するまでのタイマーをリセット。
		m_enemy->m_screamEndTimer = TIME_TO_END_SCREAM_PER_SEC;
		// ステートを咆哮状態にする
		m_enemy->ChangeState(m_enemy->enEnemyState_Scream);

		m_enemy->m_isMove = false;
	}
	// 目標の座標に近くなったら
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// ステートを歩き状態にする
		m_enemy->ChangeState(m_enemy->enEnemyState_Walk);

	}
	// 一定時間以内に帰還できなかった場合
	else if (m_enemy->m_timeToReturn <= 0.0f) {
		// 強制的にステートを歩き状態にする
		m_enemy->ChangeState(m_enemy->enEnemyState_Walk);

	}
	m_enemy->m_isPathFindingWhileReturning = false;
}

void EnemyReturnToPathState::ProcessByState()
{
	// パス移動への帰還
	ReturnToPath();
	// プレイヤーの音を捜索
	m_enemy->SearchSoundOfPlayer();
}

void EnemyReturnToPathState::ReturnToPath()
{
	bool isEnd;

	// 移動直前の座標を記録
	m_enemy->m_lastPosition = m_enemy->m_position;

	// パスを検索していないならば
	if (m_enemy->m_isPathFindingWhileReturning == false) {
		// 現在の座標から一番近い座標のポイントを取得
		m_enemy->m_point = m_enemy->m_enemyPath.GetNearPoint(m_enemy->m_position);
		// 帰還目標のパスまでのパスを検索
		m_enemy->m_pathFinding.Execute(
			m_enemy->m_path,
			m_enemy->m_nvmMesh,
			m_enemy->m_position,
			m_enemy->m_point->s_position,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		m_enemy->m_isPathFindingWhileReturning = true;
	}

	// 設定されたパスをもとに移動
	m_enemy->m_position = m_enemy->m_path.Move(
		m_enemy->m_position,
		WALK_SPEED,
		isEnd
	);

	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 distance = m_enemy->m_position - m_enemy->m_lastPosition;
	distance.Normalize();

	// 移動する方向を設定
	m_enemy->m_moveVector = distance;
}

void EnemyReturnToPathState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Walk,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}
