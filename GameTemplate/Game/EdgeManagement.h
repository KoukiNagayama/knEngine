#pragma once
class Bell;
class Player;
class Enemy;
class TitleCamera;
class Game;
class EdgeManagement
{
public:
	EdgeManagement() {};
	~EdgeManagement() {};

	/// <summary>
	/// 初期化処理。
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// クリア。
	/// </summary>
	void Clear();
	/// <summary>
	/// 輪郭線制御の情報を取得。
	/// </summary>
	/// <returns>輪郭線制御の情報</returns>
	EdgeControl* GetEdgeControl()
	{
		return &m_edgeControl;
	}
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="number">音源の番号。</param>
	/// <param name="position">設定する座標。</param>
	void SetPosition(int number, Vector3 position) 
	{
		m_edgeControl.SetPosition(number, position);
	}
	/// <summary>
	/// 音源が鳴っているかどうかを設定。
	/// </summary>
	/// <param name="number">音源の番号。</param>
	/// <param name="isSound">音源が鳴っているか。1は鳴っている。0は鳴っていない。</param>
	void SetIsSound(int number, int isSound)
	{
		m_edgeControl.SetIsSound(number, isSound);
	}
	/// <summary>
	/// 時間経過による影響率を設定。
	/// </summary>
	/// <param name="number">音源の番号。</param>
	/// <param name="rateByTime">時間経過による影響率。</param>
	void SetRateByTime(int number, float rateByTime)
	{
		m_edgeControl.SetRate(number, rateByTime);
	}
private:
	/// <summary>
	/// 座標を指定。
	/// </summary>
	void SpecifyPosition();
	/// <summary>
	/// 時間による影響率を指定。
	/// </summary>
	void SpecifyRateByTime();
	/// <summary>
	/// 音が鳴っているかを指定。
	/// </summary>
	void SpecifyIsSound();
	/// <summary>
	/// ベルの影響率を計算。
	/// </summary>
	void CalcRateOfBell();
	/// <summary>
	/// プレイヤーの足音の影響率を計算。
	/// </summary>
	void CalcRateOfPlayerFootstep();
	/// <summary>
	/// エネミー1の足音の影響率を計算。
	/// </summary>
	void CalcRateOfEnemyFootstep1();
	/// <summary>
	/// エネミー1の咆哮の影響率を計算。
	/// </summary>
	void CalcRateOfEnemyScream1();
	/// <summary>
	/// エネミー2の足音の影響率を計算。
	/// </summary>
	void CalcRateOfEnemyFootstep2();
	/// <summary>
	/// エネミー2の咆哮の影響率を計算。
	/// </summary>
	void CalcRateOfEnemyScream2();
	/// <summary>
	/// ベルの音が鳴っているか指定。
	/// </summary>
	void SpecifyIsBellSounding();
	/// <summary>
	/// プレイヤーの足音が鳴っているか指定。
	/// </summary>
	void SpecifyIsPlayerFootstepSounding();
	/// <summary>
	/// エネミー1の足音が鳴っているか指定。
	/// </summary>
	void SpecifyIsEnemyFootstep1Sounding();
	/// <summary>
	/// エネミー1の咆哮が鳴っているか指定。
	/// </summary>
	void SpecifyIsEnemyScream1Sounding();
	/// <summary>
	/// エネミー2の足音が鳴っているか指定。
	/// </summary>
	void SpecifyIsEnemyFootstep2Sounding();
	/// <summary>
	/// エネミー2の咆哮が鳴っているか指定。
	/// </summary>
	void SpecifyIsEnemyScream2Sounding();
private:
	enum EnSoundSourceData
	{
		enSoundSourceData_Bell,				// ベル
		enSoundSourceData_PlayerFootstep,	// プレイヤーの足音
		enSoundSourceData_EnemyFootstep1,	// エネミー1の足音
		enSoundSourceData_EnemyScream1,		// エネミー1の咆哮
		enSoundSourceData_Title,			// タイトル
		//enSoundSourceData_BellAfterTitle,	// タイトルでのベル
		enSoundSourceData_EnemyFootstep2,	// エネミー2の足音
		enSoundSourceData_EnemyScream2,		// エネミー2の咆哮
		enSoundSourceData_Num				// 音の個数
	};


	EdgeControl				m_edgeControl;											// 輪郭線制御
	Bell*					m_bell = nullptr;										// ベル
	Player*					m_player = nullptr;										// プレイヤー
	Enemy*					m_enemy1 = nullptr;										// エネミー1
	Enemy*					m_enemy2 = nullptr;										// エネミー2
	TitleCamera*			m_titleCamera = nullptr;								// タイトルカメラ
	float					m_rateByTimeOfBell = 0.0f;								// ベルの時間経過による影響率
	bool					m_isRateUpOfBell = false;								// ベルの影響率を上げるか
	bool					m_isLastBellSound = false;								// ベルは直前は鳴っていたか 
	float					m_rateByTimeOfPlayerFootstep = 0.0f;					// プレイヤーの足音の時間経過による影響率
	bool					m_isRateUpOfPlayerFootstep = false;						// プレイヤーの足音の影響率を上げるか
	bool					m_isLastPlayerFootstepSound = false;					// プレイヤーの足音は直前は鳴っていたか
	float					m_rateByTimeOfEnemyFootstep = 0.0f;						// エネミーの足音の時間経過による影響率
	bool					m_isRateUpOfEnemyFootstep = false;						// エネミーの足音の影響率を上げるか
	bool					m_isLastEnemyFootstepSound = false;						// エネミーの足音は直前は鳴っていたか
	float					m_rateByTimeOfEnemyScream = 0.0f;						// エネミーの咆哮の時間経過による影響率
	bool					m_isRateUpOfEnemyScream = false;						// エネミーの咆哮の影響率を上げるか
	bool					m_isLastEnemyScream = false;							// エネミーの咆哮は直前は鳴っていたか
	Vector3					m_positionOfCenterInTitle = Vector3::Zero;				// タイトルの時の中心の座標
	Game*					m_game = nullptr;										// ゲーム
	float					m_rateByTimeOfBellAfterTitle = 1.0f;
	float					m_rateByTimeOfEnemyFootstep2 = 0.0f;
	bool					m_isRateUpOfEnemyFootstep2 = false;
	bool					m_isLastEnemyFootstep2Sound = false;
	float					m_rateByTimeOfEnemyScream2 = 0.0f;						// エネミー2の咆哮の時間経過による影響率
	bool					m_isRateUpOfEnemyScream2 = false;						// エネミー2の咆哮の影響率を上げるか
	bool					m_isLastEnemyScream2 = false;							// エネミー2の咆哮は直前は鳴っていたか

};

