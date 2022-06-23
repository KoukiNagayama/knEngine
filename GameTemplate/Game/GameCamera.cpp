#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace {
	const float TO_CAMERA_POSITION_X_FROM_TARGET = 0.0f;	// 注視点から視点へのベクトルのX要素
	const float TO_CAMERA_POSITION_Y_FROM_TARGET = 0.0f;	// 注視点から視点へのベクトルのY要素
	const float TO_CAMERA_POSITION_Z_FROM_TARGET = -77.0f;	// 注視点から視点へのベクトルのZ要素
	const float FAR_CLIP = 10000.0f;						// 遠平面
	const float NEAR_CLIP = 3.0f;							// 近平面
	const float CAMERA_SPEED_MULTIPLIER = 1.5f;				// カメラの回転速度の倍率
	const float CAMERA_ANGLE_MULTIPLIER = 1.3f;				// カメラの角度の倍率
	const float CAMERA_ROTATION_UPPER_LIMIT = 0.35f;		// カメラの回転の上の上限
	const float CAMERA_ROTATION_LOWER_LIMIT = -0.7f;		// カメラの回転の下の上限
	const float CAMERA_POSITION_Y = 250.0f;					// 視点の高さ
}

bool GameCamera::Start()
{
	// 注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(
		TO_CAMERA_POSITION_X_FROM_TARGET,
		TO_CAMERA_POSITION_Y_FROM_TARGET,
		TO_CAMERA_POSITION_Z_FROM_TARGET
	);

	// 近平面までの距離を設定。
	g_camera3D->SetNear(NEAR_CLIP);
	// 遠平面までの距離を設定。
	g_camera3D->SetFar(FAR_CLIP);

	//m_position.y = 

	// プレイヤーを検索
	m_player = FindGO<Player>("player");
	return true;
}

void GameCamera::Update()
{
	// 移動
	Move();

	// 注視点を変更
	ViewPoint();

	g_camera3D->Update();
}

void GameCamera::Move()
{
	// 1人称視点の為カメラはプレイヤーの座標を代入。
	m_position = m_player->GetPosition();

	// 視点の高さを上げる
	m_position.y = CAMERA_POSITION_Y;

	g_camera3D->SetPosition(m_position);
}

void GameCamera::ViewPoint()
{
	// 古い注視点から視点までのベクトル
	Vector3 toCameraPosOld = m_toCameraPos;

	float x = g_pad[0]->GetRStickXF() * CAMERA_SPEED_MULTIPLIER;
	float y = g_pad[0]->GetRStickYF() * CAMERA_SPEED_MULTIPLIER;

	// Y軸周りの回転。
	Quaternion qRot;
	// Degree単位で回転クォータニオンを作成。
	qRot.SetRotationDeg(Vector3::AxisY, CAMERA_ANGLE_MULTIPLIER * x);
	// 注視点から視点に伸びるベクトルに作成した回転クォータニオンを適用する。
	qRot.Apply(m_toCameraPos);

	// X軸周りの回転。
	Vector3 axisX;
	// Y軸と注視点から伸びるベクトルの外積を計算し正規化する。
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	// Degree単位で回転クォータニオンを作成。
	qRot.SetRotationDeg(axisX, CAMERA_ANGLE_MULTIPLIER * -y);
	// 注視点から視点に伸びるベクトルに作成した回転クォータニオンを適用する。
	qRot.Apply(m_toCameraPos);
	m_rotation = qRot;

	// 回転の上限を超えたならば
	if (RotationLimit() == false) {
		// カメラの注視点を変えない
		m_toCameraPos = toCameraPosOld;
	}

	// 注視点を計算する
	m_target = m_position + m_toCameraPos;

	// 注視点を設定
	g_camera3D->SetTarget(m_target);

}

bool GameCamera::RotationLimit()
{
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	// カメラの上限を超えているか。
	if (toPosDir.y < CAMERA_ROTATION_LOWER_LIMIT
		|| toPosDir.y > CAMERA_ROTATION_UPPER_LIMIT
		) 
	{
		return false;
	}
	// 超えていない。
	return true;
}
