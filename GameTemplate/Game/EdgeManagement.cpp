#include "stdafx.h"
#include "EdgeManagement.h"
#include "Bell.h"
#include "Player.h"
#include "Enemy.h"
#include "TitleCamera.h"
#include "Game.h"
namespace
{
	const float BELL_RANGE = 800.0f;						// �x�����֊s���ɉe����^����͈�
	const float PLAYER_FOOTSTEP_RANGE = 300.0f;				// �v���C���[�̑������֊s���ɉe����^����͈�
	const float ENEMY_FOOTSTEP_RANGE = 450.0f;				// �G�l�~�[�̑������֊s���ɉe����^����͈�
	const float ENEMY_SCREAM_RANGE = 1300.0f;				// �G�l�~�[�̙��K���֊s���ɉe����^����͈�
	const float TITLE_RANGE = 800.0f;						// �^�C�g���ŗ֊s���ɉe����^����͈�
	const float TITLE_OUTLINE_RATE = 1.0f;					// �^�C�g���ł̗֊s���̉e����
	const float NO_TITLE_OUTLINE_RATE = 0.0f;				// �^�C�g���ł̗֊s���̉e�����Ȃ�
	const int IS_SOUND = 1;									// �������Ă���
	const int IS_NOT_SOUND = 0;								// �������Ă��Ȃ�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;			// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.0065f;		// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;				// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;				// ���Ԃɂ��e�����̍ŏ��l
}

void EdgeManagement::Init()
{
	m_game = FindGO<Game>("game");
	// �x���̃C���X�^���X�������B
	m_bell = FindGO<Bell>("bell");
	// �v���C���[�̃C���X�^���X�������B
	m_player = FindGO<Player>("player");

	// �G�l�~�[������
	const auto& enemys = FindGOs<Enemy>("enemy");
	// �G�l�~�[�̐�
	const int size = enemys.size();
	
	for (int i = 0; i < size; i++) {
		switch (i) {
		case 0:
			m_enemy1 = enemys[i];
			break;
		case 1:
			m_enemy2 = enemys[i];
			break;
		case 2:
			m_enemy3 = enemys[i];
			break;
		}
	}
	

	// �x���̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_Bell,
		Vector3::Zero,
		BELL_RANGE,
		m_rateByTimeOfBell
	);
	m_edgeControl.SetIsSound(enSoundSourceData_Bell, IS_NOT_SOUND);


	// �v���C���[�̑����̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_PlayerFootstep,
		Vector3::Zero,
		PLAYER_FOOTSTEP_RANGE,
		m_rateByTimeOfPlayerFootstep
	);
	m_edgeControl.SetIsSound(enSoundSourceData_PlayerFootstep, IS_NOT_SOUND);


	// �G�l�~�[1�̑����̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep1,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep1, IS_NOT_SOUND);


	// �G�l�~�[1�̙��K�̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream1,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream1, IS_NOT_SOUND);

	// �^�C�g���̎��́B
	if (m_game->IsTitle()) {
		// �^�C�g���J�����̏����擾����B
		m_titleCamera = FindGO<TitleCamera>("titleCamera");
		// �֊s���`��̒��S���^�C�g���J�����̍��W�Ƃ���B
		m_positionOfCenterInTitle = m_titleCamera->GetPosition();
	}

	// �^�C�g���̎��̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_Title,
		m_positionOfCenterInTitle,
		TITLE_RANGE,
		TITLE_OUTLINE_RATE
	);
	m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_NOT_SOUND);

	// �G�l�~�[2�̑����̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep2,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep2
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep2, IS_NOT_SOUND);

	// �G�l�~�[2�̙��K�̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream2,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream2
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream2, IS_NOT_SOUND);

	// �G�l�~�[3�̑����̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep3,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep3
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep3, IS_NOT_SOUND);

	// �G�l�~�[3�̙��K�̗֊s���ւ̉e�����������B
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream3,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream3
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream3, IS_NOT_SOUND);

}

void EdgeManagement::Update()
{
	if (m_bell == nullptr || m_player == nullptr || m_enemy1 == nullptr || m_enemy2 == nullptr) {
		return;
	}

	// �������Ă��邩���w��B
	SpecifyIsSound();

	if (m_game->IsInGame() == false) {
		return;
	}

	// ���W���w��B
	SpecifyPosition();

	// ���Ԃɂ��e�������w��B
	SpecifyRateByTime();
}

void EdgeManagement::Clear()
{
	// �o�^���̃N���A�B
	m_edgeControl.Clear();
}

void EdgeManagement::SpecifyPosition()
{
	// �x���B
	m_edgeControl.SetPosition(enSoundSourceData_Bell, m_bell->GetPosition());

	// �v���C���[�̑����B
	m_edgeControl.SetPosition(enSoundSourceData_PlayerFootstep, m_player->GetPosition());

	// �G�l�~�[1�̑����B
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep1, m_enemy1->GetPosition());

	// �G�l�~�[1�̙��K�B
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream1, m_enemy1->GetPosition());

	// �G�l�~�[2�̑����B
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep2, m_enemy2->GetPosition());
	
	// �G�l�~�[2�̙��K�B
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream2, m_enemy2->GetPosition());

	// �G�l�~�[3�̑����B
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep3, m_enemy3->GetPosition());

	// �G�l�~�[3�̙��K�B
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream3, m_enemy3->GetPosition());
}

