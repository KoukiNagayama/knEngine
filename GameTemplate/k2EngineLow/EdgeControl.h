#pragma once
namespace nsK2EngineLow
{
	const int MAXIMUM_SIZE_OF_RECORDABLE_SOUND = 50;	// �o�^�\�ȉ����̍ő��
	/// <summary>
	/// �����f�[�^�\����
	/// </summary>
	struct SoundSourceData
	{
		Vector3		s_position = Vector3::Zero;		// �����̍��W
		float		s_range = 0.0f;					// �e���͈�
		float		s_rateByTime = 0.0f;			// ���Ԍo�߂ɂ��e����
		float		s_pad1[3];
		int			s_isSound = 0;					// ���������Ă��邩
		int			s_pad2[3];
		
	};
	/// <summary>
	/// �����f�[�^�̒萔�o�b�t�@�p�\����
	/// </summary>
	struct CBSoundSourceData
	{
		SoundSourceData		s_soundSourceData[MAXIMUM_SIZE_OF_RECORDABLE_SOUND];	// �����f�[�^�z��
		int					s_numSoundSource = 0;									// �o�^���ꂽ�����̌�
		float				s_sizeOfColorValue = 0.0f;
	};

	/// <summary>
	/// �����ɂ��֊s������N���X
	/// </summary>
	class EdgeControl
	{
	public:
		EdgeControl() {};
		~EdgeControl() {};
		
		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="number">�o�^����ԍ�</param>
		/// <param name="position">���W</param>
		/// <param name="range">�e���͈�</param>
		/// <param name="rateByTime">���Ԍo�߂ɂ��e����</param>
		void Init(int number,
			Vector3 position,
			float range,
			float rateByTime
		);
		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();
		/// <summary>
		///	�o�^���ꂽ���̃N���A
		/// </summary>
		void Clear();
		/// <summary>
		/// �����̍��W��ݒ�B
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="position">�����̍��W</param>
		void SetPosition(int number, Vector3 position)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_position = position;
		}
		/// <summary>
		/// �����̉e���͈͂�ݒ�B
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="range">�����̉e���͈�</param>
		void SetRange(int number, float range)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_range = range;
		}
		/// <summary>
		/// ���������Ă��邩�ǂ�����ݒ�
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="isSound">���������Ă��邩</param>
		void SetIsSound(int number, int isSound)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_isSound = isSound;
		}
		/// <summary>
		/// �����̉e����
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="rate">�e����</param>
		void SetRate(int number, float rate)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_rateByTime = rate;
		}
		/// <summary>
		/// �����f�[�^�p�萔�o�b�t�@�\���̂��擾
		/// </summary>
		/// <returns>�����f�[�^�p�萔�o�b�t�@�\����</returns>
		CBSoundSourceData& GetSoundSourceData()
		{
			return m_cbSoundSourceData;
		}
	private:
		enum SizeChangeState {
			enSizeChange_ValueIncrease,	// �l�㏸	
			enSizeChange_ValueDecrease	// �l����
		};
		CBSoundSourceData	m_cbSoundSourceData;								// �����f�[�^�̒萔�o�b�t�@�p�\����
		float				m_sizeOfColorValue = 0.0f;							// �F�̐��l�̑傫��
		SizeChangeState		m_sizeChangeState = enSizeChange_ValueIncrease;		// ���l�ύX�X�e�[�g
	};
}


