#include "stdafx.h"
#include "EdgeManagement.h"
#include "Bell.h"
#include "Player.h"
#include "Enemy.h"
#include "TitleCamera.h"
#include "Game.h"
namespace
{
	const float BELL_RANGE = 800.0f;						// ベルが輪郭線に影響を与える範囲
	const float PLAYER_FOOTSTEP_RANGE = 300.0f;				// プレイヤーの足音が輪郭線に影響を与える範囲
	const float ENEMY_FOOTSTEP_RANGE = 450.0f;				// エネミーの足音が輪郭線に影響を与える範囲
	const float ENEMY_SCREAM_RANGE = 1300.0f;				// エネミーの咆哮が輪郭線に影響を与える範囲
	const float TITLE_RANGE = 800.0f;						// タイトルで輪郭線に影響を与える範囲
	const float TITLE_OUTLINE_RATE = 1.0f;					// タイトルでの輪郭線の影響率
	const float NO_TITLE_OUTLINE_RATE = 0.0f;				// タイトルでの輪郭線の影響率なし
	const int IS_SOUND = 1;									// 音が鳴っている
	const int IS_NOT_SOUND = 0;								// 音が鳴っていない
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;			// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.0065f;		// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;				// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;				// 時間による影響率の最小値
}

void EdgeManagement::Init()
{
	m_game = FindGO<Game>("game");
	// ベルのインスタンスを検索。
	m_bell = FindGO<Bell>("bell");
	// プレイヤーのインスタンスを検索。
	m_player = FindGO<Player>("player");

	// エネミーを検索
	const auto& enemys = FindGOs<Enemy>("enemy");
	// エネミーの数
	const int size = enemys.size();
	
	for (int i = 0; i < size; i++) {
		switch (i) {
		case 0:
			m_enemy1 = enemys[i];
			break;
		case 1:
			m_enemy2 = enemys[i];
			break;
		case 2:
			m_enemy3 = enemys[i];
			break;
		}
	}
	

	// ベルの輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_Bell,
		Vector3::Zero,
		BELL_RANGE,
		m_rateByTimeOfBell
	);
	m_edgeControl.SetIsSound(enSoundSourceData_Bell, IS_NOT_SOUND);


	// プレイヤーの足音の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_PlayerFootstep,
		Vector3::Zero,
		PLAYER_FOOTSTEP_RANGE,
		m_rateByTimeOfPlayerFootstep
	);
	m_edgeControl.SetIsSound(enSoundSourceData_PlayerFootstep, IS_NOT_SOUND);


	// エネミー1の足音の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep1,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep1, IS_NOT_SOUND);


	// エネミー1の咆哮の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream1,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream1, IS_NOT_SOUND);

	// タイトルの時は。
	if (m_game->IsTitle()) {
		// タイトルカメラの情報を取得する。
		m_titleCamera = FindGO<TitleCamera>("titleCamera");
		// 輪郭線描画の中心をタイトルカメラの座標とする。
		m_positionOfCenterInTitle = m_titleCamera->GetPosition();
	}

	// タイトルの時の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_Title,
		m_positionOfCenterInTitle,
		TITLE_RANGE,
		TITLE_OUTLINE_RATE
	);
	m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_NOT_SOUND);

	// エネミー2の足音の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep2,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep2
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep2, IS_NOT_SOUND);

	// エネミー2の咆哮の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream2,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream2
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream2, IS_NOT_SOUND);

	// エネミー3の足音の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep3,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep3
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep3, IS_NOT_SOUND);

	// エネミー3の咆哮の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream3,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream3
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream3, IS_NOT_SOUND);

}

void EdgeManagement::Update()
{
	if (m_bell == nullptr || m_player == nullptr || m_enemy1 == nullptr || m_enemy2 == nullptr) {
		return;
	}

	// 音が鳴っているかを指定。
	SpecifyIsSound();

	if (m_game->IsInGame() == false) {
		return;
	}

	// 座標を指定。
	SpecifyPosition();

	// 時間による影響率を指定。
	SpecifyRateByTime();
}

void EdgeManagement::Clear()
{
	// 登録情報のクリア。
	m_edgeControl.Clear();
}

void EdgeManagement::SpecifyPosition()
{
	// ベル。
	m_edgeControl.SetPosition(enSoundSourceData_Bell, m_bell->GetPosition());

	// プレイヤーの足音。
	m_edgeControl.SetPosition(enSoundSourceData_PlayerFootstep, m_player->GetPosition());

	// エネミー1の足音。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep1, m_enemy1->GetPosition());

	// エネミー1の咆哮。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream1, m_enemy1->GetPosition());

	// エネミー2の足音。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep2, m_enemy2->GetPosition());
	
	// エネミー2の咆哮。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream2, m_enemy2->GetPosition());

	// エネミー3の足音。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep3, m_enemy3->GetPosition());

	// エネミー3の咆哮。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream3, m_enemy3->GetPosition());
}

