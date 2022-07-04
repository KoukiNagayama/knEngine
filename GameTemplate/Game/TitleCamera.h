#pragma once
class TitleText;
class TitleCamera : public IGameObject
{
public:
	TitleCamera() {};
	~TitleCamera() {};

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
	/// 視点を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_cameraPos = position;
	}
	/// <summary>
	/// 視点を取得
	/// </summary>
	/// <returns>座標</returns>
	Vector3 GetPosition() const
	{
		return m_cameraPos;
	}
private:
	Vector3				m_cameraPos;			// 座標(視点)	
	Vector3				m_toCameraPos;			// 注視点から視点に向かうベクトル
	TitleText*			m_tText = nullptr;		// タイトルテキスト
};

