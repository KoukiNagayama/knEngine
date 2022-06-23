#pragma once
namespace nsK2EngineLow
{
	const int MAXIMUM_SIZE_OF_RECORDABLE_SOUND = 50;	// 登録可能な音源の最大個数
	/// <summary>
	/// 音源データ構造体
	/// </summary>
	struct SoundSourceData
	{
		Vector3		s_position = Vector3::Zero;		// 音源の座標
		float		s_range = 0.0f;					// 影響範囲
		float		s_rateByTime = 0.0f;			// 時間経過による影響率
		float		s_pad1[3];
		int			s_isSound = 0;					// 音源が鳴っているか
		int			s_pad2[3];
		
	};
	/// <summary>
	/// 音源データの定数バッファ用構造体
	/// </summary>
	struct CBSoundSourceData
	{
		SoundSourceData		s_soundSourceData[MAXIMUM_SIZE_OF_RECORDABLE_SOUND];	// 音源データ配列
		int					s_numSoundSource = 0;									// 登録された音源の個数
		float				s_sizeOfColorValue = 0.0f;
	};

	/// <summary>
	/// 音源による輪郭線制御クラス
	/// </summary>
	class EdgeControl
	{
	public:
		EdgeControl() {};
		~EdgeControl() {};
		
		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="number">登録する番号</param>
		/// <param name="position">座標</param>
		/// <param name="range">影響範囲</param>
		/// <param name="rateByTime">時間経過による影響率</param>
		void Init(int number,
			Vector3 position,
			float range,
			float rateByTime
		);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		///	登録された情報のクリア
		/// </summary>
		void Clear();
		/// <summary>
		/// 音源の座標を設定。
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="position">音源の座標</param>
		void SetPosition(int number, Vector3 position)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_position = position;
		}
		/// <summary>
		/// 音源の影響範囲を設定。
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="range">音源の影響範囲</param>
		void SetRange(int number, float range)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_range = range;
		}
		/// <summary>
		/// 音源が鳴っているかどうかを設定
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="isSound">音源が鳴っているか</param>
		void SetIsSound(int number, int isSound)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_isSound = isSound;
		}
		/// <summary>
		/// 音源の影響率
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="rate">影響率</param>
		void SetRate(int number, float rate)
		{
			m_cbSoundSourceData.s_soundSourceData[number].s_rateByTime = rate;
		}
		/// <summary>
		/// 音源データ用定数バッファ構造体を取得
		/// </summary>
		/// <returns>音源データ用定数バッファ構造体</returns>
		CBSoundSourceData& GetSoundSourceData()
		{
			return m_cbSoundSourceData;
		}
	private:
		enum SizeChangeState {
			enSizeChange_ValueIncrease,	// 値上昇	
			enSizeChange_ValueDecrease	// 値減少
		};
		CBSoundSourceData	m_cbSoundSourceData;								// 音源データの定数バッファ用構造体
		float				m_sizeOfColorValue = 0.0f;							// 色の数値の大きさ
		SizeChangeState		m_sizeChangeState = enSizeChange_ValueIncrease;		// 数値変更ステート
	};
	extern EdgeControl g_edgeControl;
}


