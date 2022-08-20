#include "stdafx.h"
#include "EnemyScreamState.h"
#include "Enemy.h"

namespace
{
	const float MINIMUM_CHASE_TIME = 20.0f;						// �Œ���ǐՂ��鎞��
	const int	ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER = 4;		// �G�l�~�[�̙��K�̉����̓o�^�ԍ�
	const float SCREAM_VOLUME = 1.5f;							// ���K�̉���
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;		// �A�j���[�V������Ԏ���
}

void EnemyScreamState::ManageState()
{
	// ���K���I�����Ă��Ȃ��Ȃ��
	if (m_enemy->m_isScream == true) {
		return;
	}
	m_enemy->m_isMove = true;
	// �Œ���ǐՂ��鎞�Ԃ��w��
	m_enemy->m_chaseTime = MINIMUM_CHASE_TIME;
	// �X�e�[�g��ǐՏ�Ԃɂ���
	m_enemy->ChangeState(m_enemy->enEnemyState_Chase);
	// �ǐՂ��Ă���悤�ɋL�^�B
	m_enemy->m_isChase = true;
}

void EnemyScreamState::ProcessByState()
{
	Scream();
}

void EnemyScreamState::Scream()
{
	// ���K�̉�����炷
	if (m_enemy->m_screamSound == nullptr) {
		m_enemy->m_screamSound = NewGO<SoundSource>(0);
		m_enemy->m_screamSound->Init(ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER);
		m_enemy->m_screamSound->SetVolume(SCREAM_VOLUME);
		m_enemy->m_screamSound->Play(false);
		m_enemy->m_isScream = true;
	}

	// �^�C�}�[�B
	m_enemy->m_screamEndTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_enemy->m_screamEndTimer <= 0.0f) {
		m_enemy->m_isScream = false;
		m_enemy->m_screamSound = nullptr;
	}
}

void EnemyScreamState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Scream,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}
