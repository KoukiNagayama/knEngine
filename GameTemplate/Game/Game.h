#pragma once

#include "LevelRender.h"
#include "EdgeManagement.h"

class Player;
class GameCamera;
class BackGround;
class Bell;
class Enemy;
class CollectItem;

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
	/// <summary>
	/// ゲームクリアしているか。
	/// </summary>
	/// <returns>ゲームクリアになっていればtrue。そうでなければfalse。</returns>
	bool IsGameClearNow()
	{
		return m_gameState == enGameState_GameClear;
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
	/// スコアを増やす。
	/// </summary>
	/// <param name="score">スコアの増値</param>
	void AddScore(int score)
	{
		m_score += score;
	}
private:
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
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
	/// インゲームを初期化
	/// </summary>
	void InitInGame();
	/// <summary>
	/// インゲームのオブジェクトを削除
	/// </summary>
	void DeleteInGameObject();
private:
	enum EnGameState
	{
		enGameState_Title,			// タイトル
		enGameState_InGame,			// インゲーム
		enGameState_GameClear,		// ゲームクリア
		enGameState_GameOver,		// ゲームオーバー
		enGameState_GameEnd			// ゲームエンド
	};	

	EnGameState			m_gameState = enGameState_Title;		// ゲームステート
	bool				m_isResultDisplayFinished = false;		// リザルトの表示が終了しているか
	bool				m_isEndGameOverProcess = false;			// ゲームオーバー時の処理が終了しているか
	LevelRender			m_inGameLevel;							// インゲームのレベル
	Player*				m_player = nullptr;						// プレイヤークラス
	GameCamera*			m_gameCamera = nullptr;					// ゲームカメラクラス
	BackGround*			m_inGameStage = nullptr;				// 背景クラス
	Bell*				m_bell = nullptr;						// ベルクラス
	Enemy*				m_enemy = nullptr;						// エネミークラス
	EdgeManagement		m_edgeManagement;						// 輪郭線制御
	bool				m_isGameOver = false;					// ゲームオーバーになっているか
	int					m_score = 0;							// プレイヤーのスコア
	int					m_highScore = 0;						// プレイヤーのハイスコア
	CollectItem*		m_collectItem = nullptr;				// 収集アイテム					
};

