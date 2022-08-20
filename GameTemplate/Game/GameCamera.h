#pragma once
class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera() {};
	/// <summary>
	/// 開始処理
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="x">x座標。</param>
	/// <param name="y">y座標。</param>
	/// <param name="z">z座標。</param>
	void SetPosition(float x, float y, float z)
	{
		SetPosition({ x,y,z });
	}
	/// <summary>
	/// 注視点から視点に向かうベクトルの取得
	/// </summary>
	/// <returns>注視点から視点に向かうベクトル</returns>
	const Vector3 GetToCameraPos()
	{
		return m_toCameraPos;
	}
	/// <summary>
	/// 移動可能フラグを設定
	/// </summary>
	/// <param name="movable">移動可能かどうか</param>
	void SetIsMovable(const bool movable)
	{
		m_isMovable = movable;
	}
private:
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	/// <summary>
	/// 注視点の処理。
	/// </summary>
	void ViewPoint();
	/// <summary>
	/// 回転の上限
	/// </summary>
	bool RotationLimit();
private:
	Player*			m_player = nullptr;					// プレイヤー
	Vector3			m_toCameraPos = Vector3::Zero;		// 注視点から視点へのベクトル
	Vector3			m_position = Vector3::Zero;			// カメラの座標
	Quaternion		m_rotation = Quaternion::Identity;	// カメラの回転
	//bool			m_isTargetInRange = true;			// 注視点がカメラの回転の範囲内であるならば
	Vector3			m_target = Vector3::Zero;			// 注視点
	bool			m_isMovable = true;					// 移動が可能か？

};	

