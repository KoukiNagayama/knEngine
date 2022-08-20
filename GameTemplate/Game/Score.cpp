#include "stdafx.h"
#include "Score.h"
#include "Game.h"

namespace
{
	const Vector2 NUMBER_SPRITE_SIZE = { 65.0f,104.0f };					// �����̉摜�T�C�Y
	const Vector3 SCORE_NUMBER_DEF_POSITION = { 700.0f,450.0f,0.0f };		// �X�R�A��3���ڂ̐����̍��W
	const Vector3 HIGHSCORE_NUMBER_DEF_POSITION = { 700.0f,345.0f,0.0f };	// �n�C�X�R�A��3���ڐ����̍��W
	const Vector3 NUMBER_ADD_POSITION = { 60.0f,0.0f,0.0f };				// �����̈ړ��ʁi�����Ԋu�j
	const Vector3 SCORE_SPRITE_POSITION = { 550.0f,445.0f,0.0f };			// SCORE�̃X�v���C�g�̍��W
	const Vector3 HIGHSCORE_SPRITE_POSITION = { 480.0f,330.0f,0.0f };		// HIGHSCORE�̃X�v���C�g�̍��W
	const Vector2 SCORE_AND_HIGHSCORE_SPRITE_SIZE = { 512.0f,512.0f };		// SCORE��HIGHSCORE�̃X�v���C�g�̉摜�T�C�Y
	const Vector3 SCORE_AND_HIGHSCORE_SPRITE_SCALE = {1.0f,1.0f,1.0f};		// SCORE��HIGHSCORE�̃X�v���C�g�̊g�嗦
	const int INIT_DIGIT_VALUE = 1;											// ����������
	const int INIT_NUMBER_VALUE = 0;										// ���߂鐔��������
	const int DIGIT_EXTRACTION_VALUE = 10;									// ���𒊏o����l
}

Score::Score()
{
	// �Q�[���N���X�̃I�u�W�F�N�g���������B
	m_game = FindGO<Game>("game");

	// �v�f�����v�Z
	m_numberNum = sizeof(m_scoreNumberSprite) / sizeof(SpriteRender);

	// ���W�ݒ�
	Vector3 scoreNumber_pos = SCORE_NUMBER_DEF_POSITION;
	Vector3 highScoreNumber_pos = HIGHSCORE_NUMBER_DEF_POSITION;

	for (int i = 0; i < m_numberNum; i++) {
		// �����ƂɃX�v���C�g�̍��W��ݒ�B
		m_scoreNumberSprite[i].SetPosition(scoreNumber_pos);
		m_highScoreNumberSprite[i].SetPosition(highScoreNumber_pos);

		// ���W�����炷�B
		scoreNumber_pos += NUMBER_ADD_POSITION;
		highScoreNumber_pos += NUMBER_ADD_POSITION;

		// �X�v���C�g���X�V�B
		m_scoreNumberSprite[i].Update();
		m_highScoreNumberSprite[i].Update();
	}

	// SCORE�̃X�v���C�g�̏������B
	m_scoreSprite.Init(
		"Assets/sprite/SCORE.DDS",
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.x,
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.y
	);
	// SCORE�̃X�v���C�g�̍��W��ݒ�B
	m_scoreSprite.SetPosition(SCORE_SPRITE_POSITION);
	// SCORE�̃X�v���C�g�̊g�嗦��ݒ�B
	m_scoreSprite.SetScale(SCORE_AND_HIGHSCORE_SPRITE_SCALE);
	

	// HIGHSCORE�̃X�v���C�g�̏������B
	m_highScoreSprite.Init(
		"Assets/sprite/HIGHSCORE.DDS",
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.x,
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.y
	);
	// HIGHSCORE�̃X�v���C�g�̍��W��ݒ�B
	m_highScoreSprite.SetPosition(HIGHSCORE_SPRITE_POSITION);
	// HIGHSCORE�̃X�v���C�g�̊g�嗦��ݒ�B
	m_highScoreSprite.SetScale(SCORE_AND_HIGHSCORE_SPRITE_SCALE);

	// �X�v���C�g���X�V����B
	m_scoreSprite.Update();
	m_highScoreSprite.Update();
}

void Score::Update()
{
	// ���݂̃X�R�A���擾
	m_score = m_game->GetScore();
	m_highScore = m_game->GetHighScore();

	// ���݂̃X�R�A���L�^���ꂽ�X�R�A�ƈ�v���Ă��Ȃ��Ȃ�΁B
	if (m_score != m_lastScore) {
		// �\������X�R�A���X�V����B
		ScoreUpdate();
	}

	// ���݂̃n�C�X�R�A���L�^���ꂽ�n�C�X�R�A�ƈ�v���Ă��Ȃ��Ȃ�΁B
	if (m_highScore != m_lastHighScore) {
		// �\������n�C�X�R�A���X�V����B
		HighScoreUpdate();
	}

}

void Score::ScoreUpdate()
{
	// int�ɕϊ����Ă���
	int now_score = m_score;

	int digit = INIT_DIGIT_VALUE;	// ���̏�����
	int num = INIT_NUMBER_VALUE;	// ���߂鐔�l�̏�����

	// �����̍X�V
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// �ꌅ�ڂ��珇�ԂɊe���̒l�𒊏o
		num = (now_score / digit) % DIGIT_EXTRACTION_VALUE;
		digit *= DIGIT_EXTRACTION_VALUE;

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

		// �X�R�A���L�^�B
		m_lastScore = m_score;
	}
}

void Score::HighScoreUpdate()
{
	// int�ɕϊ����Ă���
	int now_score = m_highScore;

	int digit = INIT_DIGIT_VALUE;	// ������
	int num = INIT_NUMBER_VALUE;

	// �����̍X�V
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// �ꌅ�ڂ��珇�ԂɊe���̒l�𒊏o
		num = (now_score / digit) % DIGIT_EXTRACTION_VALUE;
		digit *= DIGIT_EXTRACTION_VALUE;

		switch (num)
		{
		case 0:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_0.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 1:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_1.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 2:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_2.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 3:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_3.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 4:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_4.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 5:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_5.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 6:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_6.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 7:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_7.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 8:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_8.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 9:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_9.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		}

		// �X�R�A���L�^�B
		m_lastHighScore = m_highScore;
	}
}

void Score::Render(RenderContext& rc)
{
	// ���݂̃Q�[���X�e�[�g���C���Q�[���Ȃ�΁B
	if (m_game->IsInGame()) {
		// �X�v���C�g��`�悷��B
		m_scoreSprite.Draw(rc);
		m_highScoreSprite.Draw(rc);

		for (int i = 0; i < m_numberNum; i++) {
			m_scoreNumberSprite[i].Draw(rc);
			m_highScoreNumberSprite[i].Draw(rc);
		}
	}
}