void EdgeManagement::SpecifyIsSound()
{
	// �^�C�g���B
	if (m_game->IsTitle()) {
		m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_SOUND);
		return;
	}

	m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_NOT_SOUND);
	
	// �x���B
	SpecifyIsBellSounding();

	// �v���C���[�̑����B
	SpecifyIsPlayerFootstepSounding();

	// �G�l�~�[1�̑����B
	SpecifyIsEnemyFootstep1Sounding();

	// �G�l�~�[1�̙��K�B
	SpecifyIsEnemyScream1Sounding();

	// �G�l�~�[2�̑����B
	SpecifyIsEnemyFootstep2Sounding();

	// �G�l�~�[2�̙��K�B
	SpecifyIsEnemyScream2Sounding();

	// �G�l�~�[3�̑����B
	SpecifyIsEnemyFootstep3Sounding();

	// �G�l�~�[3�̙��K�B
	SpecifyIsEnemyScream3Sounding();
}

void EdgeManagement::SpecifyIsBellSounding()
{
	// ���Ă��鎞�B
	if (m_bell->IsBellRinging() == true && m_isLastBellSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_Bell, IS_SOUND);
		m_isLastBellSound = true;
	}
	// ���Ă��Ȃ����B
	else if (m_bell->IsBellRinging() == false && m_isLastBellSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_Bell, IS_NOT_SOUND);
	}
}

void EdgeManagement::SpecifyIsPlayerFootstepSounding()
{
	// ���Ă��鎞�B
	if (m_player->IsPlayerFootstepSound() == true && m_isLastPlayerFootstepSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_PlayerFootstep, IS_SOUND);
		m_isLastPlayerFootstepSound = true;
	}
	// ���Ă��Ȃ����B
	else if (m_player->IsPlayerFootstepSound() == false && m_isLastPlayerFootstepSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_PlayerFootstep, IS_NOT_SOUND);
		m_isLastPlayerFootstepSound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyFootstep1Sounding()
{
	// ���Ă��鎞�B
	if (m_enemy1->IsMoving() == true && m_isLastEnemyFootstepSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep1, IS_SOUND);
		m_isLastEnemyFootstepSound = true;
	}
	// ���Ă��Ȃ����B
	else if (m_enemy1->IsMoving() == false && m_isLastEnemyFootstepSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep1, IS_NOT_SOUND);
		m_isLastEnemyFootstepSound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScream1Sounding()
{
	// ���Ă��鎞�B
	if (m_enemy1->IsScream() == true && m_isLastEnemyScream == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream1, IS_SOUND);
		m_isLastEnemyScream = true;
	}
	// ���Ă��Ȃ����B
	else if (m_enemy1->IsScream() == false && m_isLastEnemyScream == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream1, IS_NOT_SOUND);
		m_isLastEnemyScream = false;
	}
}

void EdgeManagement::SpecifyIsEnemyFootstep2Sounding()
{
	// ���Ă��鎞�B
	if (m_enemy2->IsMoving() == true && m_isLastEnemyFootstep2Sound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep2, IS_SOUND);
		m_isLastEnemyFootstep2Sound = true;
	}
	// ���Ă��Ȃ����B
	else if (m_enemy2->IsMoving() == false && m_isLastEnemyFootstep2Sound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep2, IS_NOT_SOUND);
		m_isLastEnemyFootstep2Sound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScream2Sounding()
{
	// ���Ă��鎞�B
	if (m_enemy2->IsScream() == true && m_isLastEnemyScream2 == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream2, IS_SOUND);
		m_isLastEnemyScream2 = true;
	}
	// ���Ă��Ȃ����B
	else if (m_enemy2->IsScream() == false && m_isLastEnemyScream2 == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream2, IS_NOT_SOUND);
		m_isLastEnemyScream2 = false;
	}
}

void EdgeManagement::SpecifyIsEnemyFootstep3Sounding()
{
	// ���Ă��鎞�B
	if (m_enemy3->IsMoving() == true && m_isLastEnemyFootstep3Sound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep3, IS_SOUND);
		m_isLastEnemyFootstep3Sound = true;
	}
	// ���Ă��Ȃ����B
	else if (m_enemy3->IsMoving() == false && m_isLastEnemyFootstep3Sound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep3, IS_NOT_SOUND);
		m_isLastEnemyFootstep3Sound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScream3Sounding()
{
	// ���Ă��鎞�B
	if (m_enemy3->IsScream() == true && m_isLastEnemyScream3 == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream3, IS_SOUND);
		m_isLastEnemyScream3 = true;
	}
	// ���Ă��Ȃ����B
	else if (m_enemy3->IsScream() == false && m_isLastEnemyScream3 == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream3, IS_NOT_SOUND);
		m_isLastEnemyScream3 = false;
	}
}

