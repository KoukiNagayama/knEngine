#include "stdafx.h"
#include "EngineTest.h"
#include "LevelRender.h"
bool EngineTest::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);


	m_pos = { 1.00f,1.00f,1.00f };
	g_edgeControl.Init(0, m_pos, 3000.0f, 1.0f);
	g_edgeControl.SetIsSound(0, 1);

	/*m_unityChan.Init(
		"Assets/modelData/unityChan.tkm", 
		m_animationClips, 
		enAnimationClip_Num, 
		enModelUpAxisY, 
		0
	);

	m_unityChan.SetTRS(Vector3::Zero, Quaternion::Identity, Vector3::One);

	m_backGround.Init(
		"Assets/modelData/stage/2ndFloor/Floor2.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		0
	);
	m_backGround.SetPosition(Vector3::Zero);
	*/
	m_levelRender.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData) {
		//ステージ
		if (objData.EqualObjectName(L"floor2") == true) {

			m_backGround.Init(
				"Assets/modelData/stage/2ndFloor/Floor2.tkm",
				nullptr,
				0,
				enModelUpAxisZ,
				0
			);
			m_backGround.SetPosition(objData.position);
			return true;
		}
		//プレイヤー(カメラ)
		if (objData.EqualObjectName(L"player") == true) {

			m_unityChan.Init(
				"Assets/modelData/unityChan.tkm",
				m_animationClips,
				enAnimationClip_Num,
				enModelUpAxisY,
				0
			);
			m_pos = objData.position;
			m_unityChan.SetPosition(m_pos);
			
			//g_camera3D->SetPosition(objData.position);
			return true;
		}
		// エネミー
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			return true;
		}
		//つみき
		if (objData.EqualObjectName(L"crashtoy") == true) {
			return true;
		}
		//蓄音機
		if (objData.ForwardMatchName(L"gramophone") == true)
		{
			return true;
		}

		//レコード
		if (objData.ForwardMatchName(L"record") == true)
		{
			return true;
		}
		if (objData.ForwardMatchName(L"door") == true)
		{
			return true;
		}
		if (objData.ForwardMatchName(L"switchfloor") == true)
		{
			return true;
		}
		if (objData.ForwardMatchName(L"gimmick") == true)
		{
			return true;
		}
		if (objData.EqualObjectName(L"crowbar") == true)
		{
			return true;
		}
	});
	


	return true;
}

void EngineTest::Update()
{
	Move();

	PlayAnimation();

	Rotation();

	m_unityChan.Update();

	m_backGround.Update();

	g_edgeControl.SetPosition(0, m_pos);
}

void EngineTest::Move()
{
	m_pos.x += g_pad[0]->GetLStickXF();
	m_pos.z += g_pad[0]->GetLStickYF();

	m_unityChan.SetPosition(m_pos);

}

void EngineTest::Rotation()
{
	m_rot.AddRotationDegY(1.0f);
	m_unityChan.SetRotation(m_rot);
}

void EngineTest::PlayAnimation()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_unityChan.PlayAnimation(enAnimationClip_Idle, 0.3f);
	}
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		m_unityChan.PlayAnimation(enAnimationClip_Walk, 0.3f);
	}
}

void EngineTest::Render(RenderContext& rc)
{
	m_unityChan.Draw();

	m_backGround.Draw();

	//m_levelRender.Draw(rc);
}
