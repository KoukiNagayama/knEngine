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

namespace
{
	const int SETTING_TIME_3_MIN_PER_SEC = 180.0f;		// 秒ごとで制限時間を3分に設定。
	const int SETTING_TIME_6_MIN_PER_SEC = 360.0f;	// 秒ごとで制限時間を5分に設定。	
	const int GAME_END_TIME_PER_FRAME = 0.0f;			// フレームごとでゲームが終了する時間
	const int PICK_UP_BELL_NUMBER_TO_REGISTER = 9;		// ベルを取得した音の登録番号
	const int ESCAPE_SOUND_NUMBER_TO_REGISTER = 10;		// 逃走時の音の登録番号
	const float ESCAPE_VOLUME_MULTIPLIER = 0.3f;		// 逃走時の音の乗算する値
	const float MAX_ESCAPE_VOLUME = 1.0f;
	const float MIN_ESCAPE_VOLUME = 0.0f;
}

bool Game::Start()
{
	StartTitle();
	
	g_soundEngine->ResistWaveFileBank(PICK_UP_BELL_NUMBER_TO_REGISTER, "Assets/sound/pickup_bell.wav");
	g_soundEngine->ResistWaveFileBank(ESCAPE_SOUND_NUMBER_TO_REGISTER, "Assets/sound/escape_00.wav");
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
	PlayEscapeSound();

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
		// ゲームオーバーエフェクトの後始末。
		m_gameOverEffect->EndGameOverEffect();
		m_gameOverEffect->StopHorrorSound();
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
		m_gameTimeScreen->Reset();
		m_score = 0;
		m_highScore = 0;
	}
}

void Game::InitInGame()
{
	m_inGameLevel.Init("Assets/level3D/stage2.tkl", [&](LevelObjectData& objData) {
		//ステージ
		if (objData.EqualObjectName(L"stage") == true) {
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
		// 収集アイテム
		if (objData.EqualObjectName(L"collectItem") == true)
		{
			auto collectItem = NewGO<CollectItem>(0, "collectItem");
			collectItem->SetPosition(objData.position);
			collectItem->SetEdgeManagement(&m_edgeManagement);
			m_collectItem.push_back(collectItem);
			return true;
		}
		return false;
	});
	
	// 輪郭線制御情報の初期化
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

	for (auto collectItem : m_collectItem)
	{
		DeleteGO(collectItem);
	}
	m_collectItem.clear();

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
	m_remainingTime = SETTING_TIME_3_MIN_PER_SEC;

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

void Game::PlayEscapeSound()
{
	bool isPlaySound = false;

	if (m_gameState == enGameState_InGame) {
		isPlaySound = m_enemy->IsPlayEscapeSound();
	}

	// 音が鳴っていない時に鳴るように設定されたならば。
	if (isPlaySound && m_escapeSound == nullptr) {
		m_escapeSound = NewGO<SoundSource>(0);
		m_escapeSound->Init(ESCAPE_SOUND_NUMBER_TO_REGISTER);
		EscapeSoundVolumeControl(isPlaySound);
		m_escapeSound->Play(true);
	}

	// 音は鳴っているが、音量が小さいとき。
	if (isPlaySound && m_escapeSoundVolume < MAX_ESCAPE_VOLUME) {
		// フェードインさせる。
		EscapeSoundVolumeControl(true);
	}
	// 音が鳴っている時に鳴らないように設定されたならば。
	else if (isPlaySound == false && m_escapeSound != nullptr) {
		// フェードアウトさせる。
		EscapeSoundVolumeControl(false);
		if (m_escapeSoundVolume <= MIN_ESCAPE_VOLUME) {
			// 音を削除。
			DeleteGO(m_escapeSound);
			m_escapeSound = nullptr;
			// 音量をリセット。
			m_escapeSoundVolume = MAX_ESCAPE_VOLUME;
		}
	}

}

void Game::EscapeSoundVolumeControl(bool fadeIn)
{

	if (fadeIn) {
		// フェードイン。
		m_escapeSoundVolume += ESCAPE_VOLUME_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
		// 上限値を超えるならば。
		if (m_escapeSoundVolume > 1.0f) {
			// 数値を固定する。
			m_escapeSoundVolume = 1.0f;
		}
	}
	else {
		// フェードアウト。
		m_escapeSoundVolume -= ESCAPE_VOLUME_MULTIPLIER * g_gameTime->GetFrameDeltaTime();

	}	
	// 音量を設定する。
	m_escapeSound->SetVolume(m_escapeSoundVolume);
}
