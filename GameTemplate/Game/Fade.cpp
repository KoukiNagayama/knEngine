#include "stdafx.h"
#include "Fade.h"

namespace
{
	//const Vector3	SCALE = Vector3(1.436f, 1.436f, 1.0f);
	//const Vector3	POSITION = Vector3(-90.0f, 7.75, 0.0f);

	const float MAX_ALPHA_VALUE = 1.0f;				// α値の最大
	const float MIN_ALPHA_VALUE = 0.0f;				// α値の最小
	const float MUL_RGB_VALUE = 1.0f;				// 乗算するRGBの要素の数値
	const float SPRITE_W = 1920.0f;					// スプライトの横幅
	const float SPRITE_H = 1080.0f;					// スプライトの縦幅
}

Fade::Fade()
{
}


Fade::~Fade()
{
}

bool Fade::Start()
{
	m_fadeSprite.Init("Assets/sprite/fade.DDS", SPRITE_W, SPRITE_H);
	m_fadeSprite.Update();
	return true;
}

void Fade::Update()
{
	switch (m_state) {
		// フェードイン。
	case enState_FadeIn:
		// α値を減少させる。
		m_currentAlpha -= m_fadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= MIN_ALPHA_VALUE) {
			m_currentAlpha = MIN_ALPHA_VALUE;
			m_state = enState_Idle;
		}
		break;
		// フェードアウト。
	case enState_FadeOut:
		// α値を増加させる。
		m_currentAlpha += m_fadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= MAX_ALPHA_VALUE) {
			m_currentAlpha = MAX_ALPHA_VALUE;
			m_state = enState_Idle;
		}
		break;
		// 待機。
	case enState_Idle:

		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	if (m_currentAlpha > MIN_ALPHA_VALUE) {
		m_fadeSprite.SetMulColor({ MUL_RGB_VALUE, MUL_RGB_VALUE, MUL_RGB_VALUE, m_currentAlpha });
		m_fadeSprite.Draw(rc);
	}
}