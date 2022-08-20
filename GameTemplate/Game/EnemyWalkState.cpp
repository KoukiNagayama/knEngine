#include "stdafx.h"
#include "Enemy.h"
#include "EnemyWalkState.h"

namespace
{
	const float TIME_TO_END_SCREAM_PER_SEC = 3.0f;			// ���K���I������܂ł̎���
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;	// �������̖ڕW���W�ւ̓��B������Ƃ�ő勗��
	const float WALK_SPEED = 6.5f * 0.7f;					// ��������
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// �A�j���[�V������Ԏ���
}

void EnemyWalkState::ManageState() 
{
	// �G�𔭌����Ă��Ȃ��Ȃ��
	if (m_enemy->m_isFound == false) {
		m_enemy->m_isMove = true;
		return;
	}
	// ���K�����Z�b�g
	m_enemy->m_screamRateByTime = 0.0f;
	m_enemy->m_isScream = true;
	m_enemy->m_isMove = false;
	// ���K���I������܂ł̃^�C�}�[�����Z�b�g�B
	m_enemy->m_screamEndTimer = TIME_TO_END_SCREAM_PER_SEC;
	// �X�e�[�g����K��Ԃɂ���
	m_enemy->ChangeState(m_enemy->enEnemyState_Scream);
}

void EnemyWalkState::ProcessByState()
{
	// ����
	Walk();
	// �v���C���[�̉���{��
	m_enemy->SearchSoundOfPlayer();
}

void EnemyWalkState::Walk()
{
	// �ړI�n�܂ł̋���
	Vector3 distance = m_enemy->m_point->s_position - m_enemy->m_position;

	// �ړI�n�܂ł̋������߂����
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
		// ���ݍŌ�̃|�C���g�ɂ���Ȃ��
		if (m_enemy->m_point->s_number == m_enemy->m_enemyPath.GetPointListSize() - 1) {
			// �ŏ��̃|�C���g�����擾
			m_enemy->m_point = m_enemy->m_enemyPath.GetFirstPoint();
		}
		// �Ō�ȊO�̃|�C���g�ɂ���Ȃ��
		else {
			// ���̃|�C���g�����擾
			m_enemy->m_point = m_enemy->m_enemyPath.GetNextPoint(m_enemy->m_point->s_number);
		}

	}

	// �ړ����������ݒ�
	m_enemy->m_moveVector = distance;
	m_enemy->m_moveVector.Normalize();

	// �������̈ړ����x��ݒ�
	m_enemy->m_moveSpeed = m_enemy->m_moveVector * WALK_SPEED;

	// ���W�Ɉړ����x�����Z
	m_enemy->m_position += m_enemy->m_moveSpeed;
}

void EnemyWalkState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Walk,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}