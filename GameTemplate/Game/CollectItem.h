#pragma once
#include "sound/SoundSource.h"

class EdgeManagement;
class Player;
class Game;
class Bell;
class CollectItem : public IGameObject
{
public:
	CollectItem() {};
	~CollectItem() {};

	/// <summary>
	/// 開始処理。
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト。</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 輪郭線に影響を与える情報を設定。
	/// </summary>
	/// <param name="edgeManagement">輪郭線に影響を与える情報。</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
	const void SetPosition(Vector3 position)
	{
		m_position = position;
	}
private:
	/// <summary>
	/// 回転。
	/// </summary>
	void Rotation();
	/// <summary>
	/// プレイヤーとの距離を調べる。
	/// </summary>
	void CheckDistanceToPlayer();
	/// <summary>
	/// 再び出現するまでのカウント。
	/// </summary>
	void CountAppearsAgain();
	/// <summary>
	/// アイテムを取得する。
	/// </summary>
	void RetrieveItem();

private:
	ModelRender				m_itemModel;							// アイテムのモデル
	Vector3					m_position = Vector3::Zero;				// 座標
	Quaternion				m_rotation = Quaternion::Identity;		// 回転
	Vector3					m_scale = Vector3::One;					// 拡大率
	EdgeManagement*			m_edgeManagement = nullptr;				// 輪郭線制御
	bool					m_isDraw = true;						// 描画するか
	Player*					m_player = nullptr;						// プレイヤー
	float					m_drawAgainTimerPerSec = 0.0f;			// 再び描画されるようになるまでのタイマー(単位:秒)
	Game*					m_game = nullptr;						// ゲーム
	Vector3					m_playerPos = Vector3::Zero;			// プレイヤーの座標
	Vector3					m_distToPlayer = Vector3::Zero;			// プレイヤーとの距離
	int						m_currentColor;							// 現在の色
	Bell*					m_bell = nullptr;						// ベル
	float					m_resetColorTimerPerSec = 0.0f;			// 色をリセットするタイマー(単位:秒)
	SoundSource*			m_pickUpBellSound = nullptr;
};