void EdgeManagement::SpecifyIsSound()
{
	// タイトル。
	if (m_game->IsTitle()) {
		m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_SOUND);
		return;
	}

	m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_NOT_SOUND);
	
	// ベル。
	SpecifyIsBellSounding();

	// プレイヤーの足音。
	SpecifyIsPlayerFootstepSounding();

	// エネミー1の足音。
	SpecifyIsEnemyFootstep1Sounding();

	// エネミー1の咆哮。
	SpecifyIsEnemyScream1Sounding();

	// エネミー2の足音。
	SpecifyIsEnemyFootstep2Sounding();

	// エネミー2の咆哮。
	SpecifyIsEnemyScream2Sounding();

	// エネミー3の足音。
	SpecifyIsEnemyFootstep3Sounding();

	// エネミー3の咆哮。
	SpecifyIsEnemyScream3Sounding();
}

void EdgeManagement::SpecifyIsBellSounding()
{
	// 鳴っている時。
	if (m_bell->IsBellRinging() == true && m_isLastBellSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_Bell, IS_SOUND);
		m_isLastBellSound = true;
	}
	// 鳴っていない時。
	else if (m_bell->IsBellRinging() == false && m_isLastBellSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_Bell, IS_NOT_SOUND);
	}
}

void EdgeManagement::SpecifyIsPlayerFootstepSounding()
{
	// 鳴っている時。
	if (m_player->IsPlayerFootstepSound() == true && m_isLastPlayerFootstepSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_PlayerFootstep, IS_SOUND);
		m_isLastPlayerFootstepSound = true;
	}
	// 鳴っていない時。
	else if (m_player->IsPlayerFootstepSound() == false && m_isLastPlayerFootstepSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_PlayerFootstep, IS_NOT_SOUND);
		m_isLastPlayerFootstepSound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyFootstep1Sounding()
{
	// 鳴っている時。
	if (m_enemy1->IsMoving() == true && m_isLastEnemyFootstepSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep1, IS_SOUND);
		m_isLastEnemyFootstepSound = true;
	}
	// 鳴っていない時。
	else if (m_enemy1->IsMoving() == false && m_isLastEnemyFootstepSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep1, IS_NOT_SOUND);
		m_isLastEnemyFootstepSound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScream1Sounding()
{
	// 鳴っている時。
	if (m_enemy1->IsScream() == true && m_isLastEnemyScream == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream1, IS_SOUND);
		m_isLastEnemyScream = true;
	}
	// 鳴っていない時。
	else if (m_enemy1->IsScream() == false && m_isLastEnemyScream == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream1, IS_NOT_SOUND);
		m_isLastEnemyScream = false;
	}
}

void EdgeManagement::SpecifyIsEnemyFootstep2Sounding()
{
	// 鳴っている時。
	if (m_enemy2->IsMoving() == true && m_isLastEnemyFootstep2Sound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep2, IS_SOUND);
		m_isLastEnemyFootstep2Sound = true;
	}
	// 鳴っていない時。
	else if (m_enemy2->IsMoving() == false && m_isLastEnemyFootstep2Sound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep2, IS_NOT_SOUND);
		m_isLastEnemyFootstep2Sound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScream2Sounding()
{
	// 鳴っている時。
	if (m_enemy2->IsScream() == true && m_isLastEnemyScream2 == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream2, IS_SOUND);
		m_isLastEnemyScream2 = true;
	}
	// 鳴っていない時。
	else if (m_enemy2->IsScream() == false && m_isLastEnemyScream2 == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream2, IS_NOT_SOUND);
		m_isLastEnemyScream2 = false;
	}
}

void EdgeManagement::SpecifyIsEnemyFootstep3Sounding()
{
	// 鳴っている時。
	if (m_enemy3->IsMoving() == true && m_isLastEnemyFootstep3Sound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep3, IS_SOUND);
		m_isLastEnemyFootstep3Sound = true;
	}
	// 鳴っていない時。
	else if (m_enemy3->IsMoving() == false && m_isLastEnemyFootstep3Sound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep3, IS_NOT_SOUND);
		m_isLastEnemyFootstep3Sound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScream3Sounding()
{
	// 鳴っている時。
	if (m_enemy3->IsScream() == true && m_isLastEnemyScream3 == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream3, IS_SOUND);
		m_isLastEnemyScream3 = true;
	}
	// 鳴っていない時。
	else if (m_enemy3->IsScream() == false && m_isLastEnemyScream3 == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream3, IS_NOT_SOUND);
		m_isLastEnemyScream3 = false;
	}
}

