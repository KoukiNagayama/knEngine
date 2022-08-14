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
	const int PLAYABLE_TIME_PER_SEC = 10.0f;					// 秒ごとで制限時間を3分に設定。
	const int GAME_END_TIME_PER_SEC = 0.0f;						// 秒ごとでゲームが終了する時間
	const int PICK_UP_BELL_NUMBER_TO_REGISTER = 9;				// ベルを取得した音の登録番号
	const int ESCAPE_SOUND_NUMBER_TO_REGISTER = 10;				// 逃走時の音の登録番号
	const float SOUND_VOLUME_MULTIPLIER = 0.3f;					// 逃走時の音の乗算する値
	const float MAX_SOUND_VOLUME = 1.0f;						// 最大音量
	const float MIN_SOUND_VOLUME = 0.0f;						// 最小音量
	const float MAX_SOUND_MUL_VOLUME = 0.7f;					// 音量に乗算する最大値
	const float MIN_SOUND_MUL_VOLUME = 0.0f;					// 音量に乗算する最小値
	const float CLOSE_TO_ENEMY_SOUND_NUMBER_TO_REGISTER = 11;	// 敵が近くなった時の音の登録番号
	const float CLOSE_TO_ENEMY_SOUND_RANGE = 2000.0f;			// 敵が近くなった時に聴こえる音の範囲
	const int GAME_TIME_SCREEN_PRIORITY = 10;					// 残り時間表示オブジェクトの描画の優先順位(遅め)
	const int SCORE_SCREEN_PRIORITY = 11;						// スコア表示オブジェクトの描画の優先順位(遅め)
	const int GENERAL_PRIORITY = 0;								// 汎用的なオブジェクトの描画の優先順位(早め)
	const int FADE_PRIORITY = 12;								// フェードのオブジェクトの描画の優先順位(一番遅い)
	const float FADE_SPEED_TITLE_TO_IN_GAME = 0.75f;			// タイトルからインゲームに遷移するフェードのスピード
	const float FADE_SPEED_IN_GAME_TO_RESULT = 1.4f;			// インゲームからリザルトに遷移するフェードのスピード
	const float FADE_SPEED_RESULT_TO_TITLE = 1.0f;				// リザルトからタイトルに遷移するフェードのスピード
	const int RESET_SCORE = 0;									// スコアのリセット
	const int RESET_HIGHSCORE = 0;								// ハイスコアのリセット
}

bool Game::Start()
{
	// タイトルを初期化。
	StartTitle();
	
	// 波形データを登録する。
	g_soundEngine->ResistWaveFileBank(PICK_UP_BELL_NUMBER_TO_REGISTER, "Assets/sound/pickup_bell.wav");
	g_soundEngine->ResistWaveFileBank(ESCAPE_SOUND_NUMBER_TO_REGISTER, "Assets/sound/escape_00.wav");
	g_soundEngine->ResistWaveFileBank(CLOSE_TO_ENEMY_SOUND_NUMBER_TO_REGISTER, "Assets/sound/normal_00.wav");

	// 残り時間を表示するオブジェクトを作成（前面に出すため描画順番遅め）
	m_gameTimeScreen = NewGO<GameTimeScreen>(GAME_TIME_SCREEN_PRIORITY, "gameTimeScreen");
	// 無を描画しないように初期化
	m_gameTimeScreen->GameTimerUpdate(m_remainingTime);

	// ゲームオーバーエフェクトを作成
	m_gameOverEffect = new GameOverEffect;

	// スコアを表示する
	m_scoreScreen = NewGO<Score>(SCORE_SCREEN_PRIORITY, "score");
	// 表示するスコアを更新。
	m_scoreScreen->ScoreUpdate();
	m_scoreScreen->HighScoreUpdate();

	return true;
}

