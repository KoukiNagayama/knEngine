#pragma once
class Result : public IGameObject
{
public:
	Result();
	~Result();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() {};
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="score">表示するスコア</param>
	void Init(const int score);

private:
	void ScoreUpdate();

	SpriteRender			m_resultSprite_BG;				// 背景画像
	SpriteRender			m_scoreNumberSprite[3];			// スコアの数字

	int						m_score = 0;					// スコア
	int						m_numberNum = -1;				// 数字の数（最初に計算）
	bool					m_initFlag = false;				// 初期化フラグ

};

