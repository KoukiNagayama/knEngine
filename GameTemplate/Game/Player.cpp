#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

namespace
{
	const float PLAYER_RADIUS = 50.0f;			// プレイヤーの半径
	const float PLAYER_HEIGHT = 170.0f;			// プレイヤーの高さ
	const float WALK_SPEED = 500.0f;			// 歩く速さ
	const float FRICTION = 0.5f;				// 摩擦
	const float END_OF_MOVE = 0.001f;			// 1フレームの移動量によって移動終了を表すベクトル
	const float Y_AXIS_MOVING_AMOUNT = 0.0f;	// Y軸の移動量
}

bool Player::Start()
{
	// キャラクターコントローラーを初期化。
	m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT, m_position);

	m_gameCamera = FindGO<GameCamera>("gameCamera");
	return true;
}

void Player::Update()
{
	// 移動
	Move();

	Rotation();

	// ステート管理
	ManageState();
}

void Player::Move()
{
	// 左スティックの入力量を計算
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	// カメラの前方向と右方向のベクトルを取得。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	// Y方向には移動させない。
	forward.y = Y_AXIS_MOVING_AMOUNT;
	right.y = Y_AXIS_MOVING_AMOUNT;

	// 移動速度にスティックの入力量を加算する。
	m_moveSpeed += right * stickL.x * WALK_SPEED;
	m_moveSpeed += forward * stickL.y * WALK_SPEED;

	// 移動に摩擦を付与する。
	m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
	m_moveSpeed.z -= m_moveSpeed.z * FRICTION;

	// 移動速度が既定値以下だったら
	if (m_moveSpeed.Length() < END_OF_MOVE)
	{
		// さらに摩擦を付与する。
		m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
		m_moveSpeed.z -= m_moveSpeed.z * FRICTION;
	}

	// キャラコンを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::Rotation()
{
	//カメラの向きを取得
	Vector3 rotVec = m_gameCamera->GetToCameraPos();
	//yの値の情報は要らないので、0.0fを代入して正規化。
	rotVec.y = 0.0f;
	rotVec.Normalize();

	//現在どれだけ回転しているのかの角度を求める。
	float angle = atan2(rotVec.x, rotVec.z);
	//その角度を使って、Y軸回りでQuaternionを設定する。
	m_rotation.SetRotation(Vector3::AxisY, angle);
}

void Player::ManageState()
{
	switch (m_playerState) {
	case enPlayerState_Idle:
		break;
	case enPlayerState_Walk:
		break;
	case enPlayerState_Stop:
		break;
	}
}

void Player::Render(RenderContext& rc)
{

}
