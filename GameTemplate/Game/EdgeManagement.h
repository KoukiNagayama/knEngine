#pragma once
class Bell;
class Player;
class Enemy;
class TitleCamera;
class Game;
class EdgeManagement
{
public:
	EdgeManagement() {};
	~EdgeManagement() {};

	/// <summary>
	/// �����������B
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �N���A�B
	/// </summary>
	void Clear();
	/// <summary>
	/// �֊s������̏����擾�B
	/// </summary>
	/// <returns>�֊s������̏��</returns>
	EdgeControl* GetEdgeControl()
	{
		return &m_edgeControl;
	}
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="number">�����̔ԍ��B</param>
	/// <param name="position">�ݒ肷����W�B</param>
	void SetPosition(int number, Vector3 position) 
	{
		m_edgeControl.SetPosition(number, position);
	}
	/// <summary>
	/// ���������Ă��邩�ǂ�����ݒ�B
	/// </summary>
	/// <param name="number">�����̔ԍ��B</param>
	/// <param name="isSound">���������Ă��邩�B1�͖��Ă���B0�͖��Ă��Ȃ��B</param>
	void SetIsSound(int number, int isSound)
	{
		m_edgeControl.SetIsSound(number, isSound);
	}
	/// <summary>
	/// ���Ԍo�߂ɂ��e������ݒ�B
	/// </summary>
	/// <param name="number">�����̔ԍ��B</param>
	/// <param name="rateByTime">���Ԍo�߂ɂ��e�����B</param>
	void SetRateByTime(int number, float rateByTime)
	{
		m_edgeControl.SetRate(number, rateByTime);
	}
private:
	/// <summary>
	/// ���W���w��B
	/// </summary>
	void SpecifyPosition();
	/// <summary>
	/// ���Ԃɂ��e�������w��B
	/// </summary>
	void SpecifyRateByTime();
	/// <summary>
	/// �������Ă��邩���w��B
	/// </summary>
	void SpecifyIsSound();
	/// <summary>
	/// �x���̉e�������v�Z�B
	/// </summary>
	void CalcRateOfBell();
	/// <summary>
	/// �v���C���[�̑����̉e�������v�Z�B
	/// </summary>
	void CalcRateOfPlayerFootstep();
	/// <summary>
	/// �G�l�~�[1�̑����̉e�������v�Z�B
	/// </summary>
	void CalcRateOfEnemyFootstep1();
	/// <summary>
	/// �G�l�~�[1�̙��K�̉e�������v�Z�B
	/// </summary>
	void CalcRateOfEnemyScream1();
	/// <summary>
	/// �G�l�~�[2�̑����̉e�������v�Z�B
	/// </summary>
	void CalcRateOfEnemyFootstep2();
	/// <summary>
	/// �G�l�~�[2�̙��K�̉e�������v�Z�B
	/// </summary>
	void CalcRateOfEnemyScream2();
	/// <summary>
	/// �G�l�~�[3�̑����̉e�������v�Z�B
	/// </summary>
	void CalcRateOfEnemyFootstep3();
	/// <summary>
	/// �G�l�~�[3�̙��K�̉e�������v�Z�B
	/// </summary>
	void CalcRateOfEnemyScream3();
	/// <summary>
	/// �x���̉������Ă��邩�w��B
	/// </summary>
	void SpecifyIsBellSounding();
	/// <summary>
	/// �v���C���[�̑��������Ă��邩�w��B
	/// </summary>
	void SpecifyIsPlayerFootstepSounding();
	/// <summary>
	/// �G�l�~�[1�̑��������Ă��邩�w��B
	/// </summary>
	void SpecifyIsEnemyFootstep1Sounding();
	/// <summary>
	/// �G�l�~�[1�̙��K�����Ă��邩�w��B
	/// </summary>
	void SpecifyIsEnemyScream1Sounding();
	/// <summary>
	/// �G�l�~�[2�̑��������Ă��邩�w��B
	/// </summary>
	void SpecifyIsEnemyFootstep2Sounding();
	/// <summary>
	/// �G�l�~�[2�̙��K�����Ă��邩�w��B
	/// </summary>
	void SpecifyIsEnemyScream2Sounding();
	/// <summary>
	/// �G�l�~�[3�̑��������Ă��邩�w��B
	/// </summary>
	void SpecifyIsEnemyFootstep3Sounding();
	/// <summary>
	/// �G�l�~�[2�̙��K�����Ă��邩�w��B
	/// </summary>
	void SpecifyIsEnemyScream3Sounding();
private:
	enum EnSoundSourceData
	{
		enSoundSourceData_Bell,				// �x��
		enSoundSourceData_PlayerFootstep,	// �v���C���[�̑���
		enSoundSourceData_EnemyFootstep1,	// �G�l�~�[1�̑���
		enSoundSourceData_EnemyScream1,		// �G�l�~�[1�̙��K
		enSoundSourceData_Title,			// �^�C�g��
		enSoundSourceData_EnemyFootstep2,	// �G�l�~�[2�̑���
		enSoundSourceData_EnemyScream2,		// �G�l�~�[2�̙��K
		enSoundSourceData_EnemyFootstep3,	// �G�l�~�[3�̑���
		enSoundSourceData_EnemyScream3,		// �G�l�~�[3�̙��K
		enSoundSourceData_Num				// ���̌�
	};


