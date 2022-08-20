#include "k2EngineLowPreCompile.h"
#include "EdgeControl.h"

namespace
{

}

namespace nsK2EngineLow
{

	//const float AMOUNT_OF_COLOR_CHANGE_PER_FRAME = 0.01f;	// �t���[�����Ƃ̐F�̕ω���

	void EdgeControl::Init(int number,
		Vector3 position,
		float range,
		float rateByTime
	)
	{
		m_cbSoundSourceData.s_soundSourceData[number].s_position = position;
		m_cbSoundSourceData.s_soundSourceData[number].s_range = range;
		m_cbSoundSourceData.s_soundSourceData[number].s_rateByTime = rateByTime;
		m_cbSoundSourceData.s_numSoundSource += 1;
	}

	void EdgeControl::Update()
	{
		// �l���㏸����
		if (m_sizeChangeState == enSizeChange_ValueIncrease) {
			//m_sizeOfColorValue += AMOUNT_OF_COLOR_CHANGE_PER_FRAME;
			// �F�̒l�̑傫�����w��̒l�����傫���Ȃ����Ȃ��
			if (m_sizeOfColorValue >= 1.0f) {
				// �l������������
				m_sizeChangeState = enSizeChange_ValueDecrease;
			}
		}
		// �l����������
		else if (m_sizeChangeState == enSizeChange_ValueDecrease) {
			//m_sizeOfColorValue -= AMOUNT_OF_COLOR_CHANGE_PER_FRAME;
			// �F�̒l�̑傫�����w��̒l�����������Ȃ����Ȃ��
			if (m_sizeOfColorValue <= 0.0f) {
				// �l���㏸������
				m_sizeChangeState = enSizeChange_ValueIncrease;
			}
		}
		m_cbSoundSourceData.s_sizeOfColorValue = m_sizeOfColorValue;
	}

	void EdgeControl::Clear()
	{
		for (int i = 0; i < m_cbSoundSourceData.s_numSoundSource; i++) {
			m_cbSoundSourceData.s_soundSourceData[i].s_position = Vector3::Zero;
			m_cbSoundSourceData.s_soundSourceData[i].s_range = 0.0f;
			m_cbSoundSourceData.s_soundSourceData[i].s_rateByTime = 0.0f;
			m_cbSoundSourceData.s_soundSourceData[i].s_isSound = 0;
		}
		m_cbSoundSourceData.s_numSoundSource = 0;

	}
}
