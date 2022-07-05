#include "stdafx.h"
#include "GameTimeScreen.h"

namespace
{
	const Vector2 NUMBER_SPRITE_SIZE = { 100.0f,160.0f };				// 数字の画像サイズ
	const Vector3 NUMBER_DEF_POSITION = { -860.0f,430.0f,0.0f };		// 3桁目の数字の座標
	const Vector3 NUMBER_ADD_POSITION = { 120.0f,0.0f,0.0f };			// 数字の移動量（文字間隔）

	const int COLOR_CHANGE_BORDER = 30;									// 残り時間がこの値以下になったら色変更
	const Vector4 COLOR_CHANGE = { 1.0f,0.0f,0.0f,1.0f };				// 残り時間が少ない時の色
}

GameTimeScreen::GameTimeScreen() {

	// 要素数を計算
	m_numberNum = sizeof(m_numberSprite) / sizeof(SpriteRender);

	// 座標設定
	Vector3 number_pos = NUMBER_DEF_POSITION;
	for (int i = 0; i < m_numberNum; i++) {

		m_numberSprite[i].SetPosition(number_pos);
		number_pos += NUMBER_ADD_POSITION;

		m_numberSprite[i].Update();
	}
}

GameTimeScreen::~GameTimeScreen() {
}

void GameTimeScreen::GameTimerUpdate(const float time) {

	// intに変換しておく
	int now_time = (int)time;

	int digit = 1;	// 初期化
	int num = 0;

	// 数字の更新
	for (int i = m_numberNum - 1; i >= 0; i--) {

		// 一桁目から順番に各桁の値を抽出
		num = (now_time / digit) % 10;
		digit *= 10;

		switch (num)
		{
		case 0:
			m_numberSprite[i].Init("Assets/sprite/Number_0.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 1:
			m_numberSprite[i].Init("Assets/sprite/Number_1.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 2:
			m_numberSprite[i].Init("Assets/sprite/Number_2.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 3:
			m_numberSprite[i].Init("Assets/sprite/Number_3.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 4:
			m_numberSprite[i].Init("Assets/sprite/Number_4.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 5:
			m_numberSprite[i].Init("Assets/sprite/Number_5.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 6:
			m_numberSprite[i].Init("Assets/sprite/Number_6.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 7:
			m_numberSprite[i].Init("Assets/sprite/Number_7.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 8:
			m_numberSprite[i].Init("Assets/sprite/Number_8.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		case 9:
			m_numberSprite[i].Init("Assets/sprite/Number_9.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
			break;
		}
	}

	// 色の更新
	if (m_colorChangeFlag == false && now_time <= COLOR_CHANGE_BORDER) {
		m_colorChangeFlag = true;
		for (int i = 0; i < m_numberNum; i++) {
			m_numberSprite[i].SetMulColor(COLOR_CHANGE);
		}
	}
}

void GameTimeScreen::Render(RenderContext& rc) {

	for (int i = 0; i < m_numberNum; i++) {
		m_numberSprite[i].Draw(rc);
	}
}

