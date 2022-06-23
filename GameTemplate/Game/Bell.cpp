#include "stdafx.h"
#include "Bell.h"
#include "Player.h"

namespace
{
	const float SHIFT_POSITION_FROM_CAMERA = 35.0f;				// カメラからずらす値
	const float MODEL_POSITION_UP = 25.0f;						// ベルを少し上にあげる
}

bool Bell::Start()
{
	// プレイヤーを検索。
	m_player = FindGO<Player>("player");

	// ベルのモデルを初期化。
	m_bellModel.Init(
		"Assets/modelData/item/bell.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		0
	);



	return true;
}

void Bell::Update()
{
	MoveWithPlayer();

	m_bellModel.Update();
}

void Bell::MoveWithPlayer()
{
	
	// 上方向ベクトル。
	Vector3 up;
	// 右方向ベクトル。
	Vector3 right;
	// 前方向ベクトル。
	Vector3 forward;

	// カメラの右方向を取得。
	right = g_camera3D->GetRight();
	// カメラの前方向を取得。
	forward = g_camera3D->GetForward();
	// カメラの上方向を外積によって求める。
	up.Cross(right, forward);
	up.Normalize();

	m_position = g_camera3D->GetPosition();
	// ベルの座標をずらす。
	m_position += g_camera3D->GetRight() * SHIFT_POSITION_FROM_CAMERA;
	m_position += g_camera3D->GetForward() * SHIFT_POSITION_FROM_CAMERA;
	m_position += up * MODEL_POSITION_UP;

	// ベルの座標を設定。
	m_bellModel.SetPosition(m_position);
	// ベルの回転を設定。
	m_bellModel.SetRotation(m_player->GetRotation());

}

void Bell::Render(RenderContext& rc)
{
	m_bellModel.Draw();
}
