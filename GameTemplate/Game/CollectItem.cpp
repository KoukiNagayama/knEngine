#include "stdafx.h"
#include "CollectItem.h"
#include "EdgeManagement.h"
#include "Player.h"
#include "Game.h"
namespace
{
	const int NUM_ANIMATIONCLIP = 0;								// アニメーションクリップの数
	const int MAX_INSTANCE = 0;
	const int EDGE_COLOR_WHITE = 0;									// 輪郭線の色(白)
	const int EDGE_COLOR_YELLOW = 2;								// 輪郭線の色(黄)
	const Vector3 ITEM_MODEL_SCALE = { 5.0f,5.0f,5.0f };			// モデルの拡大率
	const float PLAYER_POSSIBLE_GET_ITEM_DISTANCE_TO_ITEM = 100.0f;	// プレイヤーがアイテムを取得可能な距離
	const float TIME_TO_DRAW_AGAIN_PER_SEC = 5.0f;					// 再び描画するまでの時間。(単位:秒)
	const float MINIMUM_TIMER_VALUE = 0.0f;							// タイマーの最低値
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
		EDGE_COLOR_WHITE,
		m_edgeManagement->GetEdgeControl()
	);
	// モデルの拡大率を設定。
	m_itemModel.SetScale(ITEM_MODEL_SCALE);

	// プレイヤーのインスタンスを検索。
	m_player = FindGO<Player>("player");

	// ゲームのインスタンスを検索。
	m_game = FindGO<Game>("game");

	return true;

}

void CollectItem::Update()
{
	// 描画されていないならば。
	if (m_isDraw == false) {
		// 出現するまでの時間を調べる。
		CountAppearsAgain();
		// 以降の処理を行わない。
		return;
	}
	// プレイヤーとの距離を調べる。
	CheckDistanceToPlayer();

	// 回転させる。
	Rotation();

	// モデルの更新。
	m_itemModel.Update();
}

void CollectItem::Rotation()
{
	// 回転を加算。
	m_rotation.AddRotationDegY(1.5f);
	// モデルに回転の値を設定。
	m_itemModel.SetRotation(m_rotation);
}

void CollectItem::CheckDistanceToPlayer()
{
	// プレイヤーの座標。
	Vector3 playerPos = m_player->GetPosition();
	// プレイヤーとの距離。
	Vector3 distToPlayer = m_position - playerPos;

	// プレイヤーがアイテムを取得するならば。
	if (distToPlayer.Length() < PLAYER_POSSIBLE_GET_ITEM_DISTANCE_TO_ITEM) {
		// スコアを増やす。
		m_game->AddScore(1);
		// 描画をしないようにする。
		m_isDraw = false;
		// 再び描画されるまでのタイマーをリセット。
		m_drawAgainTimerPerSec = TIME_TO_DRAW_AGAIN_PER_SEC;
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