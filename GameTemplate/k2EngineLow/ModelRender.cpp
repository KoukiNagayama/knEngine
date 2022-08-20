#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "RenderingEngine.h"
#include "EdgeControl.h"

namespace
{
	const int NORMAL_MODEL_NUM = 0;			// 通常モデル
	const int ENEMY_MODEL_NUM = 1;			// エネミーモデル
	const int COLLECT_ITEM_MODEL_NUM = 2;	// 回収アイテムモデル
}

namespace nsK2EngineLow
{
	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		int maxInstance,
		int outlineType,
		EdgeControl* edgeControl
	)
	{
		//m_edgeColor = outlineType;
		m_edgeControl = edgeControl;
		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips);
		// フォワードレンダリング用のモデルを初期化。
		InitForwardRenderingModel(filePath, enModelUpAxis, outlineType);
		// GBuffer描画用のモデルを初期化
		InitModelOnRenderToReservePass(filePath, enModelUpAxis);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips,
		int numAnimationClips)
	{
		m_numAnimationClips = numAnimationClips;
		m_animationClips = animationClips;

		if (m_animationClips != nullptr) {
			// アニメーションを初期化
			m_animation.Init(
				m_skeleton,					// アニメーションを流し込むスケルトン
				m_animationClips,			// アニメーションクリップ
				m_numAnimationClips			// アニメーションの数
			);
		}
	}

	void ModelRender::InitForwardRenderingModel(const char* filePath,
		EnModelUpAxis enModelUpAxis,
		int outlineType
	)
	{
		ModelInitData modelInitData;
		// tkmファイルのファイルパスを指定する。
		modelInitData.m_tkmFilePath = filePath;
		// シェーダーファイルのファイルパスを指定する。
		if (outlineType == NORMAL_MODEL_NUM) {
			modelInitData.m_fxFilePath = "Assets/shader/normalModel.fx";
		}
		else if (outlineType == ENEMY_MODEL_NUM) {
			modelInitData.m_fxFilePath = "Assets/shader/enemy.fx";
		}
		else if (outlineType == COLLECT_ITEM_MODEL_NUM) {
			modelInitData.m_fxFilePath = "Assets/shader/collectItem.fx";
		}
		// エントリーポイントを指定する。
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// カラーバッファのフォーマットを指定する。
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// 各種マップを拡張SRVに設定する。
		modelInitData.m_expandShaderResoruceView[0] = &g_renderingEngine.GetDepthValue().GetRenderTargetTexture();
		modelInitData.m_expandShaderResoruceView[1] = &g_renderingEngine.GetWorldPos().GetRenderTargetTexture();
		modelInitData.m_expandShaderResoruceView[2] = &g_renderingEngine.GetNormal().GetRenderTargetTexture();
		
		// モデルの上方向を指定する。
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// 音源データを定数バッファとして設定する
		modelInitData.m_expandConstantBuffer = (void *)&m_edgeControl->GetSoundSourceData();
		modelInitData.m_expandConstantBufferSize = sizeof(m_edgeControl->GetSoundSourceData());
		// 作成した初期化データをもとにモデルを初期化する。
		m_forwardRenderModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnRenderToReservePass(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;
		// モデルの上方向を指定する
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// シェーダーファイルのファイルパスを指定する
		modelInitData.m_fxFilePath = "Assets/shader/renderToReservePassForModel.fx";
		// エントリーポイントを指定する。
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// tkmファイルのファイルパスを指定する
		modelInitData.m_tkmFilePath = filePath;
		// 初期化データをもとにモデルを初期化
		m_renderToReserveModel.Init(modelInitData);
	}
	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "インスタンスの数が多すぎです。");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_forwardRenderModel.CalcWorldMatrix(pos, rot, scale);

		// インスタンシング描画を行う。
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//インスタンス数が0の場合のみアニメーション関係の更新を行う。
			// スケルトンを更新。
			// 各インスタンスのワールド空間への変換は、
			// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
			// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
			m_skeleton.Update(g_matIdentity);
			//アニメーションを進める。
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::Update()
	{

		UpdateWorldMatrix();

		if (m_skeleton.IsInited()) {
			// スケルトンを更新
			m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
		}
		
		// アニメーションを更新
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::UpdateWorldMatrix()
	{
		if (m_renderToReserveModel.IsInited())
		{
			m_renderToReserveModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_forwardRenderModel.IsInited())
		{
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Draw()
	{
		if (m_renderToReserveModel.IsInited())
		{
			g_renderingEngine.Add3DModelToRenderReservePass(m_renderToReserveModel);
		}
		if (m_forwardRenderModel.IsInited())
		{
			g_renderingEngine.Add3DModelToForwardRenderPass(m_forwardRenderModel);
		}
	}
}
