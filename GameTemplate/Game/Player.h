#pragma once
class GameCamera;
class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};
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
		SetPosition({ x, y, z });
	}
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	const Vector3& GetPosition() const 
	{
		return m_position;
	}
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
private:
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// ステートによる処理。
	/// </summary>
	void ProcessByState();

private:
	enum EnPlayerState {
		enPlayerState_Idle,		// 待機
		enPlayerState_Walk,		// 歩き
		enPlayerState_Stop		// 停止
	};
	GameCamera*				m_gameCamera = nullptr;				// ゲームカメラクラス
	Vector3					m_position = Vector3::Zero;			// 座標
	Quaternion				m_rotation = Quaternion::Identity;	// 回転
	Vector3					m_moveSpeed = Vector3::Zero;		// 移動速度
	CharacterController		m_charaCon;							// キャラクターコントローラー
	EnPlayerState			m_playerState = enPlayerState_Idle;	// プレイヤーステート
	
};

