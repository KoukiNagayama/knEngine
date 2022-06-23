#pragma once
class Player;
class Bell : public IGameObject
{
public:
	Bell() {};
	~Bell() {};
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
	/// プレイヤーに合わせて移動。
	/// </summary>
	void MoveWithPlayer();
private:
	Player*				m_player = nullptr;				// プレイヤークラス。
	ModelRender			m_bellModel;					// ベルのモデル
	Vector3				m_position = Vector3::Zero;		// 座標
	
};

