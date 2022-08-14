#include "stdafx.h"
#include "Result.h"
#include "Fade.h"

namespace
{
	const float	  RESULT_SPRITE_W = 1920.0f;								// �w�i�X�v���C�g�̉����B
	const float   RESULT_SPRITE_H = 1080.0f;								// �w�i�X�v���C�g�̏c���B

	const Vector2 NUMBER_SPRITE_SIZE = { 160.0f,280.0f };					// �����̉摜�T�C�Y
	const Vector3 SCORE_NUMBER_DEF_POSITION = { -220.0f,60.0f,0.0f };		// �X�R�A��3���ڂ̐����̍��W
	const Vector3 NUMBER_ADD_POSITION = { 200.0f,0.0f,0.0f };				// �����̈ړ��ʁi�����Ԋu�j
}

Result::Result() {

}

Result::~Result() {

}

void Result::Init(const int score) {

	// �X�R�A�̕ۑ�
	m_score = score;

	// �X�v���C�g�̏�����
	m_resultSprite_BG.Init(
		"Assets/sprite/GameClear_New.DDS",
		RESULT_SPRITE_W,
		RESULT_SPRITE_H
	);

	// �v�f�����v�Z
	m_numberNum = sizeof(m_scoreNumberSprite) / sizeof(SpriteRender);

	// �摜��ݒ�
	ScoreUpdate();

	// ���W�ݒ�
	Vector3 scoreNumber_pos = SCORE_NUMBER_DEF_POSITION;

	for (int i = 0; i < m_numberNum; i++) {
		// �����ƂɃX�v���C�g�̍��W��ݒ�B
		m_scoreNumberSprite[i].SetPosition(scoreNumber_pos);

		// ���W�����炷�B
		scoreNumber_pos += NUMBER_ADD_POSITION;

		// �X�v���C�g���X�V�B
		m_scoreNumberSprite[i].Update();
	}

	// �ݒ芮��
	m_initFlag = true;
}

void Result::Render(RenderContext& rc) {

	if (m_initFlag == false) {
		return;
	}

	m_resultSprite_BG.Draw(rc);

	for (int i = 0; i < m_numberNum; i++) {
		m_scoreNumberSprite[i].Draw(rc);
	}

}

void Result::ScoreUpdate() {

	int digit = 1;	// ������
	int num = 0;

	// �����̍X�V
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// �ꌅ�ڂ��珇�ԂɊe���̒l�𒊏o
		num = (m_score / digit) % 10;
		digit *= 10;

		switch (num)
		{
		case 0:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_0.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 1:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_1.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 2:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_2.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 3:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_3.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 4:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_4.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 5:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_5.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 6:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_6.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 7:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_7.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 8:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_8.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 9:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_9.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		}
	}

}