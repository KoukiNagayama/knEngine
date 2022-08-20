#include "stdafx.h"
#include "EnemyAttackState.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"

namespace
{
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;		// �A�j���[�V������Ԏ���
}

void EnemyAttackState::ManageState()
{
	// �Q�[���I�[�o�[
	m_enemy->m_attackingTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_enemy->m_attackingTimer <= 0.0f && m_enemy->m_isCaughtPlayer == false) {
		m_enemy->m_isCaughtPlayer = true;
		m_enemy->m_game->SetGameOver();
	}
}

void EnemyAttackState::ProcessByState()
{
	// �U��
	Attack();
}

void EnemyAttackState::Attack()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_enemy->m_player->GetPosition();

	Vector3 distance = playerPos - m_enemy->m_position;
	distance.Normalize();

	m_enemy->m_moveVector = distance;
}

void EnemyAttackState::PlayAnimation()
{
	m_enemy->m_enemyModel.PlayAnimation(
		m_enemy->enAnimationClip_Attack,
		INTERPOLATION_TIME_FOR_ANIMATION
	);
}