void EdgeManagement::SpecifyRateByTime()
{
	// �x���B
	CalcRateOfBell();
	// �v���C���[�̑����B
	CalcRateOfPlayerFootstep();
	// �G�l�~�[1�̑����B
	CalcRateOfEnemyFootstep1();
	// �G�l�~�[1�̙��K�B
	CalcRateOfEnemyScream1();
	// �G�l�~�[2�̑����B
	CalcRateOfEnemyFootstep2();
	// �G�l�~�[2�̙��K�B
	CalcRateOfEnemyScream2();
	// �G�l�~�[3�̑����B
	CalcRateOfEnemyFootstep3();
	// �G�l�~�[3�̙��K�B
	CalcRateOfEnemyScream3();
	// �^�C�g���B
	m_edgeControl.SetRate(enSoundSourceData_Title, NO_TITLE_OUTLINE_RATE);
}

void EdgeManagement::CalcRateOfBell()
{
	// �x�������Ă���Ȃ�΁B
	if (m_bell->IsBellRinging()) {
		// �x���̉e�������グ��悤�ɂ���B
		m_isRateUpOfBell = true;
	}
	// �x���̉e�������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfBell) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfBell < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfBell += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfBell = false;
		}
	}
	// �x���̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfBell > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfBell -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfBell < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfBell = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �x���̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_Bell, m_rateByTimeOfBell);
}

void EdgeManagement::CalcRateOfPlayerFootstep()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_player->IsPlayerFootstepSound()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfPlayerFootstep = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfPlayerFootstep) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfPlayerFootstep < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfPlayerFootstep += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfPlayerFootstep = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfPlayerFootstep > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfPlayerFootstep -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfPlayerFootstep < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfPlayerFootstep = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_PlayerFootstep, m_rateByTimeOfPlayerFootstep);
}

void EdgeManagement::CalcRateOfEnemyFootstep1()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_enemy1->IsMoving()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfEnemyFootstep = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfEnemyFootstep) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfEnemyFootstep < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfEnemyFootstep += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfEnemyFootstep = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfEnemyFootstep > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfEnemyFootstep -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyFootstep < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyFootstep = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep1, m_rateByTimeOfEnemyFootstep);
}

void EdgeManagement::CalcRateOfEnemyScream1()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_enemy1->IsScream()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfEnemyScream = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfEnemyScream) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfEnemyScream < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfEnemyScream += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfEnemyScream = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfEnemyScream > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfEnemyScream -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyScream < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyScream = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream1, m_rateByTimeOfEnemyScream);
}

void EdgeManagement::CalcRateOfEnemyFootstep2()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_enemy2->IsMoving()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfEnemyFootstep2 = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfEnemyFootstep2) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfEnemyFootstep2 < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfEnemyFootstep2 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfEnemyFootstep2 = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfEnemyFootstep2 > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfEnemyFootstep2 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyFootstep2 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyFootstep2 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep2, m_rateByTimeOfEnemyFootstep2);
}

void EdgeManagement::CalcRateOfEnemyScream2()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_enemy2->IsScream()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfEnemyScream2 = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfEnemyScream2) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfEnemyScream2 < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfEnemyScream2 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfEnemyScream2 = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfEnemyScream2 > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfEnemyScream2 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyScream2 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyScream2 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream2, m_rateByTimeOfEnemyScream2);
}

void EdgeManagement::CalcRateOfEnemyFootstep3()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_enemy3->IsMoving()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfEnemyFootstep3 = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfEnemyFootstep3) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfEnemyFootstep3 < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfEnemyFootstep3 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfEnemyFootstep3 = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfEnemyFootstep3 > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfEnemyFootstep3 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyFootstep3 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyFootstep3 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep3, m_rateByTimeOfEnemyFootstep3);
}

void EdgeManagement::CalcRateOfEnemyScream3()
{
	// �v���C���[�̑��������Ă���Ȃ�΁B
	if (m_enemy3->IsScream()) {
		// �v���C���[�̑����̉e�������グ��悤�ɂ���B
		m_isRateUpOfEnemyScream3 = true;
	}
	// �v���C���[�̑������グ��悤�ɂȂ��Ă���Ȃ�΁B
	if (m_isRateUpOfEnemyScream3) {
		// �e�������ő�l�����ł���΁B
		if (m_rateByTimeOfEnemyScream3 < RATE_BY_TIME_MAX_VALUE) {
			// �e�������グ��B
			m_rateByTimeOfEnemyScream3 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// �e�������ő�l�𒴂��Ă���Ȃ�΁B
		else {
			// �e�������グ�Ȃ��悤�ɂ���B
			m_isRateUpOfEnemyScream3 = false;
		}
	}
	// �v���C���[�̑����̉e������0���傫���Ȃ��
	else if (m_rateByTimeOfEnemyScream3 > RATE_BY_TIME_MIN_VALUE) {
		// �e������������B
		m_rateByTimeOfEnemyScream3 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyScream3 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyScream3 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// �v���C���[�̑����̉e������ݒ�B
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream3, m_rateByTimeOfEnemyScream3);
}

