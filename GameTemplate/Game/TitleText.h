#pragma once
class EdgeManagement;
class TitleText : public IGameObject
{
public:
	TitleText() {};
	~TitleText() {};
	/// <summary>
	/// 開始処理
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
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
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 輪郭線に影響を与える情報を設定。
	/// </summary>
	/// <param name="edgeManagement">輪郭線に影響を与える情報。</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
private:
	ModelRender			m_textModel;			// モデルレンダー
	Vector3				m_position;				// 座標
	Vector3				m_scale;				// 拡大率
	Quaternion			m_rotation;				// 回転
	EdgeManagement*		m_edgeManagement = nullptr;
};

