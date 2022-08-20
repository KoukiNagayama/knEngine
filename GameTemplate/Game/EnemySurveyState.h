#pragma once

#include "IEnemyState.h"

class EnemySurveyState : public IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">エネミー</param>
	EnemySurveyState(Enemy* enemy) :
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
	/// 見回し
	/// </summary>
	void Survey();
	/// <summary>
	/// アニメーション再生処理
	/// </summary>
	void PlayAnimation() override;
};

