#pragma once

#include "IEnemyState.h"

class EnemyReturnToPathState : public IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">エネミー</param>
	EnemyReturnToPathState(Enemy* enemy) :
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
	/// パス移動への帰還
	/// </summary>
	void ReturnToPath();
	/// <summary>
	/// アニメーション再生処理
	/// </summary>
	void PlayAnimation() override;
};

