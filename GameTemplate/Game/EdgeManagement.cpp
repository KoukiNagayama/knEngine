#include "stdafx.h"
#include "EdgeManagement.h"
#include "Bell.h"
#include "Player.h"
#include "Enemy.h"
#include "TitleCamera.h"
#include "Game.h"
namespace
{
	const float BELL_RANGE = 2000.0f;					// ベルが輪郭線に影響を与える範囲
	const float PLAYER_FOOTSTEP_RANGE = 300.0f;			// プレイヤーの足音が輪郭線に影響を与える範囲
	const float ENEMY_FOOTSTEP_RANGE = 450.0f;			// エネミーの足音が輪郭線に影響を与える範囲
	const float ENEMY_SCREAM_RANGE = 1300.0f;			// エネミーの咆哮が輪郭線に影響を与える範囲
	const float TITLE_RANGE = 800.0f;					// タイトルで輪郭線に影響を与える範囲
	const float TITLE_OUTLINE_RATE = 1.0f;				// タイトルでの輪郭線の影響率

	const int IS_SOUND = 1;								// 音が鳴っている
	const int IS_NOT_SOUND = 0;							// 音が鳴っていない
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;		// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.00f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;			// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;			// 時間による影響率の最小値
}

void EdgeManagement::Init()
{
	m_game = FindGO<Game>("game");
	// ベルのインスタンスを検索。
	m_bell = FindGO<Bell>("bell");
	// プレイヤーのインスタンスを検索。
	m_player = FindGO<Player>("player");
	// エネミーのインスタンスを検索。
	m_enemy = FindGO<Enemy>("enemy");
	



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


	// エネミーの足音の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyFootstep,
		Vector3::Zero,
		ENEMY_FOOTSTEP_RANGE,
		m_rateByTimeOfEnemyFootstep
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep, IS_NOT_SOUND);


	// エネミーの咆哮の輪郭線への影響を初期化。
	m_edgeControl.Init(
		enSoundSourceData_EnemyScream,
		Vector3::Zero,
		ENEMY_SCREAM_RANGE,
		m_rateByTimeOfEnemyScream
	);
	m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream, IS_NOT_SOUND);

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
}

void EdgeManagement::Update()
{
	if (m_bell == nullptr || m_player == nullptr || m_enemy == nullptr) {
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

	// エネミーの足音。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyFootstep, m_enemy->GetPosition());

	// エネミーの咆哮。
	m_edgeControl.SetPosition(enSoundSourceData_EnemyScream, m_enemy->GetPosition());
}

void EdgeManagement::SpecifyIsSound()
{
	if (m_game->IsTitle()) {
		m_edgeControl.SetIsSound(enSoundSourceData_Title, IS_SOUND);
		return;
	}
	
	// ベル。
	SpecifyIsBellSounding();

	// プレイヤーの足音。
	SpecifyIsPlayerFootstepSounding();

	// エネミーの足音。
	SpecifyIsEnemyFootstepSounding();

	// エネミーの咆哮。
	SpecifyIsEnemyScreamSounding();
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

void EdgeManagement::SpecifyIsEnemyFootstepSounding()
{
	// 鳴っている時。
	if (m_enemy->IsMoving() == true && m_isLastEnemyFootstepSound == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep, IS_SOUND);
		m_isLastEnemyFootstepSound = true;
	}
	// 鳴っていない時。
	else if (m_enemy->IsMoving() == false && m_isLastEnemyFootstepSound == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyFootstep, IS_NOT_SOUND);
		m_isLastEnemyFootstepSound = false;
	}
}

void EdgeManagement::SpecifyIsEnemyScreamSounding()
{
	// 鳴っている時。
	if (m_enemy->IsScream() == true && m_isLastEnemyScream == false) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream, IS_SOUND);
		m_isLastEnemyScream = true;
	}
	// 鳴っていない時。
	else if (m_enemy->IsScream() == false && m_isLastEnemyScream == true) {
		m_edgeControl.SetIsSound(enSoundSourceData_EnemyScream, IS_NOT_SOUND);
		m_isLastEnemyScream = false;
	}
}

void EdgeManagement::SpecifyRateByTime()
{
	// ベル。
	CalcRateOfBell();
	// プレイヤーの足音。
	CalcRateOfPlayerFootstep();
	// エネミーの足音。
	CalcRateOfEnemyFootstep();
	// エネミーの咆哮。
	CalcRateOfEnemyScream();
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

void EdgeManagement::CalcRateOfEnemyFootstep()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy->IsMoving()) {
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
	m_edgeControl.SetRate(enSoundSourceData_EnemyFootstep, m_rateByTimeOfEnemyFootstep);
}

void EdgeManagement::CalcRateOfEnemyScream()
{
	// プレイヤーの足音が鳴っているならば。
	if (m_enemy->IsScream()) {
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
	m_edgeControl.SetRate(enSoundSourceData_EnemyScream, m_rateByTimeOfEnemyScream);
}