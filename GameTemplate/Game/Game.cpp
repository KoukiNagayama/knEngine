#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"

bool Game::Start()
{

	return true;
}

void Game::Update()
{
	// ステート管理
	ManageState();

}

void Game::ManageState()
{
	switch (m_gameState) {
	// タイトル
	case enGameState_Title:
		StateTransitionProccesingFromTitle();
		break;
	// インゲーム
	case enGameState_InGame:
		StateTransitionProccesingFromInGame();
		break;
	// ゲームオーバー
	case enGameState_GameOver:
		StateTransitionProccesingFromGameOver();
		break;
	// ゲームエンド
	case enGameState_GameEnd:
		StateTransitionProccesingFromGameEnd();
		break;
	default:
		break;
	}
}

void Game::StateTransitionProccesingFromTitle()
{
	// ゲームステートをインゲームに変更する。
	m_gameState = enGameState_InGame;
	// インゲームを初期化する。
	InitInGame();
}

void Game::StateTransitionProccesingFromInGame()
{

}

void Game::StateTransitionProccesingFromGameOver()
{
	// ゲームオーバー時の処理が終了しているならば
	if (m_isEndGameOverProcess) {
		// ゲームステートをインゲームに変更する。
		m_gameState = enGameState_InGame;
	}
}

void Game::StateTransitionProccesingFromGameEnd()
{
	// リザルトの表示が終了しているならば
	if (m_isResultDisplayFinished) {
		// ゲームステートをタイトルに変更する。
		m_gameState = enGameState_Title;
	}
}

void Game::InitInGame()
{
	m_inGameLevel.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData) {
		//ステージ
		if (objData.EqualObjectName(L"floor2") == true) {
			// 背景クラス
			m_inGameStage = NewGO<BackGround>(0, "backGround");
			return true;
		}
		//プレイヤー(カメラ)
		if (objData.EqualObjectName(L"player") == true) {
			// プレイヤークラス
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objData.position);
			// ゲームカメラクラス
			m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
			m_gameCamera->SetPosition(objData.position);

			m_bell = NewGO<Bell>(0, "bell");
			return true;
		}
		// エネミー
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			return true;
		}
		//つみき
		if (objData.EqualObjectName(L"crashtoy") == true) {
			return true;
		}
		//蓄音機
		if (objData.ForwardMatchName(L"gramophone") == true)
		{
			return true;
		}

		//レコード
		if (objData.ForwardMatchName(L"record") == true)
		{
			return true;
		}
		if (objData.ForwardMatchName(L"door") == true)
		{
			return true;
		}
		if (objData.ForwardMatchName(L"switchfloor") == true)
		{
			return true;
		}
		if (objData.ForwardMatchName(L"gimmick") == true)
		{
			return true;
		}
		if (objData.EqualObjectName(L"crowbar") == true)
		{
			return true;
		}
	});


}
