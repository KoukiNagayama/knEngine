#pragma once

#include "LevelRender.h"
#include "EdgeManagement.h"
#include "sound/SoundSource.h"

class Player;
class GameCamera;
class BackGround;
class Bell;
class Enemy;
class CollectItem;
class TitleCamera;
class TitleText;
class GameOver;
class TitleSprite;
class GameTimeScreen;
class GameOverEffect;
class Score;
class Result;
class ScreenEffect;
class Fade;

class Game : public IGameObject
{
public:
	Game() {};
	~Game() {};
	/// <summary>
	/// �J�n����
	/// </summary>
	/// <returns>���������I�����Ă����true�B</returns>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	bool IsTitle()
	{
		return m_gameState == enGameState_Title;
	}
	bool IsInGame()
	{
		return m_gameState == enGameState_InGame;
	}
	/// <summary>
	/// �Q�[���I�[�o�[���Ă��邩�B
	/// </summary>
	/// <returns>�Q�[���I�[�o�[�ɂȂ��Ă����true�B�����łȂ����false�B</returns>
	bool IsGameOverNow()
	{
		return m_gameState == enGameState_GameOver;
	}
	/// <summary>
	/// �Q�[���I�[�o�[�Ƃ���B
	/// </summary>
	void SetGameOver()
	{
		m_isGameOver = true;
	}
	/// <summary>
	/// �Q�[���I�[�o�[�G�t�F�N�g�J�n�B
	/// �v���C���[�̈ړ���J�����̉�]���֎~����B
	/// </summary>
	void StartGameOverEffect();
	/// <summary>
	/// �X�R�A�𑝂₷�B
	/// </summary>
	/// <param name="score">�X�R�A�̑��l</param>
	void AddScore(int score)
	{
		m_score += score;
	}
	/// <summary>
	/// �X�R�A���擾�B
	/// </summary>
	/// <returns>�X�R�A�B</returns>
	int GetScore()
	{
		return m_score;
	}
	/// <summary>
	/// �n�C�X�R�A���擾�B
	/// </summary>
	/// <returns>�n�C�X�R�A�B</returns>
	int GetHighScore()
	{
		return m_highScore;
	}
private:
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �X�e�[�g���Ƃ̏���
	/// </summary>
	void ProcessByState();
	/// <summary>
	/// �^�C�g������̃X�e�[�g�J�ڏ���
	/// </summary>
	void StateTransitionProccesingFromTitle();
	/// <summary>
	/// �C���Q�[������̃X�e�[�g�J�ڏ���
	/// </summary>
	void StateTransitionProccesingFromInGame();
	/// <summary>
	/// �Q�[���G���h����̃X�e�[�g�J�ڏ���
	/// </summary>
	void StateTransitionProccesingFromGameEnd();
	/// <summary>
	/// �Q�[���I�[�o�[����̃X�e�[�g�J�ڏ���
	/// </summary>
	void StateTransitionProccesingFromGameOver();
	/// <summary>
	/// �^�C�g���������쐬
	/// </summary>
	void StartTitle();
	/// <summary>
	/// �C���Q�[����������
	/// </summary>
	void InitInGame();
	/// <summary>
	/// �C���Q�[���̃I�u�W�F�N�g���폜
	/// </summary>
	void DeleteInGameObject();
	/// <summary>
	/// �^�C�g���̃I�u�W�F�N�g���폜
	/// </summary>
	void DeleteTitleObject();
	/// <summary>
	/// �Q�[���̎c�莞�Ԃ̏���
	/// </summary>
	void GameTimer();
	/// <summary>
	/// �����̉��𗬂��B
	/// </summary>
	void PlayEscapeSound();
	/// <summary>
	/// �������̉��ʃR���g���[��
	/// </summary>
	void EscapeSoundVolumeControl(bool fadeIn);
private:
	enum EnGameState
	{
		enGameState_Title,			// �^�C�g��
		enGameState_InGame,			// �C���Q�[��
		enGameState_GameOver,		// �Q�[���I�[�o�[
		enGameState_GameEnd			// �Q�[���G���h
	};	

	EnGameState					m_gameState = enGameState_Title;		// �Q�[���X�e�[�g
	bool						m_isResultDisplayFinished = false;		// ���U���g�̕\�����I�����Ă��邩
	bool						m_isEndGameOverProcess = false;			// �Q�[���I�[�o�[���̏������I�����Ă��邩
	LevelRender					m_inGameLevel;							// �C���Q�[���̃��x��
	Player*						m_player = nullptr;						// �v���C���[�N���X
	GameCamera*					m_gameCamera = nullptr;					// �Q�[���J�����N���X
	BackGround*					m_inGameStage = nullptr;				// �w�i�N���X
	Bell*						m_bell = nullptr;						// �x���N���X
	Enemy*						m_enemy = nullptr;						// �G�l�~�[�N���X
	EdgeManagement				m_edgeManagement;						// �֊s������
	bool						m_isGameOver = false;					// �Q�[���I�[�o�[�ɂȂ��Ă��邩
	int							m_score = 0;							// �v���C���[�̃X�R�A
	int							m_highScore = 0;						// �v���C���[�̃n�C�X�R�A
	std::vector<CollectItem*>	m_collectItem;							// ���W�A�C�e��					
	float						m_remainingTime = 0.0f;					// �c�莞��
	LevelRender					m_titleLevel;							// �^�C�g���̃��x��
	TitleCamera*				m_titleCamera = nullptr;				// �^�C�g���J�����N���X
	TitleText*					m_titleText = nullptr;					// �^�C�g���e�L�X�g�N���X
	GameOver*					m_gameOver = nullptr;					// �Q�[���I�[�o�[�N���X
	TitleSprite*				m_titleSprite = nullptr;				// �^�C�g���X�v���C�g�N���X
	GameTimeScreen*				m_gameTimeScreen = nullptr;				// �c�莞�ԕ\���N���X
	GameOverEffect*				m_gameOverEffect = nullptr;				// �Q�[���I�[�o�[�G�t�F�N�g
	Score*						m_scoreScreen = nullptr;				// �X�R�A�\���N���X
	Result*						m_result = nullptr;						// ���U���g�N���X
	SoundSource*				m_escapeSound = nullptr;				// �������̉�
	float						m_escapeSoundVolume = 1.0f;				// �������̉���
	Fade*						m_fade = nullptr;						// �t�F�[�h
	bool						m_isWaitFadeout = false;				// �t�F�[�h�A�E�g�܂ő҂��Ă����Ԃ�
};

