#pragma once
class EdgeManagement;
class BackGround : public IGameObject
{
public:
	BackGround() {};
	~BackGround();
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
	/// 描画処理
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 輪郭線に影響を与える情報を設定。
	/// </summary>
	/// <param name="edgeManagement">輪郭線に影響を与える情報</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
private:
	ModelRender				m_inGameStage;							// ステージのモデル
	Vector3					m_position = Vector3::Zero;				// 座標
	Quaternion				m_rotation = Quaternion::Identity;		// 回転
	Vector3					m_scale = Vector3::One;					// 拡大率
	EdgeManagement*			m_edgeManagement = nullptr;				// 輪郭線に影響を与える情報
	PhysicsStaticObject		m_physicsStaticObject;					// 静的物理オブジェクト								
};

