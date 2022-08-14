#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"
#include "Enemy.h"
#include "CollectItem.h"
#include "TitleCamera.h"
#include "TitleText.h"
#include "GameOver.h"
#include "TitleSprite.h"
#include "GameTimeScreen.h"
#include "GameOverEffect.h"
#include "Score.h"
#include "Result.h"
#include "sound/SoundEngine.h"
#include "ScreenEffect.h"
#include "Fade.h"

namespace
{
	const int PLAYABLE_TIME_PER_SEC = 10.0f;					// �b���ƂŐ������Ԃ�3���ɐݒ�B
	const int GAME_END_TIME_PER_SEC = 0.0f;						// �b���ƂŃQ�[�����I�����鎞��
	const int PICK_UP_BELL_NUMBER_TO_REGISTER = 9;				// �x�����擾�������̓o�^�ԍ�
	const int ESCAPE_SOUND_NUMBER_TO_REGISTER = 10;				// �������̉��̓o�^�ԍ�
	const float SOUND_VOLUME_MULTIPLIER = 0.3f;					// �������̉��̏�Z����l
	const float MAX_SOUND_VOLUME = 1.0f;						// �ő剹��
	const float MIN_SOUND_VOLUME = 0.0f;						// �ŏ�����
	const float MAX_SOUND_MUL_VOLUME = 0.7f;					// ���ʂɏ�Z����ő�l
	const float MIN_SOUND_MUL_VOLUME = 0.0f;					// ���ʂɏ�Z����ŏ��l
	const float CLOSE_TO_ENEMY_SOUND_NUMBER_TO_REGISTER = 11;	// �G���߂��Ȃ������̉��̓o�^�ԍ�
	const float CLOSE_TO_ENEMY_SOUND_RANGE = 2000.0f;			// �G���߂��Ȃ������ɒ������鉹�͈̔�
	const int GAME_TIME_SCREEN_PRIORITY = 10;					// �c�莞�ԕ\���I�u�W�F�N�g�̕`��̗D�揇��(�x��)
	const int SCORE_SCREEN_PRIORITY = 11;						// �X�R�A�\���I�u�W�F�N�g�̕`��̗D�揇��(�x��)
	const int GENERAL_PRIORITY = 0;								// �ėp�I�ȃI�u�W�F�N�g�̕`��̗D�揇��(����)
	const int FADE_PRIORITY = 12;								// �t�F�[�h�̃I�u�W�F�N�g�̕`��̗D�揇��(��Ԓx��)
	const float FADE_SPEED_TITLE_TO_IN_GAME = 0.75f;			// �^�C�g������C���Q�[���ɑJ�ڂ���t�F�[�h�̃X�s�[�h
	const float FADE_SPEED_IN_GAME_TO_RESULT = 1.4f;			// �C���Q�[�����烊�U���g�ɑJ�ڂ���t�F�[�h�̃X�s�[�h
	const float FADE_SPEED_RESULT_TO_TITLE = 1.0f;				// ���U���g����^�C�g���ɑJ�ڂ���t�F�[�h�̃X�s�[�h
	const int RESET_SCORE = 0;									// �X�R�A�̃��Z�b�g
	const int RESET_HIGHSCORE = 0;								// �n�C�X�R�A�̃��Z�b�g
}

bool Game::Start()
{
	// �^�C�g�����������B
	StartTitle();
	
	// �g�`�f�[�^��o�^����B
	g_soundEngine->ResistWaveFileBank(PICK_UP_BELL_NUMBER_TO_REGISTER, "Assets/sound/pickup_bell.wav");
	g_soundEngine->ResistWaveFileBank(ESCAPE_SOUND_NUMBER_TO_REGISTER, "Assets/sound/escape_00.wav");
	g_soundEngine->ResistWaveFileBank(CLOSE_TO_ENEMY_SOUND_NUMBER_TO_REGISTER, "Assets/sound/normal_00.wav");

	// �c�莞�Ԃ�\������I�u�W�F�N�g���쐬�i�O�ʂɏo�����ߕ`�揇�Ԓx�߁j
	m_gameTimeScreen = NewGO<GameTimeScreen>(GAME_TIME_SCREEN_PRIORITY, "gameTimeScreen");
	// ����`�悵�Ȃ��悤�ɏ�����
	m_gameTimeScreen->GameTimerUpdate(m_remainingTime);

	// �Q�[���I�[�o�[�G�t�F�N�g���쐬
	m_gameOverEffect = new GameOverEffect;

	// �X�R�A��\������
	m_scoreScreen = NewGO<Score>(SCORE_SCREEN_PRIORITY, "score");
	// �\������X�R�A���X�V�B
	m_scoreScreen->ScoreUpdate();
	m_scoreScreen->HighScoreUpdate();

	return true;
}

