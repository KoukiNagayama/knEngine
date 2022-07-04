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
private:
	SpriteRender			m_gameOverSprite;				// ゲームオーバー時のスプライト
	float					m_currentAlpha = 0.0f;			// 現在のα値
	float					m_timerToFadeOut = 0.0f;		// フェードアウトに移行するまでの時間
	Vector3					m_gameOverSpritePosition;		// ゲームオーバーのスプライトを表示する座標			
	Vector3					m_gameOverSpriteScale;			// ゲームオーバーのスプライトの拡大率
	bool					m_isFadeOut = false;			// フェードアウトするか
	bool					m_isEndGameOverProcess = false; // ゲームオーバーの処理をすべて終了したか。
};

