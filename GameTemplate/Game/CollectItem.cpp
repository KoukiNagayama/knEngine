#include "stdafx.h"
#include "CollectItem.h"
#include "EdgeManagement.h"
#include "Player.h"
#include "Game.h"
#include "Bell.h"
#include "sound/SoundEngine.h"

namespace
{
	const int NUM_ANIMATIONCLIP = 0;								// アニメーションクリップの数
	const int MAX_INSTANCE = 0;
	const int EDGE_COLOR_WHITE = 0;									// 輪郭線の色(白)
	const int EDGE_COLOR_YELLOW = 2;								// 輪郭線の色(黄)
	const Vector3 ITEM_MODEL_SCALE = { 5.0f,5.0f,5.0f };			// モデルの拡大率
	const float PLAYER_POSSIBLE_GET_ITEM_DISTANCE_TO_ITEM = 100.0f;	// プレイヤーがアイテムを取得可能な距離
	const float TIME_TO_DRAW_AGAIN_PER_SEC = 60.0f;					// 再び描画するまでの時間。(単位:秒)
	const float MINIMUM_TIMER_VALUE = 0.0f;							// タイマーの最低値
	const int LOW_POINT = 1;										// 低いポイントを取得した時に代入する値。
	const int HIGH_POINT = 3;										// 高いポイントを取得した時に代入する値。
	const float COLOR_CHANGE_DISTANCE_TO_PLAYER = 500.0f;			// 色を変更するプレイヤーとの距離
	const int PICK_UP_BELL_NUMBER_TO_REGISTER = 9;					// ベルを取得した音の登録番号
	const float PICK_UP_BELL_VOLUME = 1.0f;							// ベルを取得した際の音の音量
	const float ADD_ROTATION_PER_FRAME = 1.5f;						// フレームごとに回転を増加させる値
}

bool CollectItem::Start()
{
	// モデルの初期化。
	m_itemModel.Init(
		"Assets/modelData/item/bell.tkm",
		nullptr,
		NUM_ANIMATIONCLIP,
		enModelUpAxisZ,
		MAX_INSTANCE,
		EDGE_COLOR_YELLOW,
		m_edgeManagement->GetEdgeControl()
	);
	// モデルの拡大率を設定。
	m_itemModel.SetScale(ITEM_MODEL_SCALE);

	// プレイヤーのインスタンスを検索。
	m_player = FindGO<Player>("player");

	// ゲームのインスタンスを検索。
	m_game = FindGO<Game>("game");

	// プレイヤーが持つベルのインスタンスを検索。
	m_bell = FindGO<Bell>("bell");

	// 現在の輪郭線の色が白であることを保持する。
	m_currentColor = EDGE_COLOR_WHITE;

	return true;

}

void CollectItem::Update()
{
	// 回転させる。
	Rotation();

	// 描画されていないならば。
	if (m_isDraw == false) {
		// 出現するまでの時間を調べる。
		CountAppearsAgain();
		// 以降の処理を行わない。
		return;
	}
	// アイテムを取得。
	RetrieveItem();

	// 色を変更するか調べる。
	//CheckChangeColor();

	// 座標を設定。
	m_itemModel.SetPosition(m_position);

	// モデルの更新。
	m_itemModel.Update();
}

void CollectItem::Rotation()
{
	// 回転を加算。
	m_rotation.AddRotationDegY(ADD_ROTATION_PER_FRAME);
	// モデルに回転の値を設定。
	m_itemModel.SetRotation(m_rotation);
}

void CollectItem::CheckDistanceToPlayer()
{
	// プレイヤーの座標。
	m_playerPos = m_player->GetPosition();
	// プレイヤーとの距離。
	m_distToPlayer = m_position - m_playerPos;

}

void CollectItem::RetrieveItem()
{
	// プレイヤーとの距離を調べる。
	CheckDistanceToPlayer();

	// プレイヤーがアイテムを取得するならば。
	if (m_distToPlayer.Length() < PLAYER_POSSIBLE_GET_ITEM_DISTANCE_TO_ITEM) {
		// ポイントを加算する。
		m_game->AddScore(LOW_POINT);

		// 描画をしないようにする。
		m_isDraw = false;
		// 再び描画されるまでのタイマーをリセット。
		m_drawAgainTimerPerSec = TIME_TO_DRAW_AGAIN_PER_SEC;

		m_pickUpBellSound = NewGO<SoundSource>(0);
		m_pickUpBellSound->Init(PICK_UP_BELL_NUMBER_TO_REGISTER);
		m_pickUpBellSound->SetVolume(PICK_UP_BELL_VOLUME);
		m_pickUpBellSound->Play(false);
	}
}

void CollectItem::CheckChangeColor()
{
	// プレイヤーとの距離を調べる。
	CheckDistanceToPlayer();

	// プレイヤーがベルを鳴らしているか。
	bool isBellRing = m_bell->IsBellRinging();

	// 現在の色が黄ならば色を白に戻すためのタイマーを進める。
	if (m_currentColor == EDGE_COLOR_YELLOW) {
		m_resetColorTimerPerSec -= g_gameTime->GetFrameDeltaTime();
	}

	// プレイヤーがベルを鳴らしている時に距離が近く、現在の色が白ならば。
	if (m_distToPlayer.Length() < COLOR_CHANGE_DISTANCE_TO_PLAYER 
		&& m_currentColor == EDGE_COLOR_WHITE
		&& isBellRing == true
		) 
	{
		// モデルを初期化する。(輪郭線の色は黄)
		m_itemModel.Init(
			"Assets/modelData/item/bell.tkm",
			nullptr,
			NUM_ANIMATIONCLIP,
			enModelUpAxisZ,
			MAX_INSTANCE,
			EDGE_COLOR_YELLOW,
			m_edgeManagement->GetEdgeControl()
		);
		// 現在の色を保持する。
		m_currentColor = EDGE_COLOR_YELLOW;
		// 色をリセットするタイマーを初期化する。
		m_resetColorTimerPerSec = 5.0f;

	}
	// プレイヤーとの距離が遠く、現在の色が黄ならば。
	else if (m_currentColor == EDGE_COLOR_YELLOW
		&& m_resetColorTimerPerSec <= 0.0f
		)
	{
		// モデルを初期化する。(輪郭線の色は白)
		m_itemModel.Init(
			"Assets/modelData/item/bell.tkm",
			nullptr,
			NUM_ANIMATIONCLIP,
			enModelUpAxisZ,
			MAX_INSTANCE,
			EDGE_COLOR_WHITE,
			m_edgeManagement->GetEdgeControl()
		);
		m_currentColor = EDGE_COLOR_WHITE;
	}
	
}

void CollectItem::CountAppearsAgain()
{
	// 再び描画されるまでのタイマーの値を減らす。
	m_drawAgainTimerPerSec -= g_gameTime->GetFrameDeltaTime();

	if (m_drawAgainTimerPerSec <= MINIMUM_TIMER_VALUE) {
		// 描画するように設定する。
		m_isDraw = true;
	}
}

void CollectItem::Render(RenderContext& rc)
{
	// 描画しないようになっているならば。
	if (m_isDraw == false) {
		// 描画しない。
		return;
	}
	// モデルの描画。
	m_itemModel.Draw();
}