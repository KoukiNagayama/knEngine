#pragma once
class Score : public IGameObject
{
public:
	Score() {};
	~Score() {};
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
	/// <summary>
	/// 1の位を求める。
	/// </summary>
	void FindToOnesPlace();
	/// <summary>
	/// 10の位を求める。
	/// </summary>
	void FindToTensPlace();
	/// <summary>
	/// 100の位を求める。
	/// </summary>
	void FindToHundredthPlace();

private:
	SpriteRender			m_onesPlaceSprite;		// 1の位のスプライト
	SpriteRender			m_tensPlaceSprite;		// 10の位のスプライト
	SpriteRender			m_hundredthPlaceSprite;	// 100の位のスプライト

	int						m_score;				// スコア
	int						m_lastScore;			// 直前のスコア
};

