#include "stdafx.h"
#include "EnemyChaseState.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
namespace
{
	const float PATH_FINDING_TIMER = 0.7f;					// �p�X����������Ԋu
	const float ENEMY_RADIUS = 30.0f;						// �G�l�~�[�̔��a
	const float ENEMY_HEIGHT = 200.0f;						// �G�l�~�[�̍���
	const float RUN_SPEED = 9.5f * 0.7f;					// ���鑬��
	const float ATTACKING_RANGE = 120.0f;					// �U���\�Ȕ͈�
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// �A�j���[�V������Ԏ���
}

void EnemyChaseState::ManageState()
{
	// �v���C���[���U���\�ȋ����Ȃ��
	if (m_enemy->m_isAttackable == true) {
		m_enemy->ChangeState(m_enemy->enEnemyState_Attack);
		// �Q�[���I�[�o�[�G�t�F�N�g�̊J�n
		m_enemy->m_game->StartGameOverEffect();
		m_enemy->m_isMove = false;
		return;
	}
	// �G��ǐՂ����Ԃ��ێ�����Ă���Ȃ��
	if (m_enemy->m_chaseTime > 0.0f || m_enemy->m_isFound == true) {
		return;
	}
	// �v���C���[���������Ă����Ȃ��
	if (m_enemy->m_isFound == false) {
		// ���񂷎��Ԃ����Z�b�g
		m_enemy->m_surveyTimer = 0.0f;
		// �X�e�[�g�����񂵏�Ԃɂ���
		m_enemy->ChangeState(m_enemy->enEnemyState_Survey);
		// �p�X����������Ԋu�����Z�b�g
		m_enemy->m_pathFindingTimer = PATH_FINDING_TIMER;
		m_enemy->m_isMove = false;
		m_enemy->m_isChase = false;

	}
}

void EnemyChaseState::ProcessByState()
{
	// �ǐ�
	Chase();
	// �v���C���[�̉���{��
	m_enemy->SearchSoundOfPlayer();
}

void EnemyChaseState::Chase()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_enemy->m_player->GetPosition();

	if (m_enemy->m_pathFindingTimer <= 0.0f) {
		// �v���C���[�܂ł̃p�X������
		m_enemy->m_pathFinding.Execute(
			m_enemy->m_path,
			m_enemy->m_nvmMesh,
			m_enemy->m_position,
			playerPos,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		// �^�C�}�[���Z�b�g
		m_enemy->m_pathFindingTimer = PATH_FINDING_TIMER;
	}

	m_enemy->m_pathFindingTimer -= g_gameTime->GetFrameDeltaTime();

	// �w�肵�����W�ւ̈ړ�������������
	bool isEnd;
	// �ړ����O�̍��W���L�^
	m_enemy->m_lastPosition = m_enemy->m_position;

	// �Œ�ǐՂ���c�莞��
	// �ǐՂ���c�莞�Ԃ����炷
	m_enemy->m_chaseTime -= g_gameTime->GetFrameDeltaTime();
	// �ǐՂ���c�莞�Ԃ�0�ɂȂ����Ȃ�
	if (m_enemy->m_chaseTime < 0.0f) {
		m_enemy->m_chaseTime = 0.0f;
	}


	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_enemy->m_position = m_enemy->m_path.Move(
		m_enemy->m_position,
		RUN_SPEED,
		isEnd
	);

	// ����
	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 momentDist = m_enemy->m_position - m_enemy->m_lastPosition;
	momentDist.Normalize();

	// �ړ����������ݒ�
	m_enemy->m_moveVector = momentDist;

	// �v���C���[�Ƃ̋���
	Vector3 distanceToPlayer = m_enemy->m_position - playerPos;
	// �v���C���[�Ƃ̋������߂����
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// �U�����s��
		m_enemy->m_isAttackable = true;
	}
}

void EnemyChaseState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Run,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}