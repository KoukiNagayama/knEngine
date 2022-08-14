#pragma once
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	/// <summary>
	/// フェードインを開始。
	/// </summary>
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	/// <summary>
	/// フェードアウトを開始。
	/// </summary>
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/// <summary>
	/// フェードしているか。
	/// </summary>
	/// <returns>しているならtrue。していなければfalse。</returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	/// <summary>
	/// α値を取得。
	/// </summary>
	/// <returns>現在のα値。</returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
	/// <summary>
	/// フェードのスピードを設定。
	/// </summary>
	/// <param name="fadeSpeed">フェードのスピード。通常は1.0</param>
	void SetFadeSpeed(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
	}
private:
	enum EnState {
		enState_FadeIn,			// フェードイン中
		enState_FadeOut,		// フェードアウト中
		enState_Idle,			// アイドル中
	};
	SpriteRender		m_fadeSprite;				// フェードで使用するスプライト
	EnState				m_state = enState_Idle;		// ステート
	float				m_currentAlpha = 1.0f;		// 現在のα値
	float				m_fadeSpeed = 1.0f;			// フェードのスピード
};


