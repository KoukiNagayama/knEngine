#pragma once

class Player;
class Enemy;
class GameCamera;

/// <summary>
/// ゲームオーバーエフェクト
/// ゲームオーバーが確定したらゲームオーバーになるまでの間だけ実行される
/// </summary>
class GameOverEffect
{
public:

	/// <summary>
	/// 初期化
	/// （ポインタ渡し）
	/// </summary>
	/// <param name="pl">プレイヤーのポインタ</param>
	/// <param name="en">敵のポインタ</param>
	/// <param name="gc">ゲームカメラのポインタ</param>
	void Init(Player* pl, Enemy* en, GameCamera* gc) {
		m_player = pl;
		m_enemy = en;
		m_gameCamera = gc;
	}

	/// <summary>
	/// エフェクトを開始する
	/// </summary>
	void StartGameOverEffect();

	/// <summary>
	/// エフェクトを終わらせる
	/// </summary>
	void EndGameOverEffect() {
		m_isPlayEffect = false;
	}

	/// <summary>
	/// ゲームオーバーエフェクトを再生
	/// </summary>
	void PlayGameOverEffect();

	/// <summary>
	/// ゲームオーバーエフェクト中？
	/// </summary>
	/// <returns>trueならゲームオーバーエフェクト中</returns>
	bool IsPlayEffect() {
		return m_isPlayEffect;
	}

	/// <summary>
	/// ホラー音を停止させる
	/// （演出用）
	/// </summary>
	void StopHorrorSound() {
		m_horrorSound->Stop();
	}

private:

	bool m_isPlayEffect = false;					// ゲームオーバーエフェクト再生フラグ

	Player* m_player = nullptr;						// プレイヤー
	Enemy* m_enemy = nullptr;						// エネミー
	GameCamera* m_gameCamera = nullptr;				// ゲームカメラ
	SoundSource* m_catchSound = nullptr;			// 捕獲の音源
	SoundSource* m_horrorSound = nullptr;			// ホラーの音源

	// ここから演出用メンバ変数
	Vector3 m_cameraNextTarget = Vector3::Zero;		// 次フレームの注視点を保存しておく

	Vector3 m_cameraTargetMove = Vector3::Zero;		// カメラの移動量
	float m_cameraTargetMoveTimer = 0.0f;			// カメラを敵に向けるタイマー

	Vector3 m_cameraShake = Vector3::Zero;			// カメラシェイク量
	float m_cameraCatchShakeTimer = 0.0f;			// 捕まった瞬間大きくシェイクする用タイマー

};

