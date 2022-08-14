#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		InitReserveRenderTarget();
		Init2DRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
		m_bloom.Init(m_mainRenderTarget);
	}
	
	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitReserveRenderTarget()
	{
		// �t���[���o�b�t�@�̏c��
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();
		// �t���[���o�b�t�@�̉���
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();

		// �[�x�l�o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_reserveRenderTarget[enReserveDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		// �@���o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_reserveRenderTarget[enReserveNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		// �����_�����O�^�[�Q�b�g�̃e�N�X�`���𖄂߂�l 
		float clearColor[4] = { 10000.0f, 10000.0f, 10000.0f, 1.0f };
		// ���[���h���W�o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_reserveRenderTarget[enReserveWorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		
		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		// �ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�����O�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		// �㏑���B
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// �𑜓x��2D�p�����_�����O�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		// �X�v���C�g�̏������I�u�W�F�N�g
		SpriteInitData spriteInitData;
		// �e�N�X�`��
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		// �𑜓x
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// �V�F�[�_�[�̃t�@�C���p�X���w��
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		// �������I�u�W�F�N�g�����ƂɃX�v���C�g��������
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{

		// �����p�p�X�ւ̃����_�����O
		RenderToReservePass(rc);

		// �֊s���̐���
		m_edgeControl.Update();

		// �t�H���[�h�����_�����O
		ForwardRendering(rc);

		// �u���[��
		m_bloom.Render(rc, m_mainRenderTarget);

		// �X�v���C�g�`��
		RenderSprite(rc);

		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);


		// �o�^����Ă���I�u�W�F�N�g���N���A
		m_renderToReservePassModels.clear();
		m_forwardRenderModels.clear();
		m_sprites.clear();
	}

	void RenderingEngine::RenderToReservePass(RenderContext& rc)
	{
		RenderTarget* rts[enReserveNum] = {
			&m_reserveRenderTarget[enReserveDepth],			// 0�Ԗڂ̃����_�����O�^�[�Q�b�g
			& m_reserveRenderTarget[enReserveNormal],		// 1�Ԗڂ̃����_�����O�^�[�Q�b�g
			& m_reserveRenderTarget[enReserveWorldPos]		// 2�Ԗڂ̃����_�����O�^�[�Q�b�g
		};

		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�\�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		// �����_�����O�^�[�Q�b�g��ݒ肷��
		rc.SetRenderTargetsAndViewport(ARRAYSIZE(rts), rts);
		// �����_�����O�^�[�Q�b�g���N���A����
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& model : m_renderToReservePassModels) {
			// ���f����`��
			model->Draw(rc);
		}

		// �������݊����҂�
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�\�ɂȂ�܂ő҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ肷��
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A����
		rc.ClearRenderTargetView(m_mainRenderTarget);

		for (auto& model : m_forwardRenderModels)
		{
			// ���f����`��
			model->Draw(rc);
		}
		// �������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::RenderSprite(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		for (auto& sprite : m_sprites) {
			// �X�v���C�g��`��
			sprite->Draw(rc);
		}

		// 2D�`��p�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_2DSprite.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// �r���[�|�[�g���w�肷��
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}

	RenderingEngine g_renderingEngine;
}
