#pragma once
class EngineTest : public IGameObject
{
public:
	EngineTest() {};
	~EngineTest() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	void Move();
private:
	ModelRender		m_unityChan;
	Vector3			m_pos;
	Vector3			m_sca;
	Quaternion		m_rot;
};

