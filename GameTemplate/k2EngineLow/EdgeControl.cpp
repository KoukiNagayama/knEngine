#include "k2EngineLowPreCompile.h"
#include "EdgeControl.h"

namespace
{

}

namespace nsK2EngineLow
{

	//const float AMOUNT_OF_COLOR_CHANGE_PER_FRAME = 0.01f;	// フレームごとの色の変化量

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
		// 値が上昇する
		if (m_sizeChangeState == enSizeChange_ValueIncrease) {
			//m_sizeOfColorValue += AMOUNT_OF_COLOR_CHANGE_PER_FRAME;
			// 色の値の大きさが指定の値よりも大きくなったならば
			if (m_sizeOfColorValue >= 1.0f) {
				// 値を減少させる
				m_sizeChangeState = enSizeChange_ValueDecrease;
			}
		}
		// 値が減少する
		else if (m_sizeChangeState == enSizeChange_ValueDecrease) {
			//m_sizeOfColorValue -= AMOUNT_OF_COLOR_CHANGE_PER_FRAME;
			// 色の値の大きさが指定の値よりも小さくなったならば
			if (m_sizeOfColorValue <= 0.0f) {
				// 値を上昇させる
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
