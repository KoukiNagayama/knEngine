#pragma once

#include "LevelRender.h"

class Player;
class GameCamera;
class BackGround;
class Bell;
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
};