void EdgeManagement::SpecifyRateByTime()
{
	// ベル。
	CalcRateOfBell();
	// プレイヤーの足音。
	CalcRateOfPlayerFootstep();
	// エネミー1の足音。
	CalcRateOfEnemyFootstep1();
	// エネミー1の咆哮。
	CalcRateOfEnemyScream1();
	// エネミー2の足音。
	CalcRateOfEnemyFootstep2();
	// エネミー2の咆哮。
	CalcRateOfEnemyScream2();
	// エネミー3の足音。
	CalcRateOfEnemyFootstep3();
	// エネミー3の咆哮。
	CalcRateOfEnemyScream3();
	// タイトル。
	m_edgeControl.SetRate(enSoundSourceData_Title, NO_TITLE_OUTLINE_RATE);
}

void EdgeManagement::CalcRateOfBell()
{
	// ベルが鳴っているならば。
	if (m_bell->IsBellRinging()) {
		// ベルの影響率を上げるようにする。
		m_isRateUpOfBell = true;
	}
	// ベルの影響率を上げるようになっているならば。
	if (m_isRateUpOfBell) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfBell < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfBell += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfBell = false;
		}
	}
	// ベルの影響率が0より大きいならば
	else if (m_rateByTimeOfBell > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfBell -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfBell < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfBell = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// ベルの影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_Bell, m_rateByTimeOfBell);
}

void EdgeManagement::CalcRateOfPlayerFootstep()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_player->IsPlayerFootstepSound()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfPlayerFootstep = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfPlayerFootstep) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfPlayerFootstep < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfPlayerFootstep += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfPlayerFootstep = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfPlayerFootstep > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfPlayerFootstep -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfPlayerFootstep < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfPlayerFootstep = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_PlayerFootstep, m_rateByTimeOfPlayerFootstep);
}

void EdgeManagement::CalcRateOfEnemyFootstep1()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy1->IsMoving()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfEnemyFootstep = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfEnemyFootstep) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfEnemyFootstep < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfEnemyFootstep += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfEnemyFootstep = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfEnemyFootstep > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfEnemyFootstep -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyFootstep < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyFootstep = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep1, m_rateByTimeOfEnemyFootstep);
}

void EdgeManagement::CalcRateOfEnemyScream1()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy1->IsScream()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfEnemyScream = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfEnemyScream) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfEnemyScream < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfEnemyScream += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfEnemyScream = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfEnemyScream > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfEnemyScream -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyScream < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyScream = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream1, m_rateByTimeOfEnemyScream);
}

void EdgeManagement::CalcRateOfEnemyFootstep2()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy2->IsMoving()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfEnemyFootstep2 = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfEnemyFootstep2) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfEnemyFootstep2 < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfEnemyFootstep2 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfEnemyFootstep2 = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfEnemyFootstep2 > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfEnemyFootstep2 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyFootstep2 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyFootstep2 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep2, m_rateByTimeOfEnemyFootstep2);
}

void EdgeManagement::CalcRateOfEnemyScream2()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy2->IsScream()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfEnemyScream2 = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfEnemyScream2) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfEnemyScream2 < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfEnemyScream2 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfEnemyScream2 = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfEnemyScream2 > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfEnemyScream2 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyScream2 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyScream2 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream2, m_rateByTimeOfEnemyScream2);
}

void EdgeManagement::CalcRateOfEnemyFootstep3()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy3->IsMoving()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfEnemyFootstep3 = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfEnemyFootstep3) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfEnemyFootstep3 < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfEnemyFootstep3 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfEnemyFootstep3 = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfEnemyFootstep3 > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfEnemyFootstep3 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyFootstep3 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyFootstep3 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep3, m_rateByTimeOfEnemyFootstep3);
}

void EdgeManagement::CalcRateOfEnemyScream3()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy3->IsScream()) {
		// プレイヤーの足音の影響率を上げるようにする。
		m_isRateUpOfEnemyScream3 = true;
	}
	// プレイヤーの足音を上げるようになっているならば。
	if (m_isRateUpOfEnemyScream3) {
		// 影響率が最大値未満であれば。
		if (m_rateByTimeOfEnemyScream3 < RATE_BY_TIME_MAX_VALUE) {
			// 影響率を上げる。
			m_rateByTimeOfEnemyScream3 += EDGE_FADE_IN_DELTA_VALUE;
		}
		// 影響率が最大値を超えているならば。
		else {
			// 影響率を上げないようにする。
			m_isRateUpOfEnemyScream3 = false;
		}
	}
	// プレイヤーの足音の影響率が0より大きいならば
	else if (m_rateByTimeOfEnemyScream3 > RATE_BY_TIME_MIN_VALUE) {
		// 影響率を下げる。
		m_rateByTimeOfEnemyScream3 -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTimeOfEnemyScream3 < RATE_BY_TIME_MIN_VALUE) {
			m_rateByTimeOfEnemyScream3 = RATE_BY_TIME_MIN_VALUE;
		}
	}
	// プレイヤーの足音の影響率を設定。
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream3, m_rateByTimeOfEnemyScream3);
}

