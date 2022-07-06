#pragma once
class Game;
class Score : public IGameObject
{
public:
	Score() ;
	~Score() {};

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
	/// 表示されるスコアの更新。
	/// </summary>
	void ScoreUpdate();
	/// <summary>
	/// 表示されるハイスコアの更新。
	/// </summary>
	void HighScoreUpdate();
private:


private:
	SpriteRender			m_scoreNumberSprite[3];			// スコアの数字
	SpriteRender			m_highScoreNumberSprite[3];		// ハイスコアの数字
	int						m_numberNum = -1;				// 数字の数（最初に計算）
	Game*					m_game = nullptr;				// ゲーム
	int						m_score = 0;					// スコア
	int						m_lastScore = 0;				// 直前のスコア
	int						m_highScore = 0;				// ハイスコア
	int						m_lastHighScore = 0;			// 直前のハイスコア
	SpriteRender			m_scoreSprite;					// SCOREのスプライト
	SpriteRender			m_highScoreSprite;				// HIGHSCOREのスプライト
};

