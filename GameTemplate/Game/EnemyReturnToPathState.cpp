#include "stdafx.h"
#include "EnemyReturnToPathState.h"
#include "Enemy.h"

namespace
{
	const float TIME_TO_END_SCREAM_PER_SEC = 3.0f;			// ���K���I������܂ł̎���
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;	// �A�Ҏ��ɖڕW�n�_�ւ̓��B��������ő勗��
	const float ENEMY_RADIUS = 30.0f;						// �G�l�~�[�̔��a
	const float ENEMY_HEIGHT = 200.0f;						// �G�l�~�[�̍���
	const float WALK_SPEED = 6.5f * 0.7f;					// ��������
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// �A�j���[�V������Ԏ���
}

void EnemyReturnToPathState::ManageState()
{
	// �ڕW�̍��W�ƌ��݂̍��W�̋����𑪂�
	Vector3 distance = m_enemy->m_point->s_position - m_enemy->m_position;

	m_enemy->m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// �v���C���[�𔭌�������
	if (m_enemy->m_isFound == true) {
		// ���K�����Z�b�g
		m_enemy->m_screamRateByTime = 0.0f;
		m_enemy->m_isScream = true;
		// ���K���I������܂ł̃^�C�}�[�����Z�b�g�B
		m_enemy->m_screamEndTimer = TIME_TO_END_SCREAM_PER_SEC;
		// �X�e�[�g����K��Ԃɂ���
		m_enemy->ChangeState(m_enemy->enEnemyState_Scream);

		m_enemy->m_isMove = false;
	}
	// �ڕW�̍��W�ɋ߂��Ȃ�����
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// �X�e�[�g�������Ԃɂ���
		m_enemy->ChangeState(m_enemy->enEnemyState_Walk);

	}
	// ��莞�Ԉȓ��ɋA�҂ł��Ȃ������ꍇ
	else if (m_enemy->m_timeToReturn <= 0.0f) {
		// �����I�ɃX�e�[�g�������Ԃɂ���
		m_enemy->ChangeState(m_enemy->enEnemyState_Walk);

	}
	m_enemy->m_isPathFindingWhileReturning = false;
}

void EnemyReturnToPathState::ProcessByState()
{
	// �p�X�ړ��ւ̋A��
	ReturnToPath();
	// �v���C���[�̉���{��
	m_enemy->SearchSoundOfPlayer();
}

void EnemyReturnToPathState::ReturnToPath()
{
	bool isEnd;

	// �ړ����O�̍��W���L�^
	m_enemy->m_lastPosition = m_enemy->m_position;

	// �p�X���������Ă��Ȃ��Ȃ��
	if (m_enemy->m_isPathFindingWhileReturning == false) {
		// ���݂̍��W�����ԋ߂����W�̃|�C���g���擾
		m_enemy->m_point = m_enemy->m_enemyPath.GetNearPoint(m_enemy->m_position);
		// �A�ҖڕW�̃p�X�܂ł̃p�X������
		m_enemy->m_pathFinding.Execute(
			m_enemy->m_path,
			m_enemy->m_nvmMesh,
			m_enemy->m_position,
			m_enemy->m_point->s_position,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		m_enemy->m_isPathFindingWhileReturning = true;
	}

	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_enemy->m_position = m_enemy->m_path.Move(
		m_enemy->m_position,
		WALK_SPEED,
		isEnd
	);

	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 distance = m_enemy->m_position - m_enemy->m_lastPosition;
	distance.Normalize();

	// �ړ����������ݒ�
	m_enemy->m_moveVector = distance;
}

void EnemyReturnToPathState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Walk,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}
