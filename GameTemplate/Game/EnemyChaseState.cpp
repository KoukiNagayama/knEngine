#include "stdafx.h"
#include "EnemyChaseState.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
namespace
{
	const float PATH_FINDING_TIMER = 0.7f;					// パスを検索する間隔
	const float ENEMY_RADIUS = 30.0f;						// エネミーの半径
	const float ENEMY_HEIGHT = 200.0f;						// エネミーの高さ
	const float RUN_SPEED = 9.5f * 0.7f;					// 走る速さ
	const float ATTACKING_RANGE = 120.0f;					// 攻撃可能な範囲
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// アニメーション補間時間
}

void EnemyChaseState::ManageState()
{
	// プレイヤーを攻撃可能な距離ならば
	if (m_enemy->m_isAttackable == true) {
		m_enemy->ChangeState(m_enemy->enEnemyState_Attack);
		// ゲームオーバーエフェクトの開始
		m_enemy->m_game->StartGameOverEffect();
		m_enemy->m_isMove = false;
		return;
	}
	// 敵を追跡する状態が維持されているならば
	if (m_enemy->m_chaseTime > 0.0f || m_enemy->m_isFound == true) {
		return;
	}
	// プレイヤーを見失っていたならば
	if (m_enemy->m_isFound == false) {
		// 見回す時間をリセット
		m_enemy->m_surveyTimer = 0.0f;
		// ステートを見回し状態にする
		m_enemy->ChangeState(m_enemy->enEnemyState_Survey);
		// パスを検索する間隔をリセット
		m_enemy->m_pathFindingTimer = PATH_FINDING_TIMER;
		m_enemy->m_isMove = false;
		m_enemy->m_isChase = false;

	}
}

void EnemyChaseState::ProcessByState()
{
	// 追跡
	Chase();
	// プレイヤーの音を捜索
	m_enemy->SearchSoundOfPlayer();
}

void EnemyChaseState::Chase()
{
	// ターゲットとなるプレイヤーの座標を取得
	Vector3	playerPos = m_enemy->m_player->GetPosition();

	if (m_enemy->m_pathFindingTimer <= 0.0f) {
		// プレイヤーまでのパスを検索
		m_enemy->m_pathFinding.Execute(
			m_enemy->m_path,
			m_enemy->m_nvmMesh,
			m_enemy->m_position,
			playerPos,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		// タイマーリセット
		m_enemy->m_pathFindingTimer = PATH_FINDING_TIMER;
	}

	m_enemy->m_pathFindingTimer -= g_gameTime->GetFrameDeltaTime();

	// 指定した座標への移動が完了したか
	bool isEnd;
	// 移動直前の座標を記録
	m_enemy->m_lastPosition = m_enemy->m_position;

	// 最低追跡する残り時間
	// 追跡する残り時間を減らす
	m_enemy->m_chaseTime -= g_gameTime->GetFrameDeltaTime();
	// 追跡する残り時間が0になったなら
	if (m_enemy->m_chaseTime < 0.0f) {
		m_enemy->m_chaseTime = 0.0f;
	}


	// 設定されたパスをもとに移動
	m_enemy->m_position = m_enemy->m_path.Move(
		m_enemy->m_position,
		RUN_SPEED,
		isEnd
	);

	// 方向
	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 momentDist = m_enemy->m_position - m_enemy->m_lastPosition;
	momentDist.Normalize();

	// 移動する方向を設定
	m_enemy->m_moveVector = momentDist;

	// プレイヤーとの距離
	Vector3 distanceToPlayer = m_enemy->m_position - playerPos;
	// プレイヤーとの距離が近ければ
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// 攻撃を行う
		m_enemy->m_isAttackable = true;
	}
}

void EnemyChaseState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Run,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}