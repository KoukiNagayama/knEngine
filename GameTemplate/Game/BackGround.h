#pragma once
class BackGround : public IGameObject
{
public:
	BackGround() {};
	~BackGround() {};
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

private:
	ModelRender			m_inGameStage;
	Vector3				m_position = Vector3::Zero;
	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_scale = Vector3::One;
};

