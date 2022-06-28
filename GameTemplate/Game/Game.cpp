#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"
#include "Enemy.h"
#include "CollectItem.h"

namespace
{

}

bool Game::Start()
{
	
	return true;
}

void Game::Update()
{
	// ステート管理
	ManageState();

	if (m_gameState == enGameState_InGame) {
		m_edgeManagement.Update();
	}

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
	if (m_isGameOver) {
		m_gameState = enGameState_GameOver;
		DeleteInGameObject();
	}
}

void Game::StateTransitionProccesingFromGameOver()
{
	// ゲームオーバー条件を解除する。
	m_isGameOver = false;
	// 輪郭線制御の登録データをクリアする。
	m_edgeManagement.Clear();
	// 今回のスコアがハイスコアより高ければ。
	if (m_score > m_highScore) {
		// ハイスコアを更新。
		m_highScore = m_score;
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_isEndGameOverProcess = true;
	}
	// ゲームオーバー時の処理が終了しているならば
	if (m_isEndGameOverProcess) {
		// ゲームステートをインゲームに変更する。
		m_gameState = enGameState_Title;
		m_isEndGameOverProcess = false;
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
			m_inGameStage->SetPosition(objData.position);
			m_inGameStage->SetEdgeManagement(&m_edgeManagement);
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
			m_bell->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		// エネミー
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->SetPosition(objData.position);
			m_enemy->SetNumber(objData.number);
			m_enemy->SetEdgeManagement(&m_edgeManagement);
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
		return false;
	});
 
	m_collectItem = NewGO<CollectItem>(0, "collectItem");
	m_collectItem->SetEdgeManagement(&m_edgeManagement);
	m_edgeManagement.Init();
}

void Game::DeleteInGameObject()
{
	auto& enemys = FindGOs<Enemy>("enemy");
	int enemySize = enemys.size();
	for (int i = 0; i < enemySize; i++) {
		m_enemy = enemys[i];
		DeleteGO(m_enemy);
	}

	auto& gameCameras = FindGOs<GameCamera>("gameCamera");
	int gameCameraSize = gameCameras.size();
	for (int i = 0; i < gameCameraSize; i++) {
		m_gameCamera = gameCameras[i];
		DeleteGO(m_gameCamera);
	}

	auto& bells = FindGOs<Bell>("bell");
	int bellSize = bells.size();
	for (int i = 0; i < bellSize; i++) {
		m_bell = bells[i];
		DeleteGO(m_bell);
	}

	auto& players = FindGOs<Player>("player");
	int playerSize = players.size();
	for (int i = 0; i < playerSize; i++) {
		m_player = players[i];
		DeleteGO(m_player);
	}

	auto& inGameStages = FindGOs<BackGround>("backGround");
	int stageSize = inGameStages.size();
	for (int i = 0; i < stageSize; i++) {
		m_inGameStage = inGameStages[i];
		DeleteGO(m_inGameStage);
	}

}

