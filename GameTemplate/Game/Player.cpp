#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"


namespace
{
	const float PLAYER_RADIUS = 50.0f;								// プレイヤーの半径
	const float PLAYER_HEIGHT = 170.0f;								// プレイヤーの高さ
	const float WALK_SPEED = 500.0f;								// 歩く速さ
	const float FRICTION = 0.5f;									// 摩擦
	const float END_OF_MOVE = 0.001f;								// 1フレームの移動量によって移動終了を表すベクトル
	const float Y_AXIS_MOVING_AMOUNT = 0.0f;						// Y軸の移動量
	const int	PLAYER_FOOTSTEP_SOUND_NUMBER_TO_REGISTER = 1;		// 登録するプレイヤーの足音の番号
	const float FOOTSTEP_VOLUME = 0.7f;								// 足音の音量
	const float SPEED_OF_MOVING_JUDGE = 0.001f;						// 移動している判定用のスピード
	const int PLAYER_FOOTSTEP_SOUND_PRIORITY = 0;					// 足音の音源オブジェクトのプライオリティ
}

Player::Player()
{

}
Player::~Player()
{
	if (m_footstepSound != nullptr) {
		DeleteGO(m_footstepSound);
	}

	m_charaCon.RemoveRigidBoby();
}

bool Player::Start()
{
	// キャラクターコントローラーを初期化。
	m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT, m_position);

	m_gameCamera = FindGO<GameCamera>("gameCamera");
	
	g_soundEngine->ResistWaveFileBank(PLAYER_FOOTSTEP_SOUND_NUMBER_TO_REGISTER, "Assets/sound/walk.wav");

	m_footstepSound = NewGO<SoundSource>(PLAYER_FOOTSTEP_SOUND_PRIORITY);
	m_footstepSound->Init(PLAYER_FOOTSTEP_SOUND_NUMBER_TO_REGISTER);
	m_footstepSound->SetVolume(FOOTSTEP_VOLUME);
	return true;
}

void Player::Update()
{
	// 移動不能なら全ての処理を中断する
	if (m_isMovable == false) {
		return;
	}

	// 移動
	Move();

	// 回転
	Rotation();

	// ステート管理
	ManageState();

	ProcessByState();
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
	if (m_moveSpeed.Length() < END_OF_MOVE){
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

void Player::ProcessByState()
{
	switch (m_playerState) {
	case enPlayerState_Idle:
		StopFootstepSound();
		break;
	case enPlayerState_Walk:
		PlayFootstepSound();
		break;
	}
}

void Player::ManageState()
{
	switch (m_playerState) {
	case enPlayerState_Idle:
		ProcessCommonStateTransition();
		break;
	case enPlayerState_Walk:
		ProcessCommonStateTransition();
		break;
	case enPlayerState_Stop:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= SPEED_OF_MOVING_JUDGE 
		|| fabsf(m_moveSpeed.z) >= SPEED_OF_MOVING_JUDGE
		)
	{
		m_playerState = enPlayerState_Walk;
	}
	else {
		m_playerState = enPlayerState_Idle;
	}

}

void Player::PlayFootstepSound()
{
	// 鳴らす。
	m_footstepSound->Play(true);
	m_isFootstepSound = true;
}

void Player::StopFootstepSound()
{
	// 止める。
	m_footstepSound->Pause();
	m_isFootstepSound = false;
}

void Player::Render(RenderContext& rc)
{

}