void Game::Update()
{
	// �X�e�[�g�Ǘ�
	ManageState();
	
	// �X�e�[�g���Ƃ̏���
	ProcessByState();

	// �������̉����Đ�����B
	PlayEscapeSound();

}

void Game::ManageState()
{
	switch (m_gameState) {
	// �^�C�g��
	case enGameState_Title:
		StateTransitionProccesingFromTitle();
		break;
	// �C���Q�[��
	case enGameState_InGame:
		StateTransitionProccesingFromInGame();
		break;
	// �Q�[���I�[�o�[
	case enGameState_GameOver:
		StateTransitionProccesingFromGameOver();
		break;
	// �Q�[���G���h
	case enGameState_GameEnd:
		StateTransitionProccesingFromGameEnd();
		break;
	default:
		break;
	}
}

void Game::ProcessByState()
{
	switch (m_gameState) {
	case enGameState_Title:
		break;
	case enGameState_InGame:
		// �^�C�}�[��i�߂�B
		GameTimer();
		// �֊s������̏����X�V����B
		m_edgeManagement.Update();
		// �Q�[���I�[�o�[�G�t�F�N�g���Ȃ���s����
		if (m_gameOverEffect->IsPlayEffect()) {
			m_gameOverEffect->PlayGameOverEffect();
		}
		break;
	case enGameState_GameOver:
		// �^�C�}�[��i�߂�B
		GameTimer();
		break;
	case enGameState_GameEnd:
		//GameTimer();
		break;
	}
}

void Game::StateTransitionProccesingFromTitle()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			//DeleteGO(m_fade);
			// �^�C�g���̃I�u�W�F�N�g���폜����B
			DeleteTitleObject();
			m_edgeManagement.Clear();
			// �Q�[���X�e�[�g���C���Q�[���ɕύX����B
			m_gameState = enGameState_InGame;
			// �C���Q�[��������������B
			InitInGame();
			// �t�F�[�h�C�����J�n����B
			m_fade->StartFadeIn();
			m_isWaitFadeout = false;
		}
	}
	else {
		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_isWaitFadeout = true;
			// �t�F�[�h�A�E�g���J�n����B
			m_fade->StartFadeOut();
			m_fade->SetFadeSpeed(FADE_SPEED_TITLE_TO_IN_GAME);
		}
	}
}

void Game::StateTransitionProccesingFromInGame()
{
	if (m_isWaitFadeout) {
		// �t�F�[�h���I����Ă���Ȃ�΁B
		if (!m_fade->IsFade()) {
			//DeleteGO(m_fade);
			// �X�e�[�g���Q�[���G���h�ɕύX����B
			m_gameState = enGameState_GameEnd;
			// �n�C�X�R�A�����߂�B
			if (m_highScore <= m_score) {
				m_highScore = m_score;
			}
			// �Q�[���I�u�W�F�N�g��S���폜����B
			DeleteInGameObject();
			// �^�C�}�[�̕`����~����B
			m_gameTimeScreen->SetDrawFlag(false);
			// ���U���g���쐬�B
			m_result = NewGO<Result>(GENERAL_PRIORITY, "result");
			m_result->Init(m_highScore);

			// �t�F�[�h�C�����J�n����B
			m_fade->StartFadeIn();
			m_isWaitFadeout = false;
		}
	}
	else {
		// �c�莞�Ԃ��Ȃ��Ȃ�΁B
		if (m_remainingTime <= GAME_END_TIME_PER_SEC) {
			m_isWaitFadeout = true;
			// �t�F�[�h�A�E�g���J�n����B
			m_fade->StartFadeOut();
			m_fade->SetFadeSpeed(FADE_SPEED_IN_GAME_TO_RESULT);
		}
	}

	// �Q�[���I�[�o�[�ɂȂ��Ă���΁B
	if (m_isGameOver) {
		m_gameOver = NewGO<GameOver>(GENERAL_PRIORITY, "gameOver");
		// �X�e�[�g���Q�[���I�[�o�[�ɕύX����B
		m_gameState = enGameState_GameOver;
		// �Q�[���I�u�W�F�N�g��S���폜����B
		DeleteInGameObject();
		// �Q�[���I�[�o�[�G�t�F�N�g�̌�n���B
		m_gameOverEffect->EndGameOverEffect();
		m_gameOverEffect->StopHorrorSound();
	}
}

