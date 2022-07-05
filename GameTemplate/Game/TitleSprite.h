#pragma once
class TitleSprite : public IGameObject
{
public:
	TitleSprite() {};
	~TitleSprite() {};

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

private:
	SpriteRender				m_pressAToStartSprite;		// スプライト
	float						m_currentAlpha = 1.0f;		// 現在のα値
};

