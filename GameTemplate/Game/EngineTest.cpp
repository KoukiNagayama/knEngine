#include "stdafx.h"
#include "EngineTest.h"

bool EngineTest::Start()
{
	m_unityChan.Init("Assets/modelData/unityChan.tkm");

	m_unityChan.SetTRS(Vector3::Zero, Quaternion::Identity, Vector3::One);

	return true;
}

void EngineTest::Update()
{
	Move();

	m_unityChan.Update();
}

void EngineTest::Move()
{
	m_pos.x += g_pad[0]->GetLStickXF();
	m_pos.z += g_pad[0]->GetLStickYF();

	m_unityChan.SetPosition(m_pos);
}

void EngineTest::Render(RenderContext& rc)
{
	m_unityChan.Draw();
}
