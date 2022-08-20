#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "RenderingEngine.h"
#include "EdgeControl.h"

namespace
{
	const int NORMAL_MODEL_NUM = 0;			// �ʏ탂�f��
	const int ENEMY_MODEL_NUM = 1;			// �G�l�~�[���f��
	const int COLLECT_ITEM_MODEL_NUM = 2;	// ����A�C�e�����f��
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
		// �X�P���g�����������B
		InitSkeleton(filePath);
		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips);
		// �t�H���[�h�����_�����O�p�̃��f�����������B
		InitForwardRenderingModel(filePath, enModelUpAxis, outlineType);
		// GBuffer�`��p�̃��f����������
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
			// �A�j���[�V������������
			m_animation.Init(
				m_skeleton,					// �A�j���[�V�����𗬂����ރX�P���g��
				m_animationClips,			// �A�j���[�V�����N���b�v
				m_numAnimationClips			// �A�j���[�V�����̐�
			);
		}
	}

	void ModelRender::InitForwardRenderingModel(const char* filePath,
		EnModelUpAxis enModelUpAxis,
		int outlineType
	)
	{
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		if (outlineType == NORMAL_MODEL_NUM) {
			modelInitData.m_fxFilePath = "Assets/shader/normalModel.fx";
		}
		else if (outlineType == ENEMY_MODEL_NUM) {
			modelInitData.m_fxFilePath = "Assets/shader/enemy.fx";
		}
		else if (outlineType == COLLECT_ITEM_MODEL_NUM) {
			modelInitData.m_fxFilePath = "Assets/shader/collectItem.fx";
		}
		// �G���g���[�|�C���g���w�肷��B
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// �J���[�o�b�t�@�̃t�H�[�}�b�g���w�肷��B
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// �e��}�b�v���g��SRV�ɐݒ肷��B
		modelInitData.m_expandShaderResoruceView[0] = &g_renderingEngine.GetDepthValue().GetRenderTargetTexture();
		modelInitData.m_expandShaderResoruceView[1] = &g_renderingEngine.GetWorldPos().GetRenderTargetTexture();
		modelInitData.m_expandShaderResoruceView[2] = &g_renderingEngine.GetNormal().GetRenderTargetTexture();
		
		// ���f���̏�������w�肷��B
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �����f�[�^��萔�o�b�t�@�Ƃ��Đݒ肷��
		modelInitData.m_expandConstantBuffer = (void *)&m_edgeControl->GetSoundSourceData();
		modelInitData.m_expandConstantBufferSize = sizeof(m_edgeControl->GetSoundSourceData());
		// �쐬�����������f�[�^�����ƂɃ��f��������������B
		m_forwardRenderModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnRenderToReservePass(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;
		// ���f���̏�������w�肷��
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��
		modelInitData.m_fxFilePath = "Assets/shader/renderToReservePassForModel.fx";
		// �G���g���[�|�C���g���w�肷��B
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��
		modelInitData.m_tkmFilePath = filePath;
		// �������f�[�^�����ƂɃ��f����������
		m_renderToReserveModel.Init(modelInitData);
	}
	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "�C���X�^���X�̐����������ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_forwardRenderModel.CalcWorldMatrix(pos, rot, scale);

		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�B
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::Update()
	{

		UpdateWorldMatrix();

		if (m_skeleton.IsInited()) {
			// �X�P���g�����X�V
			m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
		}
		
		// �A�j���[�V�������X�V
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
