#pragma once
class GameOver : public IGameObject
{
public:
	GameOver() {};
	~GameOver() {};

	/// <summary>
	/// 開始処理。
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 処理は終了したか。
	/// </summary>
	/// <returns>終了していればtrue。していなければfalse。</returns>
	bool IsEndProcess()
	{
		return m_isEndGameOverProcess;
	}
private:
	/// <summary>
	/// フェードイン。
	/// </summary>
	void FadeIn();
	/// <summary>
	/// フェードアウト。
	/// </summary>
	void FadeOut();
	/// <summary>
	/// 血飛沫画像を設定。
	/// </summary>
	void BloodInit();
private:
	SpriteRender			m_gameOverSprite_BG;			// 背景画像（暗黒）
	SpriteRender			m_gameOverSprite_Text;			// ゲームオーバー文字
	SpriteRender			m_gameOverSprite_Blood;			// ゲームオーバー血飛沫

	float					m_currentAlpha = 1.0f;			// 現在のα値
	float					m_timerToFadeOut = 0.0f;		// フェードアウトに移行するまでの時間
	Vector3					m_gameOverSpritePosition;		// ゲームオーバーのスプライトを表示する座標			
	Vector3					m_gameOverSpriteScale;			// ゲームオーバーのスプライトの拡大率
	bool					m_isFadeOut = false;			// フェードアウトするか
	bool					m_isEndGameOverProcess = false; // ゲームオーバーの処理をすべて終了したか。

	int						m_spriteShakeNo = 0;			// シェイク番号
	int						m_shakeNum = -1;				// シェイク最大数
	bool					m_isDrawText = true;			// ゲームオーバーをテキストを描画

	float					m_bloodTimer_Start = 0.0f;		// 血飛沫開始タイマー
	float					m_bloodTimer_Wait = 0.0f;		// 血飛沫更新タイマー
	bool					m_isBloodPlaying = false;		// 血飛沫再生中
	int						m_bloodNo = 0;					// 血飛沫番号
};

