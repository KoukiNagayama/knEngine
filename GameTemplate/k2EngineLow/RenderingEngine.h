#pragma once
#include "Bloom.h"
#include "EdgeControl.h"
namespace nsK2EngineLow {

	const int MAX_TEXT_SIZE = 256;

	class RenderingEngine
	{
	public:
		/// <summary>
		/// �����_�����O�p�C�v���C����������
		/// </summary>
		void Init();
		/// <summary>
		/// �����_�����O�p�C�v���C�������s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Execute(RenderContext& rc);
		/// <summary>
		/// �����p�̕`��p�X�Ƀ��f����ǉ�
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToRenderReservePass(Model& model)
		{
			m_renderToReservePassModels.push_back(&model);
		}
		/// <summary>
		/// �t�H���[�h�����_�����O�̕`��p�X�Ƀ��f����ǉ�
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
		/// <summary>
		/// �X�v���C�g�`��p�X�ɃX�v���C�g��ǉ�
		/// </summary>
		/// <param name="sprite"></param>
		void AddSpriteToSpriteRenderPass(Sprite& sprite)
		{
			m_sprites.push_back(&sprite);
		}
		/// <summary>
		/// �[�x�l�L�^�p�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�[�x�l�L�^�p�����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetDepthValue()
		{
			return m_reserveRenderTarget[enReserveDepth];
		}
		/// <summary>
		/// ���[���h���W�L�^�p�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>���[���h���W�L�^�p�̃����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetWorldPos()
		{
			return m_reserveRenderTarget[enReserveWorldPos];
		}
		/// <summary>
		/// �@���L�^�p�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�@���L�^�p�̃����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetNormal()
		{
			return m_reserveRenderTarget[enReserveNormal];
		}
	private:
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g��������
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// �����p�����_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitReserveRenderTarget();
		/// <summary>
		/// 2D�`��p�����_�����O�^�[�Q�b�g��������
		/// </summary>
		void Init2DRenderTarget();
		/// <summary>
		/// �����p�p�X�ւ̕`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderToReservePass(RenderContext& rc);
		/// <summary>
		/// �t�H���[�h�����_�����O
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// �X�v���C�g�`��
		/// </summary>
		/// <param name="rc"></param>
		void RenderSprite(RenderContext& rc);
		/// <summary>
		/// �t�H���g�`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderFont(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
	private:
		// Reserve�p�X�̃����_�����O�^�[�Q�b�g�̎��
		enum EnReserve
		{
			enReserveDepth,				// �[�x
			enReserveNormal,			// �@��
			enReserveWorldPos,			// ���[���h���W
			enReserveNum,				// G-Buffer�̐�
		};

		std::vector<Model*>		m_renderToReservePassModels;			// Reserve�̕`��p�X�ŕ`�悳��郂�f��
		std::vector<Model*>		m_forwardRenderModels;					// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		std::vector<Sprite*>	m_sprites;								// �X�v���C�g�̕`��p�X�ŕ`�悷��X�v���C�g
		RenderTarget			m_mainRenderTarget;						// ���C�������_�����O�^�[�Q�b�g
		RenderTarget			m_reserveRenderTarget[enReserveNum];	// G-Buffer�p�̃����_�����O�^�[�Q�b�g
		RenderTarget			m_2DRenderTarget;						// 2D�`��p�̃����_�����O�^�[�Q�b�g
		Sprite					m_copyMainRtToFrameBufferSprite;		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����p�̃X�v���C�g
		Sprite					m_2DSprite;								// 2D�����p�X�v���C�g
		Sprite					m_mainSprite;
		Bloom					m_bloom;
		EdgeControl				m_edgeControl;
	};
	extern RenderingEngine g_renderingEngine;
}
 

