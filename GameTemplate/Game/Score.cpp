#include "stdafx.h"
#include "Score.h"
#include "Game.h"

namespace
{
	const Vector2 NUMBER_SPRITE_SIZE = { 65.0f,104.0f };					// 数字の画像サイズ
	const Vector3 SCORE_NUMBER_DEF_POSITION = { 700.0f,450.0f,0.0f };		// スコアの3桁目の数字の座標
	const Vector3 HIGHSCORE_NUMBER_DEF_POSITION = { 700.0f,345.0f,0.0f };	// ハイスコアの3桁目数字の座標
	const Vector3 NUMBER_ADD_POSITION = { 60.0f,0.0f,0.0f };				// 数字の移動量（文字間隔）
	const Vector3 SCORE_SPRITE_POSITION = { 550.0f,445.0f,0.0f };			// SCOREのスプライトの座標
	const Vector3 HIGHSCORE_SPRITE_POSITION = { 480.0f,330.0f,0.0f };		// HIGHSCOREのスプライトの座標
	const Vector2 SCORE_AND_HIGHSCORE_SPRITE_SIZE = { 512.0f,512.0f };		// SCOREとHIGHSCOREのスプライトの画像サイズ
	const Vector3 SCORE_AND_HIGHSCORE_SPRITE_SCALE = {1.0f,1.0f,1.0f};		// SCOREとHIGHSCOREのスプライトの拡大率
}

Score::Score()
{
	// ゲームクラスのオブジェクト情報を検索。
	m_game = FindGO<Game>("game");

	// 要素数を計算
	m_numberNum = sizeof(m_scoreNumberSprite) / sizeof(SpriteRender);

	// 座標設定
	Vector3 scoreNumber_pos = SCORE_NUMBER_DEF_POSITION;
	Vector3 highScoreNumber_pos = HIGHSCORE_NUMBER_DEF_POSITION;

	for (int i = 0; i < m_numberNum; i++) {
		// 桁ごとにスプライトの座標を設定。
		m_scoreNumberSprite[i].SetPosition(scoreNumber_pos);
		m_highScoreNumberSprite[i].SetPosition(highScoreNumber_pos);

		// 座標をずらす。
		scoreNumber_pos += NUMBER_ADD_POSITION;
		highScoreNumber_pos += NUMBER_ADD_POSITION;

		// スプライトを更新。
		m_scoreNumberSprite[i].Update();
		m_highScoreNumberSprite[i].Update();
	}

	// SCOREのスプライトの初期化。
	m_scoreSprite.Init(
		"Assets/sprite/SCORE.DDS",
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.x,
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.y
	);
	// SCOREのスプライトの座標を設定。
	m_scoreSprite.SetPosition(SCORE_SPRITE_POSITION);
	// SCOREのスプライトの拡大率を設定。
	m_scoreSprite.SetScale(SCORE_AND_HIGHSCORE_SPRITE_SCALE);
	

	// HIGHSCOREのスプライトの初期化。
	m_highScoreSprite.Init(
		"Assets/sprite/HIGHSCORE.DDS",
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.x,
		SCORE_AND_HIGHSCORE_SPRITE_SIZE.y
	);
	// HIGHSCOREのスプライトの座標を設定。
	m_highScoreSprite.SetPosition(HIGHSCORE_SPRITE_POSITION);
	// HIGHSCOREのスプライトの拡大率を設定。
	m_highScoreSprite.SetScale(SCORE_AND_HIGHSCORE_SPRITE_SCALE);

	// スプライトを更新する。
	m_scoreSprite.Update();
	m_highScoreSprite.Update();
}

void Score::Update()
{
	m_score = m_game->GetScore();

	m_highScore = m_game->GetHighScore();

	// 現在のスコアが記録されたスコアと一致していないならば。
	if (m_score != m_lastScore) {
		// 表示するスコアを更新する。
		ScoreUpdate();
	}

	// 現在のハイスコアが記録されたハイスコアと一致していないならば。
	if (m_highScore != m_lastHighScore) {
		// 表示するハイスコアを更新する。
		HighScoreUpdate();
	}

}

void Score::ScoreUpdate()
{
	// intに変換しておく
	int now_score = m_score;

	int digit = 1;	// 初期化
	int num = 0;

	// 数字の更新
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// 一桁目から順番に各桁の値を抽出
		num = (now_score / digit) % 10;
		digit *= 10;

		switch (num)
		{
		case 0:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_0.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 1:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_1.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 2:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_2.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 3:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_3.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 4:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_4.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 5:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_5.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 6:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_6.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 7:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_7.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 8:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_8.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 9:
			m_scoreNumberSprite[i].Init("Assets/sprite/Number_9.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		}

		// スコアを記録。
		m_lastScore = m_score;
	}
}

void Score::HighScoreUpdate()
{
	// intに変換しておく
	int now_score = m_highScore;

	int digit = 1;	// 初期化
	int num = 0;

	// 数字の更新
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// 一桁目から順番に各桁の値を抽出
		num = (now_score / digit) % 10;
		digit *= 10;

		switch (num)
		{
		case 0:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_0.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 1:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_1.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 2:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_2.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 3:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_3.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 4:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_4.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 5:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_5.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 6:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_6.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 7:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_7.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 8:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_8.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 9:
			m_highScoreNumberSprite[i].Init("Assets/sprite/Number_9.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		}

		// スコアを記録。
		m_lastHighScore = m_highScore;
	}
}

void Score::Render(RenderContext& rc)
{
	// 現在のゲームステートがインゲームならば。
	if (m_game->IsInGame()) {
		// スプライトを描画する。
		m_scoreSprite.Draw(rc);
		m_highScoreSprite.Draw(rc);

		for (int i = 0; i < m_numberNum; i++) {
			m_scoreNumberSprite[i].Draw(rc);
			m_highScoreNumberSprite[i].Draw(rc);
		}
	}
}


