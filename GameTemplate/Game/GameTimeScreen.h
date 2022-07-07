#pragma once

/// <summary>
/// 残り時間を表示するクラス
/// 3桁対応
/// </summary>
class GameTimeScreen : public IGameObject
{
public:
	GameTimeScreen();
	~GameTimeScreen();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(){};
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);

	/// <summary>
	/// タイマーの状態を更新
	/// </summary>
	/// <param name="time">表示する時間</param>
	void GameTimerUpdate(const float time);

	/// <summary>
	/// 描画フラグを設定
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetDrawFlag(const bool flag) {
		m_drawFlag = flag;
	}

	/// <summary>
	/// 変数リセット
	/// </summary>
	void Reset() {
		m_colorChangeFlag = false;
		m_oldTime = -1;
	}

private:
	
	SpriteRender m_numberSprite[3];		// 数字画像
	int m_numberNum = -1;				// 数字の数（最初に計算）

	bool m_colorChangeFlag = false;		// 色変更フラグ

	int m_oldTime = -1;					// 旧時間

	bool m_drawFlag = true;				// 描画フラグ

};

