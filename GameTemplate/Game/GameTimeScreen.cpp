#include "stdafx.h"
#include "GameTimeScreen.h"

namespace
{
	const Vector2 NUMBER_SPRITE_SIZE = { 100.0f,160.0f };				// �����̉摜�T�C�Y
	const Vector3 NUMBER_DEF_POSITION = { -860.0f,430.0f,0.0f };		// 3���ڂ̐����̍��W
	const Vector3 NUMBER_ADD_POSITION = { 120.0f,0.0f,0.0f };			// �����̈ړ��ʁi�����Ԋu�j

	const int COLOR_CHANGE_BORDER = 30;									// �c�莞�Ԃ����̒l�ȉ��ɂȂ�����F�ύX
	const Vector4 COLOR_CHANGE = { 1.0f,0.0f,0.0f,1.0f };				// �c�莞�Ԃ����Ȃ����̐F
	const Vector4 COLOR_RESET = { 1.0f,1.0f,1.0f,1.0f };				// �c�莞�Ԃ̐F�����Z�b�g����Ƃ��̐F
}

GameTimeScreen::GameTimeScreen() {

	// �v�f�����v�Z
	m_numberNum = sizeof(m_numberSprite) / sizeof(SpriteRender);

	// ���W�ݒ�
	Vector3 number_pos = NUMBER_DEF_POSITION;
	for (int i = 0; i < m_numberNum; i++) {

		m_numberSprite[i].SetPosition(number_pos);
		number_pos += NUMBER_ADD_POSITION;

		m_numberSprite[i].Update();
	}
}

GameTimeScreen::~GameTimeScreen() {
}

void GameTimeScreen::GameTimerUpdate(const float time) {

	// int�ɕϊ����Ă���
	int now_time = (int)time;

	// �X�V�̕K�v���Ȃ��Ȃ璆�f
	if (m_oldTime == now_time) {
		return;
	}

	// ���݂̎��Ԃ�ۑ����Ă���
	m_oldTime = now_time;

	// ������
	int digit = 1;
	int num = 0;

	// �����̍X�V
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// �ꌅ�ڂ��珇�ԂɊe���̒l�𒊏o
		num = (now_time / digit) % 10;
		digit *= 10;

		switch (num)
		{
		case 0:
			m_numberSprite[i].Init("Assets/sprite/Number_0.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 1:
			m_numberSprite[i].Init("Assets/sprite/Number_1.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 2:
			m_numberSprite[i].Init("Assets/sprite/Number_2.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 3:
			m_numberSprite[i].Init("Assets/sprite/Number_3.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 4:
			m_numberSprite[i].Init("Assets/sprite/Number_4.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 5:
			m_numberSprite[i].Init("Assets/sprite/Number_5.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 6:
			m_numberSprite[i].Init("Assets/sprite/Number_6.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 7:
			m_numberSprite[i].Init("Assets/sprite/Number_7.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 8:
			m_numberSprite[i].Init("Assets/sprite/Number_8.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 9:
			m_numberSprite[i].Init("Assets/sprite/Number_9.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		}
	}

	// �F�̍X�V
	// ���Ԃ����܂�Ȃ��Ƃ������ԂɁB
	if (m_colorChangeFlag == false && now_time <= COLOR_CHANGE_BORDER) {
		m_colorChangeFlag = true;
		// �F��ԂɕύX����B
		for (int i = 0; i < m_numberNum; i++) {
			m_numberSprite[i].SetMulColor(COLOR_CHANGE);
		}
	}
	// ���Ԃɗ]�T������Ƃ��ԁ����ɁB
	else if(m_colorChangeFlag == true && now_time > COLOR_CHANGE_BORDER) {
		m_colorChangeFlag = false;
		// �F�𔒂ɕύX����B
		for (int i = 0; i < m_numberNum; i++) {
			m_numberSprite[i].SetMulColor(COLOR_RESET);
		}
	}
}

void GameTimeScreen::Render(RenderContext& rc) {

	if (m_drawFlag == false) {
		return;
	}

	for (int i = 0; i < m_numberNum; i++) {
		m_numberSprite[i].Draw(rc);
	}
}

