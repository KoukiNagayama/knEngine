#pragma once
#include "sound/SoundSource.h"

class Player;
class EdgeManagement;
class Bell : public IGameObject
{
public:
	Bell() {};
	~Bell();
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
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// 輪郭線に影響を与える情報を設定。
	/// </summary>
	/// <param name="edgeManagement">輪郭線に影響を与える情報</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
	/// <summary>
	/// ベルの音が鳴っているか。
	/// </summary>
	/// <returns>鳴っていればtrue。鳴っていなければfalse。</returns>
	bool IsBellRinging() const
	{
		return m_isRing;
	}

private:
	/// <summary>
	/// プレイヤーに合わせて移動。
	/// </summary>
	void MoveWithPlayer();
	/// <summary>
	/// 鳴らす。
	/// </summary>
	void Ring();
private:
	Player*				m_player = nullptr;						// プレイヤークラス
	ModelRender			m_bellModel;							// ベルのモデル
	Vector3				m_position = Vector3::Zero;				// 座標
	EdgeManagement*		m_edgeManagement = nullptr;				// 輪郭線を管理
	SoundSource*		m_bellSound = nullptr;					// ベルの音			
	float				m_availableAgainTimerPerSec = 0.0f;		// 再度使用可能になるタイマー
	bool				m_isRing = false;						// ベルの音が鳴っているか。
};

