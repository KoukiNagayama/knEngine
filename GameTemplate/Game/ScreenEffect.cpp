#include "stdafx.h"
#include "ScreenEffect.h"
#include "Enemy.h"
#include "Player.h"

namespace
{
	const float SPRITE_W = 1920.0f;					// �X�v���C�g�̉���
	const float SPRITE_H = 1080.0f;					// �X�v���C�g�̏c��
	const float INIT_MIN_DIST_TO_ENEMY = 10000.0f;	// �G�l�~�[�Ƃ̍ŒZ�����̏������l
	const float MAX_ALPHA_VALUE = 1.0f;				// ���l�̍ő�l
	const float MIN_ALPHA_VALUE = 0.0f;				// ���l�̍ŏ��l
	const float MAX_RANGE_TO_ENEMY = 1500.0f;		// �G�l�~�[�Ƃ̍ő勗��
	const int IDENTIFICATION_OF_ENEMY1 = 0;			// �G�l�~�[1�̎��ʂ���l
	const int IDENTIFICATION_OF_ENEMY2 = 1;			// �G�l�~�[2�̎��ʂ���l
	const float MULTIPLY_FADE_OUT = 0.3f;			// �t�F�[�h�A�E�g�ɏ�Z����l
	const float RGB_VALUE = 1.0f;					// RGB�̒l

}

bool ScreenEffect::Start()
{
	// �X�N���[���G�t�F�N�g�̏������B
	m_screenEffect.Init("Assets/sprite/screenEffect_test.DDS", SPRITE_W, SPRITE_H);
	m_screenEffect.SetPosition(Vector3::Zero);

	// �G�l�~�[������
	const auto& enemys = FindGOs<Enemy>("enemy");
	// �G�l�~�[�̐�
	const int size = enemys.size();
	
	// �G�l�~�[�̃I�u�W�F�N�g�̃A�h���X��U�蕪����B
	for (int i = 0; i < size; i++) {
		switch (i) {
		case IDENTIFICATION_OF_ENEMY1:
			m_enemy1 = enemys[i];
			break;
		case IDENTIFICATION_OF_ENEMY2:
			m_enemy2 = enemys[i];
			break;
		}
	}

	// �v���C���[������
	m_player = FindGO<Player>("player");


	return true;
}

void ScreenEffect::Update()
{

	// �X�e�[�g�Ǘ��B
	ManageState();

	if (m_enScreenEffectState == enState_Idle) {
		return;
	}

	// ���l��ύX����B
	ChangeAlphaValue();
}

void ScreenEffect::ChangeAlphaValue()
{
	// �v���C���[�̍��W���擾
	m_playerPos = m_player->GetPosition();

	// �G�l�~�[1
	if (m_enemy1->IsChase()) {
		// �G�l�~�[�̍��W
		Vector3 enemyPos = m_enemy1->GetPosition();
		// �v���C���[�Ƃ̋���
		Vector3 distToPlayer = m_playerPos - enemyPos;

		if (distToPlayer.Length() <= m_shortestDistanceToEnemy) {
			// �ŒZ�������X�V�B
			m_shortestDistanceToEnemy = distToPlayer.Length();
		}
	}

	// �G�l�~�[2
	if (m_enemy2->IsChase()) {
		// �G�l�~�[�̍��W
		Vector3 enemyPos = m_enemy2->GetPosition();
		// �v���C���[�Ƃ̋���
		Vector3 distToPlayer = m_playerPos - enemyPos;

		if (distToPlayer.Length() <= m_shortestDistanceToEnemy) {
			// �ŒZ�������X�V�B
			m_shortestDistanceToEnemy = distToPlayer.Length();
		}
	}

	// �ŒZ�������X�V����Ă���Ȃ�΁B
	if (m_shortestDistanceToEnemy != INIT_MIN_DIST_TO_ENEMY) {
		// ���݂̃��l���X�V�B
		m_currentAlpha = MAX_ALPHA_VALUE - (m_shortestDistanceToEnemy / MAX_RANGE_TO_ENEMY * MAX_ALPHA_VALUE);
	}

	// ���l�ɏ�Z���ăt�F�[�h��\������B
	m_currentAlpha *= m_alphaValueMultiplier;

	// �ŒZ�������������B
	m_shortestDistanceToEnemy = INIT_MIN_DIST_TO_ENEMY;

}

void ScreenEffect::ManageState()
{
	switch (m_enScreenEffectState) {
	case enState_FadeIn:
		// �t�F�[�h�C���B
		FadeIn();
		// ���l�ɏ�Z����l��1.0�𒴂����Ȃ�΁B
		if (m_alphaValueMultiplier >= MAX_ALPHA_VALUE) {
			// �l���Œ肷��B
			m_enScreenEffectState = enState_Fix;
		}
		break;
	case enState_Fix:
		// �ǐՏ�Ԃ��I������Ȃ�΁B
		if (m_enemy1->IsChase() == false && m_enemy2->IsChase() == false) {
			// �t�F�[�h�A�E�g������B
			m_enScreenEffectState = enState_FadeOut;
		}
		break;
	case enState_FadeOut:
		// �t�F�[�h�A�E�g�B
		FadeOut();
		// ���l�ɏ�Z����l��0.0�ȉ��ɂȂ����Ȃ�΁B
		if (m_alphaValueMultiplier <= MIN_ALPHA_VALUE) {
			m_enScreenEffectState = enState_Idle;
		}
		break;
	case enState_Idle:
		// �G�l�~�[���ǐՏ�ԂɈڍs�����Ȃ�΁B
		if (m_enemy1->IsChase() || m_enemy2->IsChase()) {
			m_enScreenEffectState = enState_FadeIn;
		}
		break;
	}
}

void ScreenEffect::FadeIn()
{
	m_alphaValueMultiplier += g_gameTime->GetFrameDeltaTime();
}

void ScreenEffect::FadeOut()
{
	m_alphaValueMultiplier -= MULTIPLY_FADE_OUT * g_gameTime->GetFrameDeltaTime();
}

void ScreenEffect::Render(RenderContext& rc)
{
	// ���l����Z�B
	m_screenEffect.SetMulColor({ RGB_VALUE,RGB_VALUE,RGB_VALUE,m_currentAlpha });
	// �`��B
	m_screenEffect.Draw(rc);
}