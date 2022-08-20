#pragma once
class Enemy;
class Game;

class IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">エネミー</param>
	IEnemyState(Enemy* enemy) : 
		m_enemy(enemy)
	{

	}
	/// <summary>
	/// ステート管理処理
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// ステートごとの処理
	/// </summary>
	virtual void ProcessByState() = 0;
	/// <summary>
	/// アニメーション再生処理
	/// </summary>
	virtual void PlayAnimation() = 0;
protected:
	Enemy* m_enemy = nullptr;
};

