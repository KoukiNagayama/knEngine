#pragma once
class Enemy : IGameObject
{
public:
	Enemy() {};
	~Enemy() {};
	
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
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// エネミーの番号を設定
	/// </summary>
	/// <param name="number">番号</param>
	void SetNumber(const int& number)
	{
		m_enemyNumber = number;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
private:

private:
	// ステート
	enum EnEnemyState {
		enEnemyState_Walk,				// 歩き	
		enEnemyState_Scream,			// 咆哮
		enEnemyState_Chase,				// 追跡
		enEnemyState_Survey,			// 見回し
		enEnemyState_ReturnToPath,		// パス移動に戻る
		enEnemyState_Attack,			// 攻撃
		enEnemyState_Num,				// ステートの数
	};

	// アニメーション
	enum EnAnimationClip {
		enAnimationClip_Walk,			// 歩き
		enAnimationClip_Run,			// 走り
		enAnimationClip_Scream,			// 咆哮
		enAnimationClip_Survey,			// 見回し
		enAnimationClip_Attack,			// 攻撃
		enAnimationClip_Num,			// アニメーションの数
	};

	Vector3					m_position = Vector3::Zero;				// 座標
	Quaternion				m_rotation = Quaternion::Identity;		// 回転
	Vector3					m_scale = Vector3::One;					// 拡大率
	int						m_enemyNumber;							// レベル上で登録されたエネミーの番号
	AnimationClip			m_animationClips[enAnimationClip_Num];	// アニメーションクリップ
	Vector3					m_moveVector;							// 移動する方向
	Vector3					m_moveSpeed;							// 移動する速さ
	EnEnemyState			m_enemyState = enEnemyState_Walk;		// ステート

};

