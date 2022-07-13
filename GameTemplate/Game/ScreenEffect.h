#pragma once
class Enemy;
class Player;
class ScreenEffect : public IGameObject
{
public:
	/// <summary>
	/// 開始処理。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト。</param>
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// α値を変更する。
	/// </summary>
	void ChangeAlphaValue();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// ステート遷移
	/// </summary>
	void StateTransitionProccesing();
	void FadeIn();
	void FadeOut();
private:
	enum EnScreenEffectState {
		enState_FadeIn,
		enState_Fix,
		enState_FadeOut,
		enState_Idle
	};
	SpriteRender			m_screenEffect;								// スクリーンエフェクトのスプライト
	float					m_currentAlpha = 0.0f;						// 現在のα値
	Enemy*					m_enemy1 = nullptr;
	Enemy*					m_enemy2 = nullptr;
	float					m_shortestDistanceToEnemy = 10000.0f;		// エネミーとの最短距離
	Vector3					m_playerPos;
	Player*					m_player = nullptr;
	EnScreenEffectState		m_enScreenEffectState = enState_Idle;
	float					m_alphaValueMultiplier = 0.0f;
};