void Game::StateTransitionProccesingFromGameOver()
{
	m_gameOver = FindGO<GameOver>("gameOver");
	// �Q�[���I�[�o�[��������������B
	m_isGameOver = false;
	// �֊s������̓o�^�f�[�^���N���A����B
	m_edgeManagement.Clear();

	// ����̃X�R�A���n�C�X�R�A��荂����΁B
	if (m_score > m_highScore) {
		// �n�C�X�R�A���X�V����B
		m_highScore = m_score;
	}

	// �X�R�A������������B
	m_score = RESET_SCORE;
	
	// �Q�[���I�[�o�[���̏������I�����Ă���Ȃ��
	if (m_gameOver->IsEndProcess()) {
		// �Q�[���X�e�[�g���C���Q�[���ɕύX����B
		m_gameState = enGameState_InGame;
		// �Q�[���I�[�o�[�̃C���X�^���X���폜����B
		DeleteGO(m_gameOver);
		// �C���Q�[��������������B
		InitInGame();
	}	
}

void Game::StateTransitionProccesingFromGameEnd()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			//DeleteGO(m_fade);
			// ���U���g���폜�B
			DeleteGO(m_result);
			// �Q�[���X�e�[�g���^�C�g���ɕύX����B
			m_gameState = enGameState_Title;
			// �^�C�g�������B
			StartTitle();
			// �^�C�}�[�̕`����ĊJ����B
			m_gameTimeScreen->SetDrawFlag(true);
			// �c�莞�Ԃ�n��
			m_gameTimeScreen->GameTimerUpdate(m_remainingTime);
			// ���Z�b�g���Ă���
			m_isResultDisplayFinished = false;
			m_gameTimeScreen->Reset();
			m_score = RESET_SCORE;
			m_highScore = RESET_HIGHSCORE;

			// �t�F�[�h�C�����J�n����B
			m_fade->StartFadeIn();
			m_fade->SetFadeSpeed(FADE_SPEED_RESULT_TO_TITLE);
			m_isWaitFadeout = false;

		}
	}
	else {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
}

