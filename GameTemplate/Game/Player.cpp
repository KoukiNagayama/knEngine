#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"


namespace
{
	const float PLAYER_RADIUS = 50.0f;								// �v���C���[�̔��a
	const float PLAYER_HEIGHT = 170.0f;								// �v���C���[�̍���
	const float WALK_SPEED = 500.0f;								// ��������
	const float FRICTION = 0.5f;									// ���C
	const float END_OF_MOVE = 0.001f;								// 1�t���[���̈ړ��ʂɂ���Ĉړ��I����\���x�N�g��
	const float Y_AXIS_MOVING_AMOUNT = 0.0f;						// Y���̈ړ���
	const int	PLAYER_FOOTSTEP_SOUND_NUMBER_TO_REGISTER = 1;		// �o�^����v���C���[�̑����̔ԍ�
	const float FOOTSTEP_VOLUME = 0.7f;								// �����̉���
	const float SPEED_OF_MOVING_JUDGE = 0.001f;						// �ړ����Ă��锻��p�̃X�s�[�h
	const int PLAYER_FOOTSTEP_SOUND_PRIORITY = 0;					// �����̉����I�u�W�F�N�g�̃v���C�I���e�B
}

Player::Player()
{

}
Player::~Player()
{
	if (m_footstepSound != nullptr) {
		DeleteGO(m_footstepSound);
	}

	m_charaCon.RemoveRigidBoby();
}

bool Player::Start()
{
	// �L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT, m_position);

	m_gameCamera = FindGO<GameCamera>("gameCamera");
	
	g_soundEngine->ResistWaveFileBank(PLAYER_FOOTSTEP_SOUND_NUMBER_TO_REGISTER, "Assets/sound/walk.wav");

	m_footstepSound = NewGO<SoundSource>(PLAYER_FOOTSTEP_SOUND_PRIORITY);
	m_footstepSound->Init(PLAYER_FOOTSTEP_SOUND_NUMBER_TO_REGISTER);
	m_footstepSound->SetVolume(FOOTSTEP_VOLUME);
	return true;
}

void Player::Update()
{
	// �ړ��s�\�Ȃ�S�Ă̏����𒆒f����
	if (m_isMovable == false) {
		return;
	}

	// �ړ�
	Move();

	// ��]
	Rotation();

	// �X�e�[�g�Ǘ�
	ManageState();

	ProcessByState();
}

void Player::Move()
{
	// ���X�e�B�b�N�̓��͗ʂ��v�Z
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	// �J�����̑O�����ƉE�����̃x�N�g�����擾�B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	// Y�����ɂ͈ړ������Ȃ��B
	forward.y = Y_AXIS_MOVING_AMOUNT;
	right.y = Y_AXIS_MOVING_AMOUNT;

	// �ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right * stickL.x * WALK_SPEED;
	m_moveSpeed += forward * stickL.y * WALK_SPEED;

	// �ړ��ɖ��C��t�^����B
	m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
	m_moveSpeed.z -= m_moveSpeed.z * FRICTION;

	// �ړ����x������l�ȉ���������
	if (m_moveSpeed.Length() < END_OF_MOVE){
		// ����ɖ��C��t�^����B
		m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
		m_moveSpeed.z -= m_moveSpeed.z * FRICTION;
	}

	// �L�����R�����g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::Rotation()
{
	//�J�����̌������擾
	Vector3 rotVec = m_gameCamera->GetToCameraPos();
	//y�̒l�̏��͗v��Ȃ��̂ŁA0.0f�������Đ��K���B
	rotVec.y = 0.0f;
	rotVec.Normalize();

	//���݂ǂꂾ����]���Ă���̂��̊p�x�����߂�B
	float angle = atan2(rotVec.x, rotVec.z);
	//���̊p�x���g���āAY������Quaternion��ݒ肷��B
	m_rotation.SetRotation(Vector3::AxisY, angle);
}

void Player::ProcessByState()
{
	switch (m_playerState) {
	case enPlayerState_Idle:
		StopFootstepSound();
		break;
	case enPlayerState_Walk:
		PlayFootstepSound();
		break;
	}
}

void Player::ManageState()
{
	switch (m_playerState) {
	case enPlayerState_Idle:
		ProcessCommonStateTransition();
		break;
	case enPlayerState_Walk:
		ProcessCommonStateTransition();
		break;
	case enPlayerState_Stop:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= SPEED_OF_MOVING_JUDGE 
		|| fabsf(m_moveSpeed.z) >= SPEED_OF_MOVING_JUDGE
		)
	{
		m_playerState = enPlayerState_Walk;
	}
	else {
		m_playerState = enPlayerState_Idle;
	}

}

void Player::PlayFootstepSound()
{
	// �炷�B
	m_footstepSound->Play(true);
	m_isFootstepSound = true;
}

void Player::StopFootstepSound()
{
	// �~�߂�B
	m_footstepSound->Pause();
	m_isFootstepSound = false;
}

void Player::Render(RenderContext& rc)
{

}
