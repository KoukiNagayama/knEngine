#include "stdafx.h"
#include "EnemySurveyState.h"
#include "Enemy.h"

namespace
{
	const float MINIMUM_CHASE_TIME = 20.0f;					// �Œ���ǐՂ��鎞��
	const float TIME_TO_LOSE_SIGHT = 5.0f;					// �v���C���[��������������
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;		// �����I�ɃX�e�[�g��J�ڂ�����܂ł̎���
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;	// �A�j���[�V������Ԏ���
}

void EnemySurveyState::ManageState()
{
	// ��莞�Ԉȓ��Ƀv���C���[�𔭌�
	if (m_enemy->m_isFound == true) {
		// �Œ���ǐՂ��鎞�Ԃ��w��
		m_enemy->m_chaseTime = MINIMUM_CHASE_TIME;
		// �ǐՂ��J�n
		m_enemy->ChangeState(m_enemy->enEnemyState_Chase);
		// �ǐՂ���B
		m_enemy->m_isChase = true;

		m_enemy->m_isMove = true;
	}
	// ��莞�ԃv���C���[���������Ă�����
	if (m_enemy->m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// �A�ҏ�Ԃ̎��Ԃ��w��
		m_enemy->m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// �X�e�[�g���p�X�ւ̋A�ҏ�Ԃɂ���
		m_enemy->ChangeState(m_enemy->enEnemyState_ReturnToPath);

		m_enemy->m_isMove = true;
	}
}

void EnemySurveyState::ProcessByState()
{
	// ����
	Survey();
	// �v���C���[�̉���{��
	m_enemy->SearchSoundOfPlayer();
}

void EnemySurveyState::Survey()
{
	// ���񂵂����Ԃ����Z
	m_enemy->m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void EnemySurveyState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Survey, 
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}
