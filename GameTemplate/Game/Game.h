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

class Game : public IGameObject
{
public:
	Game() {};
	~Game() {};
	/// <summary>
	/// 開始処理
	/// </summary>
	/// <returns>初期化が終了していればtrue。</returns>
	bool Start();
	/// <summary>
	/// 更新処理
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
	/// ゲームオーバーしているか。
	/// </summary>
	/// <returns>ゲームオーバーになっていればtrue。そうでなければfalse。</returns>
	bool IsGameOverNow()
	{
		return m_gameState == enGameState_GameOver;
	}
	/// <summary>
	/// ゲームオーバーとする。
	/// </summary>
	void SetGameOver()
	{
		m_isGameOver = true;
	}
	/// <summary>
	/// ゲームオーバーエフェクト開始。
	/// プレイヤーの移動やカメラの回転を禁止する。
	/// </summary>
	void StartGameOverEffect();
	/// <summary>
	/// スコアを増やす。
	/// </summary>
	/// <param name="score">スコアの増値</param>
	void AddScore(int score)
	{
		m_score += score;
	}
	/// <summary>
	/// スコアを取得。
	/// </summary>
	/// <returns>スコア。</returns>
	int GetScore()
	{
		return m_score;
	}
	/// <summary>
	/// ハイスコアを取得。
	/// </summary>
	/// <returns>ハイスコア。</returns>
	int GetHighScore()
	{
		return m_highScore;
	}
private:
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// ステートごとの処理
	/// </summary>
	void ProcessByState();
	/// <summary>
	/// タイトルからのステート遷移処理
	/// </summary>
	void StateTransitionProccesingFromTitle();
	/// <summary>
	/// インゲームからのステート遷移処理
	/// </summary>
	void StateTransitionProccesingFromInGame();
	/// <summary>
	/// ゲームエンドからのステート遷移処理
	/// </summary>
	void StateTransitionProccesingFromGameEnd();
	/// <summary>
	/// ゲームオーバーからのステート遷移処理
	/// </summary>
	void StateTransitionProccesingFromGameOver();
	/// <summary>
	/// タイトル部分を作成
	/// </summary>
	void StartTitle();
	/// <summary>
	/// インゲームを初期化
	/// </summary>
	void InitInGame();
	/// <summary>
	/// インゲームのオブジェクトを削除
	/// </summary>
	void DeleteInGameObject();
	/// <summary>
	/// タイトルのオブジェクトを削除
	/// </summary>
	void DeleteTitleObject();
	/// <summary>
	/// ゲームの残り時間の処理
	/// </summary>
	void GameTimer();
	/// <summary>
	/// 逃走の音を流す。
	/// </summary>
	void PlayEscapeSound();
	/// <summary>
	/// 逃走時の音量コントロール
	/// </summary>
	void EscapeSoundVolumeControl(bool fadeIn);
	/// <summary>
	/// エネミーが近いときの音の音量コントロール
	/// </summary>
	void CloseToEnemySoundVolumeControl();
	/// <summary>
	/// エネミーが近いときの音をフェードさせる。
	/// </summary>
	void FadeCloseToEnemySound();
private:
	enum EnGameState
	{
		enGameState_Title,			// タイトル
		enGameState_InGame,			// インゲーム
		enGameState_GameOver,		// ゲームオーバー
		enGameState_GameEnd			// ゲームエンド
	};	

	EnGameState					m_gameState = enGameState_Title;		// ゲームステート
	bool						m_isResultDisplayFinished = false;		// リザルトの表示が終了しているか
	bool						m_isEndGameOverProcess = false;			// ゲームオーバー時の処理が終了しているか
	LevelRender					m_inGameLevel;							// インゲームのレベル
	Player*						m_player = nullptr;						// プレイヤークラス
	GameCamera*					m_gameCamera = nullptr;					// ゲームカメラクラス
	BackGround*					m_inGameStage = nullptr;				// 背景クラス
	Bell*						m_bell = nullptr;						// ベルクラス
	Enemy*						m_enemy = nullptr;						// エネミークラス
	EdgeManagement				m_edgeManagement;						// 輪郭線制御
	bool						m_isGameOver = false;					// ゲームオーバーになっているか
	int							m_score = 0;							// プレイヤーのスコア
	int							m_highScore = 0;						// プレイヤーのハイスコア
	std::vector<CollectItem*>	m_collectItem;							// 収集アイテム					
	float						m_remainingTime = 0.0f;					// 残り時間
	LevelRender					m_titleLevel;							// タイトルのレベル
	TitleCamera*				m_titleCamera = nullptr;				// タイトルカメラクラス
	TitleText*					m_titleText = nullptr;					// タイトルテキストクラス
	GameOver*					m_gameOver = nullptr;					// ゲームオーバークラス
	TitleSprite*				m_titleSprite = nullptr;				// タイトルスプライトクラス
	GameTimeScreen*				m_gameTimeScreen = nullptr;				// 残り時間表示クラス
	GameOverEffect*				m_gameOverEffect = nullptr;				// ゲームオーバーエフェクト
	Score*						m_scoreScreen = nullptr;				// スコア表示クラス
	Result*						m_result = nullptr;						// リザルトクラス
	SoundSource*				m_escapeSound = nullptr;				// 逃走時の音
	float						m_escapeSoundVolume = 1.0f;				// 逃走時の音量
	SoundSource*				m_closeToEnemySound = nullptr;
	float						m_closeToEnemySoundVolume = 0.0f;
	float						m_distanceToNearestEnemy = 0.0f;	
	float						m_closeToEnemySoundMulVolume = 1.0f;
	bool						m_isFadeInCloseToEnemySound = false;
	bool						m_isFadeOutCloseToEnemySound = false;
	ScreenEffect*				m_screenEffect = nullptr;
};

