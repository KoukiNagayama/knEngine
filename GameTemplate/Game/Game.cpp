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

namespace
{
	const int SETTING_TIME_3_MIN_PER_FRAME = 180.0f;	// フレームごとで制限時間を3分に設定。
	const int SETTING_TIME_6_MIN_PER_FRAME = 360.0f;	// フレームごとで制限時間を5分に設定。	
	const int GAME_END_TIME_PER_FRAME = 0.0f;			// フレームごとでゲームが終了する時間
}

bool Game::Start()
{
	StartTitle();

	// 残り時間を表示するオブジェクトを作成（前面に出すため描画順番遅め）
	m_gameTimeScreen = NewGO<GameTimeScreen>(10, "gameTimeScreen");
	// 無を描画しないように初期化
	m_gameTimeScreen->GameTimerUpdate(m_remainingTime);

	// ゲームオーバーエフェクトを作成
	m_gameOverEffect = new GameOverEffect;

	m_scoreScreen = NewGO<Score>(11, "score");

	m_scoreScreen->ScoreUpdate();
	m_scoreScreen->HighScoreUpdate();

	return true;
}

void Game::Update()
{
	// ステート管理
	ManageState();

	if (m_gameState == enGameState_InGame) {
		// 残り時間を計算。
		GameTimer();
		// 輪郭線制御情報を管理。
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
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// タイトルのオブジェクトを削除する。
		DeleteTitleObject();
		m_edgeManagement.Clear();
		// ゲームステートをインゲームに変更する。
		m_gameState = enGameState_InGame;
		// インゲームを初期化する。
		InitInGame();


	}
}

void Game::StateTransitionProccesingFromInGame()
{
	// ゲームオーバーエフェクト中なら実行処理
	if (m_gameOverEffect->IsPlayEffect()) {
		m_gameOverEffect->PlayGameOverEffect();
	}

	// 残り時間が無いならば。
	if (m_remainingTime <= GAME_END_TIME_PER_FRAME) {
		// ステートをゲームエンドに変更する。
		m_gameState = enGameState_GameEnd;
		// ハイスコアを求める。
		if (m_highScore <= m_score) {
			m_highScore = m_score;
		}
		// ゲームオブジェクトを全部削除する。
		DeleteInGameObject();
		// タイマーの描画を停止する。
		m_gameTimeScreen->SetDrawFlag(false);
		// リザルトを作成。
		m_result = NewGO<Result>(0, "result");
		m_result->Init(m_highScore);
	}

	// ゲームオーバーになっていれば。
	if (m_isGameOver) {
		m_gameOver = NewGO<GameOver>(0, "gameOver");
		// ステートをゲームオーバーに変更する。
		m_gameState = enGameState_GameOver;
		// ゲームオブジェクトを全部削除する。
		DeleteInGameObject();
	}
}

void Game::StateTransitionProccesingFromGameOver()
{
	m_gameOver = FindGO<GameOver>("gameOver");
	// ゲームオーバー条件を解除する。
	m_isGameOver = false;
	// 輪郭線制御の登録データをクリアする。
	m_edgeManagement.Clear();

	// 今回のスコアがハイスコアより高ければ。
	if (m_score > m_highScore) {
		// ハイスコアを更新する。
		m_highScore = m_score;
	}

	// スコアを初期化する。
	m_score = 0;
	
	// ゲームオーバー時の処理が終了しているならば
	if (m_gameOver->IsEndProcess()) {
		// ゲームステートをインゲームに変更する。
		m_gameState = enGameState_InGame;
		// ゲームオーバーのインスタンスを削除する。
		DeleteGO(m_gameOver);
		// インゲームを初期化する。
		InitInGame();
	}	
}

void Game::StateTransitionProccesingFromGameEnd()
{
	m_edgeManagement.Clear();

	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_isResultDisplayFinished = true;
	}

	// リザルトの表示が終了しているならば
	if (m_isResultDisplayFinished) {
		// リザルトを削除。
		DeleteGO(m_result);
		// ゲームステートをタイトルに変更する。
		m_gameState = enGameState_Title;
		// タイトル準備。
		StartTitle();
		// タイマーの描画を再開する。
		m_gameTimeScreen->SetDrawFlag(true);
		// 残り時間を渡す
		m_gameTimeScreen->GameTimerUpdate(m_remainingTime);
		// リセットしておく
		m_isResultDisplayFinished = false;
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

	// ゲームオーバーエフェクトの初期化
	m_gameOverEffect->Init(m_player, m_enemy, m_gameCamera);

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

	auto& collectItems = FindGOs<CollectItem>("collectItem");
	int collectItemSize = collectItems.size();
	for (int i = 0; i < collectItemSize; i++) {
		m_collectItem = collectItems[i];
		DeleteGO(m_collectItem);
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

void Game::DeleteTitleObject()
{
	auto& titleCameras = FindGOs<TitleCamera>("titleCamera");
	int titleCameraSize = titleCameras.size();
	for (int i = 0; i < titleCameraSize; i++) {
		m_titleCamera = titleCameras[i];
		DeleteGO(m_titleCamera);
	}

	auto& titleTexts = FindGOs<TitleText>("titleText");
	int titleTextSize = titleTexts.size();
	for (int i = 0; i < titleTextSize; i++) {
		m_titleText = titleTexts[i];
		DeleteGO(m_titleText);
	}

	auto& inGameStages = FindGOs<BackGround>("backGround");
	int stageSize = inGameStages.size();
	for (int i = 0; i < stageSize; i++) {
		m_inGameStage = inGameStages[i];
		DeleteGO(m_inGameStage);
	}
	
	DeleteGO(m_titleSprite);
}

void Game::GameTimer()
{

	// 残り時間を計算。
	m_remainingTime -= g_gameTime->GetFrameDeltaTime();
	
	// 残り時間を渡す
	m_gameTimeScreen->GameTimerUpdate(m_remainingTime);

}

void Game::StartGameOverEffect() {
	m_gameOverEffect->StartGameOverEffect();
}

void Game::StartTitle() {

	// 時間を設定。
	m_remainingTime = SETTING_TIME_3_MIN_PER_FRAME;

	m_titleLevel.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
		{
			// タイトルモデル
			if (objData.EqualObjectName(L"titleText") == true) {
				m_titleText = NewGO<TitleText>(0, "titleText");
				m_titleText->SetPosition(objData.position);
				m_titleText->SetEdgeManagement(&m_edgeManagement);
				return true;
			}
			// タイトル用カメラ
			if (objData.EqualObjectName(L"titleCamera") == true) {
				m_titleCamera = NewGO<TitleCamera>(0, "titleCamera");
				m_titleCamera->SetPosition(objData.position);
				return true;
			}
			// 背景
			if (objData.EqualObjectName(L"stage2") == true) {
				m_inGameStage = NewGO<BackGround>(0, "backGround");
				m_inGameStage->SetPosition(objData.position);
				m_inGameStage->SetEdgeManagement(&m_edgeManagement);
				return true;
			}
		});
	m_titleSprite = NewGO<TitleSprite>(0, "titleSprite");

	// 輪郭線情報を初期化。
	m_edgeManagement.Init();
}
