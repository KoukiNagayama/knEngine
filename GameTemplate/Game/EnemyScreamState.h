#pragma once

#include "IEnemyState.h"

class EnemyScreamState : public IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">エネミー</param>
	EnemyScreamState(Enemy* enemy) :
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
	/// 咆哮
	/// </summary>
	void Scream();
	/// <summary>
	/// アニメーション再生処理
	/// </summary>
	void PlayAnimation() override;
};

