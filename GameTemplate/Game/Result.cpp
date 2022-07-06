#include "stdafx.h"
#include "Result.h"

namespace
{
	const float	  RESULT_SPRITE_W = 1920.0f;								// 背景スプライトの横幅。
	const float   RESULT_SPRITE_H = 1080.0f;								// 背景スプライトの縦幅。

	const Vector2 NUMBER_SPRITE_SIZE = { 160.0f,280.0f };					// 数字の画像サイズ
	const Vector3 SCORE_NUMBER_DEF_POSITION = { -220.0f,60.0f,0.0f };		// スコアの3桁目の数字の座標
	const Vector3 NUMBER_ADD_POSITION = { 200.0f,0.0f,0.0f };				// 数字の移動量（文字間隔）
}

Result::Result() {

}

Result::~Result() {

}

void Result::Init(const int score) {

	// スコアの保存
	m_score = score;

	// スプライトの初期化
	m_resultSprite_BG.Init(
		"Assets/sprite/GameClear_New.DDS",
		RESULT_SPRITE_W,
		RESULT_SPRITE_H
	);

	// 要素数を計算
	m_numberNum = sizeof(m_scoreNumberSprite) / sizeof(SpriteRender);

	// 画像を設定
	ScoreUpdate();

	// 座標設定
	Vector3 scoreNumber_pos = SCORE_NUMBER_DEF_POSITION;

	for (int i = 0; i < m_numberNum; i++) {
		// 桁ごとにスプライトの座標を設定。
		m_scoreNumberSprite[i].SetPosition(scoreNumber_pos);

		// 座標をずらす。
		scoreNumber_pos += NUMBER_ADD_POSITION;

		// スプライトを更新。
		m_scoreNumberSprite[i].Update();
	}

	// 設定完了
	m_initFlag = true;
}

void Result::Render(RenderContext& rc) {

	if (m_initFlag == false) {
		return;
	}

	m_resultSprite_BG.Draw(rc);

	for (int i = 0; i < m_numberNum; i++) {
		m_scoreNumberSprite[i].Draw(rc);
	}

}

void Result::ScoreUpdate() {

	int digit = 1;	// 初期化
	int num = 0;

	// 数字の更新
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// 一桁目から順番に各桁の値を抽出
		num = (m_score / digit) % 10;
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
	}

}