#pragma once
#include "LevelRender.h"
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
	void PlayAnimation();
	void Rotation();
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Num
	};
	ModelRender		m_unityChan;
	Vector3			m_pos;
	Vector3			m_sca;
	Quaternion		m_rot;
	AnimationClip	m_animationClips[enAnimationClip_Num];
	ModelRender		m_backGround;
	//EdgeControl		m_edgeControl;
	LevelRender		m_levelRender;
};