void Game::Update()
{
	// ステート管理
	ManageState();
	
	// ステートごとの処理
	ProcessByState();

	// 逃走時の音を再生する。
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

void Game::ProcessByState()
{
	switch (m_gameState) {
	case enGameState_Title:
		break;
	case enGameState_InGame:
		// タイマーを進める。
		GameTimer();
		// 輪郭線制御の情報を更新する。
		m_edgeManagement.Update();
		// ゲームオーバーエフェクト中なら実行処理
		if (m_gameOverEffect->IsPlayEffect()) {
			m_gameOverEffect->PlayGameOverEffect();
		}
		break;
	case enGameState_GameOver:
		// タイマーを進める。
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
			// タイトルのオブジェクトを削除する。
			DeleteTitleObject();
			m_edgeManagement.Clear();
			// ゲームステートをインゲームに変更する。
			m_gameState = enGameState_InGame;
			// インゲームを初期化する。
			InitInGame();
			// フェードインを開始する。
			m_fade->StartFadeIn();
			m_isWaitFadeout = false;
		}
	}
	else {
		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_isWaitFadeout = true;
			// フェードアウトを開始する。
			m_fade->StartFadeOut();
			m_fade->SetFadeSpeed(FADE_SPEED_TITLE_TO_IN_GAME);
		}
	}
}

