#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		int outlineType
	)
	{
		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips);
		// フォワードレンダリング用のモデルを初期化。
		InitForwardRenderingModel(filePath, enModelUpAxis, outlineType);
		// GBuffer描画用のモデルを初期化
		InitModelOnRenderToGBuffer(filePath, enModelUpAxis);
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
		modelInitData.m_fxFilePath = "Assets/shader/test.fx";
		/*if (outlineType == 0) {
			modelInitData.m_fxFilePath = "Assets/shader/edge.fx";
		}
		else if (outlineType == 1) {
			modelInitData.m_fxFilePath = "Assets/shader/edgeFRorange.fx";
		}
		else if (outlineType == 2) {
			modelInitData.m_fxFilePath = "Assets/shader/edgeFRred.fx";
		}
		else if (outlineType == 3) {
			modelInitData.m_fxFilePath = "Assets/shader/edgeTitleText.fx";
		}
		else if (outlineType == 4) {
			modelInitData.m_fxFilePath = "Assets/shader/whiteBoard.fx";
		}*/

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
		//modelInitData.m_expandConstantBuffer = (void*)&g_infoForEdge.GetSoundSourceData();
		//modelInitData.m_expandConstantBufferSize = sizeof(g_infoForEdge.GetSoundSourceData());
		// 作成した初期化データをもとにモデルを初期化する。
		m_forwardRenderModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnRenderToGBuffer(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;
		// モデルの上方向を指定する
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// シェーダーファイルのファイルパスを指定する
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBufferFor3DModel.fx";
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
		m_renderToGBufferModel.Init(modelInitData);
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
		if (m_renderToGBufferModel.IsInited())
		{
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_forwardRenderModel.IsInited())
		{
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Draw()
	{
		if (m_renderToGBufferModel.IsInited())
		{
			g_renderingEngine.Add3DModelToRenderGBufferPass(m_renderToGBufferModel);
		}
		if (m_forwardRenderModel.IsInited())
		{
			g_renderingEngine.Add3DModelToForwardRenderPass(m_forwardRenderModel);
		}
	}
}