void Game::InitInGame()
{
	m_inGameLevel.Init("Assets/level3D/stage2_3.tkl", [&](LevelObjectData& objData) {
		//�X�e�[�W
		if (objData.EqualObjectName(L"stage") == true) {
			// �w�i�N���X
			m_inGameStage = NewGO<BackGround>(GENERAL_PRIORITY, "backGround");
			m_inGameStage->SetPosition(objData.position);
			m_inGameStage->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		//�v���C���[(�J����)
		if (objData.EqualObjectName(L"player") == true) {
			// �v���C���[�N���X
			m_player = NewGO<Player>(GENERAL_PRIORITY, "player");
			m_player->SetPosition(objData.position);
			// �Q�[���J�����N���X
			m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
			m_gameCamera->SetPosition(objData.position);

			m_bell = NewGO<Bell>(GENERAL_PRIORITY, "bell");
			m_bell->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		// �G�l�~�[
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			m_enemy = NewGO<Enemy>(GENERAL_PRIORITY, "enemy");
			m_enemy->SetPosition(objData.position);
			m_enemy->SetNumber(objData.number);
			m_enemy->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		// ���W�A�C�e��
		if (objData.EqualObjectName(L"collectItem") == true)
		{
			auto collectItem = NewGO<CollectItem>(GENERAL_PRIORITY, "collectItem");
			collectItem->SetPosition(objData.position);
			collectItem->SetEdgeManagement(&m_edgeManagement);
			m_collectItem.push_back(collectItem);
			return true;
		}
		return false;
	});
	
	// �֊s��������̏�����
	m_edgeManagement.Init();


	// �Q�[���I�[�o�[�G�t�F�N�g�̏�����
	m_gameOverEffect->Init(m_player, m_enemy, m_gameCamera);
}

void Game::DeleteInGameObject()
{
	// �G�l�~�[�폜
	// �G�l�~�[�̏��������B
	auto& enemys = FindGOs<Enemy>("enemy");
	// �G�l�~�[�̐�
	int enemySize = enemys.size();
	for (int i = 0; i < enemySize; i++) {
		m_enemy = enemys[i];
		// �폜
		DeleteGO(m_enemy);
	}
	// �Q�[���J�����폜
	DeleteGO(m_gameCamera);
	// ���W�A�C�e���폜
	for (auto collectItem : m_collectItem)
	{
		DeleteGO(collectItem);
	}
	m_collectItem.clear();
	// �x���폜
	DeleteGO(m_bell);
	// �v���C���[�폜
	DeleteGO(m_player);
	// �X�e�[�W�폜
	DeleteGO(m_inGameStage);

}

void Game::DeleteTitleObject()
{
	// �^�C�g���J�����폜
	DeleteGO(m_titleCamera);
	// �^�C�g���e�L�X�g�폜
	DeleteGO(m_titleText);
	// �X�e�[�W�폜
	DeleteGO(m_inGameStage);
	// �^�C�g���ɂ���X�v���C�g���폜
	DeleteGO(m_titleSprite);
}

void Game::GameTimer()
{
	// �c�莞�Ԃ��v�Z�B
	m_remainingTime -= g_gameTime->GetFrameDeltaTime();
	
	// �c�莞�Ԃ�n��
	m_gameTimeScreen->GameTimerUpdate(m_remainingTime);
}

void Game::StartGameOverEffect() 
{
	// �Q�[���I�[�o�[�G�t�F�N�g���J�n
	m_gameOverEffect->StartGameOverEffect();
}

void Game::StartTitle() {

	// ���Ԃ�ݒ�B
	m_remainingTime = PLAYABLE_TIME_PER_SEC;

	// �e�I�u�W�F�N�g�𐶐�����B
	// �^�C�g���̃��x�����������B
	m_titleLevel.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
		{
			// �^�C�g�����f��
			if (objData.EqualObjectName(L"titleText") == true) {
				m_titleText = NewGO<TitleText>(GENERAL_PRIORITY, "titleText");
				m_titleText->SetPosition(objData.position);
				m_titleText->SetEdgeManagement(&m_edgeManagement);
				return true;
			}
			// �^�C�g���p�J����
			if (objData.EqualObjectName(L"titleCamera") == true) {
				m_titleCamera = NewGO<TitleCamera>(GENERAL_PRIORITY, "titleCamera");
				m_titleCamera->SetPosition(objData.position);
				return true;
			}
			// �w�i
			if (objData.EqualObjectName(L"stage2") == true) {
				m_inGameStage = NewGO<BackGround>(GENERAL_PRIORITY, "backGround");
				m_inGameStage->SetPosition(objData.position);
				m_inGameStage->SetEdgeManagement(&m_edgeManagement);
				return true;
			}
		});
	// �^�C�g�����ɕ\������X�v���C�g�𐶐�����B
	m_titleSprite = NewGO<TitleSprite>(GENERAL_PRIORITY, "titleSprite");

	// �֊s����������������B
	m_edgeManagement.Init();

	// �t�F�[�h�̃I�u�W�F�N�g�����ɐ�������Ă���Ȃ�΍폜����B
	if (m_fade == nullptr) {
		m_fade = NewGO<Fade>(FADE_PRIORITY, "fade");
	}

	// �t�F�[�h�C�����J�n
	m_fade->StartFadeIn();
}

void Game::PlayEscapeSound()
{
	// �����Đ����邩
	bool isPlaySound = false;

	// �G�l�~�[������
	const auto& enemys = FindGOs<Enemy>("enemy");
	// �G�l�~�[�̐�
	const int size = enemys.size();

	// �C���Q�[���̎���
	if (m_gameState == enGameState_InGame) {
		for (int i = 0; i < size; i++) {
			m_enemy = enemys[i];
			// �Đ�����悤�ɐݒ肳��Ă���Ȃ�΁B
			if (m_enemy->IsChase()) {
				isPlaySound = true;
				break;
			}
		}
	}

	// �������Ă��Ȃ����ɖ�悤�ɐݒ肳�ꂽ�Ȃ�΁B
	if (isPlaySound && m_escapeSound == nullptr) {
		m_escapeSound = NewGO<SoundSource>(0);
		m_escapeSound->Init(ESCAPE_SOUND_NUMBER_TO_REGISTER);
		EscapeSoundVolumeControl(isPlaySound);
		m_escapeSound->Play(true);
	}

	// ���͖��Ă��邪�A���ʂ��������Ƃ��B
	if (isPlaySound && m_escapeSoundVolume < MAX_SOUND_VOLUME) {
		// �t�F�[�h�C��������B
		EscapeSoundVolumeControl(true);
	}
	// �������Ă��鎞�ɖ�Ȃ��悤�ɐݒ肳�ꂽ�Ȃ�΁B
	else if (isPlaySound == false && m_escapeSound != nullptr) {
		// �t�F�[�h�A�E�g������B
		EscapeSoundVolumeControl(false);
		if (m_escapeSoundVolume <= MIN_SOUND_VOLUME) {
			// �����폜�B
			DeleteGO(m_escapeSound);
			m_escapeSound = nullptr;
			// ���ʂ����Z�b�g�B
			m_escapeSoundVolume = MAX_SOUND_VOLUME;
		}
	}

}

void Game::EscapeSoundVolumeControl(bool fadeIn)
{

	if (fadeIn) {
		// �t�F�[�h�C���B
		m_escapeSoundVolume += SOUND_VOLUME_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
		// ����l�𒴂���Ȃ�΁B
		if (m_escapeSoundVolume > MAX_SOUND_VOLUME) {
			// ���l���Œ肷��B
			m_escapeSoundVolume = MAX_SOUND_VOLUME;
		}
	}
	else {
		// �t�F�[�h�A�E�g�B
		m_escapeSoundVolume -= SOUND_VOLUME_MULTIPLIER * g_gameTime->GetFrameDeltaTime();

	}	
	// ���ʂ�ݒ肷��B
	m_escapeSound->SetVolume(m_escapeSoundVolume);
}


