#include "stdafx.h"
#include "Bell.h"
#include "Player.h"
#include "EdgeManagement.h"
#include "sound/SoundEngine.h"


namespace
{
	const float SHIFT_POSITION_FROM_CAMERA = 35.0f;				// カメラからずらす値
	const float MODEL_POSITION_UP = 25.0f;						// ベルを少し上にあげる
	const int	EDGE_COLOR_WHITE = 0;							// 輪郭線の色
	const int	NUM_ANIMATIONCLIP = 0;							// アニメーションクリップの数
	const int	MAX_INSTANCE = 0;								// インスタンスの最大数
	const int	BELL_SOUND_NUMBER_TO_REGISTER = 0;				// 登録するベルの音の番号
	const float VOLUME = 1.0f;									// 音量
	const float TIME_TO_BE_AVAILABLE_AGAIN_PER_SEC = 3.0f;		// 再度使用可能になる時間
	const float MIN_TIME_TO_BE_AVAILABLE_AGAIN_PER_SEC = 0.0f;	// 再度使用可能になる時間の最小値
	const int	BELL_SOUND_PRIORITY = 0;						// ベルの音源オブジェクトの実行優先順位
}

Bell::~Bell()
{
	if (m_bellSound != nullptr) {
		DeleteGO(m_bellSound);
	}
}

bool Bell::Start()
{
	// プレイヤーを検索。
	m_player = FindGO<Player>("player");

	// ベルのモデルを初期化。
	m_bellModel.Init(
		"Assets/modelData/item/bell.tkm",
		nullptr,
		NUM_ANIMATIONCLIP,
		enModelUpAxisZ,
		MAX_INSTANCE,
		EDGE_COLOR_WHITE,
		m_edgeManagement->GetEdgeControl()
	);

	// 波形データを登録する。
	g_soundEngine->ResistWaveFileBank(BELL_SOUND_NUMBER_TO_REGISTER, "Assets/sound/bell_low.wav");


	return true;
}

void Bell::Update()
{
	MoveWithPlayer();

	Ring();

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


	// ベルの回転を設定。
	m_bellModel.SetRotation(m_player->GetRotation());

	// ベルの座標を設定。
	m_bellModel.SetPosition(m_position);

}

void Bell::Ring()
{
	// タイマーを減らす。
	m_availableAgainTimer -= g_gameTime->GetFrameDeltaTime();

	// Bボタンを押したときにタイマーが0になっていれば
	if (g_pad[0]->IsTrigger(enButtonB) 
		&& m_availableAgainTimer <= MIN_TIME_TO_BE_AVAILABLE_AGAIN_PER_SEC
		) 
	{
		// 音源を鳴らす。
		m_bellSound = NewGO<SoundSource>(BELL_SOUND_PRIORITY);
		m_bellSound->Init(BELL_SOUND_NUMBER_TO_REGISTER);
		m_bellSound->SetVolume(VOLUME);
		m_bellSound->Play(false);

		// 鳴っている。
		m_isRing = true;
		// タイマーを再度初期化。
		m_availableAgainTimer = TIME_TO_BE_AVAILABLE_AGAIN_PER_SEC;
		return;
	}

	// 鳴っていない。
	m_isRing = false;
}


void Bell::Render(RenderContext& rc)
{
	m_bellModel.Draw();
}
