#pragma once
namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		ModelRender() {};
		~ModelRender() {};

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="outlineType">輪郭線の種類。</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			int outlineType = 0
		);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// 描画処理
		/// </summary>
		void Draw();
		/// <summary>
		/// アニメーション再生
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補間時間(単位：秒)。</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		/// <returns>再生中ならばtrue。再生していないならばfalse</returns>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/// <summary>
		/// 座標、回転、拡大率を設定
		/// </summary>
		/// <param name="position">座標。</param>
		/// <param name="rotation">回転。</param>
		/// <param name="scale">拡大率。</param>
		void SetTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(position);
			SetRotation(rotation);
			SetScale(scale);
		}
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="position">座標。</param>
		void SetPosition(const Vector3& position) 
		{
			m_position = position;
		}
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="x">x座標。</param>
		/// <param name="y">y座標。</param>
		/// <param name="z">z座標。</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転。</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 拡大率を設定
		/// </summary>
		/// <param name="scale">拡大率。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 拡大率を設定
		/// </summary>
		/// <param name="x">x座標。</param>
		/// <param name="y">y座標。</param>
		/// <param name="z">z座標。</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}
		/// <summary>
		/// アニメーションの再生速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">アニメーションの再生速度。数値の分だけ速度が倍になる。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		/// <summary>
		/// アニメーションイベントの追加
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
	private:
		/// <summary>
		/// スケルトンの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// アニメーションの初期化
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);
		/// <summary>
		/// フォワードレンダリング用モデルを初期化
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="outlineType">輪郭線の種類。</param>
		void InitForwardRenderingModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			int outlineType = 0
		);
		/// <summary>
		/// GBuffer描画用のモデルを初期化
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		void InitModelOnRenderToGBuffer(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// ワールド行列の更新
		/// </summary>
		void UpdateWorldMatrix();

	private:
		Model									m_forwardRenderModel;							// フォワードレンダリングで描画されるモデル
		Model									m_renderToGBufferModel;							// RenderToGBufferで描画されるモデル
		Skeleton								m_skeleton;										// スケルトン
		Animation								m_animation;									// アニメーション
		AnimationClip*							m_animationClips;								// アニメーションクリップ
		int										m_numAnimationClips;							// アニメーションクリップの数
		Vector3									m_position = Vector3::Zero;						// 座標
		Quaternion								m_rotation = Quaternion::Identity;				// 回転
		Vector3									m_scale = Vector3::One;							// 拡大率
		float									m_animationSpeed = 1.0f;						// アニメーションの再生速度
	};

}

