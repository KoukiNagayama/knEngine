#pragma once

#include "IEnemyState.h"

class EnemyAttackState : public IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">エネミー</param>
	EnemyAttackState(Enemy* enemy) :
		IEnemyState(enemy) {}
private:
	/// <summary>
	/// ステート管理処理
	/// </summary>
	void ManageState() override;
	/// <summary>
	/// ステートごとの処理
	/// </summary>
	void ProcessByState() override;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// アニメーション再生処理
	/// </summary>
	void PlayAnimation() override;
};

