#include "stdafx.h"
#include "GameOverEffect.h"
#include "Player.h"
#include "Enemy.h"
#include "GameCamera.h"

namespace
{
	const float CAMERA_TARGET_Y_UP = 280.0f;					// 最終注視点のY補正量
	const float CAMERA_TARGET_MOVE_TIME = 8.0f;					// ターゲットを向く時間

	const Vector3 CAMERA_CATCH_SHAKE = { 40.0f,40.0f,40.0f };	// 捕まった時のシェイク量
	const float CAMERA_CATCH_SHAKE_TIME = 0.24f;				// 捕まった時のシェイク時間
	const float CAMERA_CATCH_MUL = 0.63f;						// 捕まった時のシェイク量に乗算する（だんだん小さくする）

	const Vector3 CAMERA_ALWAYS_SHAKE = { 3.0f,3.0f,3.0f };		// 常時シェイク量

	const int	CATCH_SOUND_NUMBER_TO_REGISTER = 5;				// 登録する捕獲の音の番号
	const float CATCH_VOLUME = 1.5f;							// 捕獲の音量
	const int	HORROR_SOUND_NUMBER_TO_REGISTER = 6;			// 登録するホラーな音の番号
	const float HORROR_VOLUME = 1.5f;							// ホラーの音量

}

void GameOverEffect::StartGameOverEffect() {

	m_isPlayEffect = true;

	// プレイヤーとカメラを移動不能にする
	m_player->SetIsMovable(false);
	m_gameCamera->SetIsMovable(false);

	// 移動量を計算
	Vector3 target_pos = m_enemy->GetPosition();
	target_pos.y += CAMERA_TARGET_Y_UP;
	m_cameraTargetMove = (target_pos - g_camera3D->GetPosition()) / CAMERA_TARGET_MOVE_TIME;
	// 次フレームの注視点を保存しておく
	m_cameraNextTarget = g_camera3D->GetTarget() + m_cameraTargetMove;

	// 移動タイマーを初期化
	m_cameraTargetMoveTimer = CAMERA_TARGET_MOVE_TIME;

	// シェイクタイマーを初期化
	m_cameraCatchShakeTimer = CAMERA_CATCH_SHAKE_TIME;

	// シェイク量を初期化
	m_cameraShake = CAMERA_CATCH_SHAKE;

	// 捕獲の音の登録。
	g_soundEngine->ResistWaveFileBank(CATCH_SOUND_NUMBER_TO_REGISTER, "Assets/sound/gameover/catch.wav");
	// ホラーな音の登録。
	g_soundEngine->ResistWaveFileBank(HORROR_SOUND_NUMBER_TO_REGISTER, "Assets/sound/gameover/gameover1.wav");

	// 効果音関連の初期化
	m_catchSound = nullptr;
	m_horrorSound = nullptr;

}

void GameOverEffect::PlayGameOverEffect() {

	// 捕まった瞬間大きくシェイク
	if (m_cameraCatchShakeTimer > 0.0f) {
		m_cameraCatchShakeTimer -= g_gameTime->GetFrameDeltaTime();
		// シェイク量は適当に乱数
		Vector3 shake = {
			(float)((rand() % ((int)m_cameraShake.x * 2)) - (int)m_cameraShake.x),
			(float)((rand() % ((int)m_cameraShake.y * 2)) - (int)m_cameraShake.y),
			(float)((rand() % ((int)m_cameraShake.z * 2)) - (int)m_cameraShake.z)
		};
		// 小さくする
		m_cameraShake *= CAMERA_CATCH_MUL;
		// 0になるとクラッシュしてしまう…
		m_cameraShake.x = max(m_cameraShake.x, 1.0f);
		m_cameraShake.y = max(m_cameraShake.y, 1.0f);
		m_cameraShake.z = max(m_cameraShake.z, 1.0f);
		// 設定する
		g_camera3D->SetTarget(m_cameraNextTarget + shake);
	}
	else {

		// カメラをターゲットに向かせる
		if (m_cameraTargetMoveTimer > 0.0f) {
			m_cameraTargetMoveTimer -= g_gameTime->GetFrameDeltaTime();
			// 設定
			g_camera3D->SetTarget(m_cameraNextTarget);
			// 次フレームの注視点を保存しておく
			m_cameraNextTarget = g_camera3D->GetTarget() + m_cameraTargetMove;
		}

		// 常時微妙にシェイク
		Vector3 shake = {
			(float)((rand() % ((int)CAMERA_ALWAYS_SHAKE.x * 2)) - (int)CAMERA_ALWAYS_SHAKE.x),
			(float)((rand() % ((int)CAMERA_ALWAYS_SHAKE.y * 2)) - (int)CAMERA_ALWAYS_SHAKE.y),
			(float)((rand() % ((int)CAMERA_ALWAYS_SHAKE.z * 2)) - (int)CAMERA_ALWAYS_SHAKE.z)
		};
		// 設定する
		g_camera3D->SetTarget(g_camera3D->GetTarget() + shake);
	}

	// 効果音
	
	// 捕獲の音源を鳴らす
	if (m_catchSound == nullptr) {
		m_catchSound = NewGO<SoundSource>(0);
		m_catchSound->Init(CATCH_SOUND_NUMBER_TO_REGISTER);
		m_catchSound->SetVolume(CATCH_VOLUME);
		m_catchSound->Play(false);
	}

	// ホラーの音源を鳴らす
	if (m_horrorSound == nullptr) {
		m_horrorSound = NewGO<SoundSource>(0);
		m_horrorSound->Init(HORROR_SOUND_NUMBER_TO_REGISTER);
		m_horrorSound->SetVolume(HORROR_VOLUME);
		m_horrorSound->Play(false);
	}

}