	EdgeControl				m_edgeControl;											// �֊s������
	Bell*					m_bell = nullptr;										// �x��
	Player*					m_player = nullptr;										// �v���C���[
	Enemy*					m_enemy1 = nullptr;										// �G�l�~�[1
	Enemy*					m_enemy2 = nullptr;										// �G�l�~�[2
	Enemy*					m_enemy3 = nullptr;										// �G�l�~�[3
	TitleCamera*			m_titleCamera = nullptr;								// �^�C�g���J����
	float					m_rateByTimeOfBell = 0.0f;								// �x���̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfBell = false;								// �x���̉e�������グ�邩
	bool					m_isLastBellSound = false;								// �x���͒��O�͖��Ă����� 
	float					m_rateByTimeOfPlayerFootstep = 0.0f;					// �v���C���[�̑����̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfPlayerFootstep = false;						// �v���C���[�̑����̉e�������グ�邩
	bool					m_isLastPlayerFootstepSound = false;					// �v���C���[�̑����͒��O�͖��Ă�����
	float					m_rateByTimeOfEnemyFootstep = 0.0f;						// �G�l�~�[�̑����̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfEnemyFootstep = false;						// �G�l�~�[�̑����̉e�������グ�邩
	bool					m_isLastEnemyFootstepSound = false;						// �G�l�~�[�̑����͒��O�͖��Ă�����
	float					m_rateByTimeOfEnemyScream = 0.0f;						// �G�l�~�[�̙��K�̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfEnemyScream = false;						// �G�l�~�[�̙��K�̉e�������グ�邩
	bool					m_isLastEnemyScream = false;							// �G�l�~�[�̙��K�͒��O�͖��Ă�����
	Vector3					m_positionOfCenterInTitle = Vector3::Zero;				// �^�C�g���̎��̒��S�̍��W
	Game*					m_game = nullptr;										// �Q�[��
	float					m_rateByTimeOfBellAfterTitle = 1.0f;					// �^�C�g����̃x���̎��Ԍo�߂ɂ��e����
	float					m_rateByTimeOfEnemyFootstep2 = 0.0f;					// �G�l�~�[2�̑����̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfEnemyFootstep2 = false;						// �G�l�~�[2�̑����̉e�������グ�邩
	bool					m_isLastEnemyFootstep2Sound = false;					// �G�l�~�[2�̑����͒��O�͖��Ă�����
	float					m_rateByTimeOfEnemyScream2 = 0.0f;						// �G�l�~�[2�̙��K�̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfEnemyScream2 = false;						// �G�l�~�[2�̙��K�̉e�������グ�邩
	bool					m_isLastEnemyScream2 = false;							// �G�l�~�[2�̙��K�͒��O�͖��Ă�����
	float					m_rateByTimeOfEnemyFootstep3 = 0.0f;					// �G�l�~�[3�̑����̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfEnemyFootstep3 = false;						// �G�l�~�[3�̑����̉e�������グ�邩
	bool					m_isLastEnemyFootstep3Sound = false;					// �G�l�~�[3�̑����͒��O�͖��Ă�����
	float					m_rateByTimeOfEnemyScream3 = 0.0f;						// �G�l�~�[2�̙��K�̎��Ԍo�߂ɂ��e����
	bool					m_isRateUpOfEnemyScream3 = false;						// �G�l�~�[2�̙��K�̉e�������グ�邩
	bool					m_isLastEnemyScream3 = false;							// �G�l�~�[2�̙��K�͒��O�͖��Ă�����
};

