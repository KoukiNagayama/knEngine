#pragma once

#include "IEnemyState.h"

class EnemyWalkState : public IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">エネミー</param>
	EnemyWalkState(Enemy* enemy) :
		IEnemyState(enemy){}
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
	/// 歩き
	/// </summary>
	void Walk();
	/// <summary>
	/// アニメーション再生処理
	/// </summary>
	void PlayAnimation() override;
};