void Game::StateTransitionProccesingFromInGame()
{
	if (m_isWaitFadeout) {
		// フェードが終わっているならば。
		if (!m_fade->IsFade()) {
			//DeleteGO(m_fade);
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
			m_result = NewGO<Result>(GENERAL_PRIORITY, "result");
			m_result->Init(m_highScore);

			// フェードインを開始する。
			m_fade->StartFadeIn();
			m_isWaitFadeout = false;
		}
	}
	else {
		// 残り時間がないならば。
		if (m_remainingTime <= GAME_END_TIME_PER_SEC) {
			m_isWaitFadeout = true;
			// フェードアウトを開始する。
			m_fade->StartFadeOut();
			m_fade->SetFadeSpeed(FADE_SPEED_IN_GAME_TO_RESULT);
		}
	}

	// ゲームオーバーになっていれば。
	if (m_isGameOver) {
		m_gameOver = NewGO<GameOver>(GENERAL_PRIORITY, "gameOver");
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
	m_score = RESET_SCORE;
	
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
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			//DeleteGO(m_fade);
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
			m_score = RESET_SCORE;
			m_highScore = RESET_HIGHSCORE;

			// フェードインを開始する。
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
		//ステージ
		if (objData.EqualObjectName(L"stage") == true) {
			// 背景クラス
			m_inGameStage = NewGO<BackGround>(GENERAL_PRIORITY, "backGround");
			m_inGameStage->SetPosition(objData.position);
			m_inGameStage->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		//プレイヤー(カメラ)
		if (objData.EqualObjectName(L"player") == true) {
			// プレイヤークラス
			m_player = NewGO<Player>(GENERAL_PRIORITY, "player");
			m_player->SetPosition(objData.position);
			// ゲームカメラクラス
			m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
			m_gameCamera->SetPosition(objData.position);

			m_bell = NewGO<Bell>(GENERAL_PRIORITY, "bell");
			m_bell->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		// エネミー
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			m_enemy = NewGO<Enemy>(GENERAL_PRIORITY, "enemy");
			m_enemy->SetPosition(objData.position);
			m_enemy->SetNumber(objData.number);
			m_enemy->SetEdgeManagement(&m_edgeManagement);
			return true;
		}
		// 収集アイテム
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
	
	// 輪郭線制御情報の初期化
	m_edgeManagement.Init();


	// ゲームオーバーエフェクトの初期化
	m_gameOverEffect->Init(m_player, m_enemy, m_gameCamera);
}

void Game::DeleteInGameObject()
{
	// エネミー削除
	// エネミーの情報を検索。
	auto& enemys = FindGOs<Enemy>("enemy");
	// エネミーの数
	int enemySize = enemys.size();
	for (int i = 0; i < enemySize; i++) {
		m_enemy = enemys[i];
		// 削除
		DeleteGO(m_enemy);
	}
	// ゲームカメラ削除
	DeleteGO(m_gameCamera);
	// 収集アイテム削除
	for (auto collectItem : m_collectItem)
	{
		DeleteGO(collectItem);
	}
	m_collectItem.clear();
	// ベル削除
	DeleteGO(m_bell);
	// プレイヤー削除
	DeleteGO(m_player);
	// ステージ削除
	DeleteGO(m_inGameStage);

}

void Game::DeleteTitleObject()
{
	// タイトルカメラ削除
	DeleteGO(m_titleCamera);
	// タイトルテキスト削除
	DeleteGO(m_titleText);
	// ステージ削除
	DeleteGO(m_inGameStage);
	// タイトルにあるスプライトを削除
	DeleteGO(m_titleSprite);
}

void Game::GameTimer()
{
	// 残り時間を計算。
	m_remainingTime -= g_gameTime->GetFrameDeltaTime();
	
	// 残り時間を渡す
	m_gameTimeScreen->GameTimerUpdate(m_remainingTime);
}

void Game::StartGameOverEffect() 
{
	// ゲームオーバーエフェクトを開始
	m_gameOverEffect->StartGameOverEffect();
}

void Game::StartTitle() {

	// 時間を設定。
	m_remainingTime = PLAYABLE_TIME_PER_SEC;

	// 各オブジェクトを生成する。
	// タイトルのレベルを初期化。
	m_titleLevel.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
		{
			// タイトルモデル
			if (objData.EqualObjectName(L"titleText") == true) {
				m_titleText = NewGO<TitleText>(GENERAL_PRIORITY, "titleText");
				m_titleText->SetPosition(objData.position);
				m_titleText->SetEdgeManagement(&m_edgeManagement);
				return true;
			}
			// タイトル用カメラ
			if (objData.EqualObjectName(L"titleCamera") == true) {
				m_titleCamera = NewGO<TitleCamera>(GENERAL_PRIORITY, "titleCamera");
				m_titleCamera->SetPosition(objData.position);
				return true;
			}
			// 背景
			if (objData.EqualObjectName(L"stage2") == true) {
				m_inGameStage = NewGO<BackGround>(GENERAL_PRIORITY, "backGround");
				m_inGameStage->SetPosition(objData.position);
				m_inGameStage->SetEdgeManagement(&m_edgeManagement);
				return true;
			}
		});
	// タイトル時に表示するスプライトを生成する。
	m_titleSprite = NewGO<TitleSprite>(GENERAL_PRIORITY, "titleSprite");

	// 輪郭線情報を初期化する。
	m_edgeManagement.Init();

	// フェードのオブジェクトが既に生成されているならば削除する。
	if (m_fade == nullptr) {
		m_fade = NewGO<Fade>(FADE_PRIORITY, "fade");
	}

	// フェードインを開始
	m_fade->StartFadeIn();
}

void Game::PlayEscapeSound()
{
	// 音を再生するか
	bool isPlaySound = false;

	// エネミーを検索
	const auto& enemys = FindGOs<Enemy>("enemy");
	// エネミーの数
	const int size = enemys.size();

	// インゲームの時に
	if (m_gameState == enGameState_InGame) {
		for (int i = 0; i < size; i++) {
			m_enemy = enemys[i];
			// 再生するように設定されているならば。
			if (m_enemy->IsChase()) {
				isPlaySound = true;
				break;
			}
		}
	}

	// 音が鳴っていない時に鳴るように設定されたならば。
	if (isPlaySound && m_escapeSound == nullptr) {
		m_escapeSound = NewGO<SoundSource>(0);
		m_escapeSound->Init(ESCAPE_SOUND_NUMBER_TO_REGISTER);
		EscapeSoundVolumeControl(isPlaySound);
		m_escapeSound->Play(true);
	}

	// 音は鳴っているが、音量が小さいとき。
	if (isPlaySound && m_escapeSoundVolume < MAX_SOUND_VOLUME) {
		// フェードインさせる。
		EscapeSoundVolumeControl(true);
	}
	// 音が鳴っている時に鳴らないように設定されたならば。
	else if (isPlaySound == false && m_escapeSound != nullptr) {
		// フェードアウトさせる。
		EscapeSoundVolumeControl(false);
		if (m_escapeSoundVolume <= MIN_SOUND_VOLUME) {
			// 音を削除。
			DeleteGO(m_escapeSound);
			m_escapeSound = nullptr;
			// 音量をリセット。
			m_escapeSoundVolume = MAX_SOUND_VOLUME;
		}
	}

}

void Game::EscapeSoundVolumeControl(bool fadeIn)
{

	if (fadeIn) {
		// フェードイン。
		m_escapeSoundVolume += SOUND_VOLUME_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
		// 上限値を超えるならば。
		if (m_escapeSoundVolume > MAX_SOUND_VOLUME) {
			// 数値を固定する。
			m_escapeSoundVolume = MAX_SOUND_VOLUME;
		}
	}
	else {
		// フェードアウト。
		m_escapeSoundVolume -= SOUND_VOLUME_MULTIPLIER * g_gameTime->GetFrameDeltaTime();

	}	
	// 音量を設定する。
	m_escapeSound->SetVolume(m_escapeSoundVolume);
}


