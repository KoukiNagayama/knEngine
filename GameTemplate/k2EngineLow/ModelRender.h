#pragma once
#include "EdgeControl.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		ModelRender() {};
		~ModelRender() {};

		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="outlineType">�֊s���̎�ށB</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			int maxInstance = 0,
			int outlineType = 0,
			EdgeControl* edgeControl = nullptr
		);
		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();
		/// <summary>
		/// �C���X�^���V���O�f�[�^�̍X�V�B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		void UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
		/// <summary>
		/// �`�揈��
		/// </summary>
		void Draw();
		/// <summary>
		/// �A�j���[�V�����Đ�
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">��Ԏ���(�P�ʁF�b)�B</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		/// <returns>�Đ����Ȃ��true�B�Đ����Ă��Ȃ��Ȃ��false</returns>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/// <summary>
		/// ���W�A��]�A�g�嗦��ݒ�
		/// </summary>
		/// <param name="position">���W�B</param>
		/// <param name="rotation">��]�B</param>
		/// <param name="scale">�g�嗦�B</param>
		void SetTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(position);
			SetRotation(rotation);
			SetScale(scale);
		}
		/// <summary>
		/// ���W��ݒ�
		/// </summary>
		/// <param name="position">���W�B</param>
		void SetPosition(const Vector3& position) 
		{
			m_position = position;
		}
		/// <summary>
		/// ���W��ݒ�
		/// </summary>
		/// <param name="x">x���W�B</param>
		/// <param name="y">y���W�B</param>
		/// <param name="z">z���W�B</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// ��]��ݒ�
		/// </summary>
		/// <param name="rotation">��]�B</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// �g�嗦��ݒ�
		/// </summary>
		/// <param name="scale">�g�嗦�B</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �g�嗦��ݒ�
		/// </summary>
		/// <param name="x">x���W�B</param>
		/// <param name="y">y���W�B</param>
		/// <param name="z">z���W�B</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}
		/// <summary>
		/// �A�j���[�V�����̍Đ����x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed">�A�j���[�V�����̍Đ����x�B���l�̕��������x���{�ɂȂ�B</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		/// <summary>
		/// �A�j���[�V�����C�x���g�̒ǉ�
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/// <summary>
		/// ���f�����擾
		/// </summary>
		/// <returns>���f��</returns>
		Model& GetModel()
		{
			return m_forwardRenderModel;
		}
	private:
		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);
		/// <summary>
		/// �t�H���[�h�����_�����O�p���f����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="outlineType">�֊s���̎�ށB</param>
		void InitForwardRenderingModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			int outlineType = 0
		);
		/// <summary>
		/// �����p�X�ւ̕`��p�̃��f����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitModelOnRenderToReservePass(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// ���[���h�s��̍X�V
		/// </summary>
		void UpdateWorldMatrix();

	private:
		Model									m_forwardRenderModel;							// �t�H���[�h�����_�����O�ŕ`�悳��郂�f��
		Model									m_renderToReserveModel;							// RenderToReservePass�ŕ`�悳��郂�f��
		Skeleton								m_skeleton;										// �X�P���g��
		Animation								m_animation;									// �A�j���[�V����
		AnimationClip*							m_animationClips;								// �A�j���[�V�����N���b�v
		int										m_numAnimationClips;							// �A�j���[�V�����N���b�v�̐�
		Vector3									m_position = Vector3::Zero;						// ���W
		Quaternion								m_rotation = Quaternion::Identity;				// ��]
		Vector3									m_scale = Vector3::One;							// �g�嗦
		float									m_animationSpeed = 1.0f;						// �A�j���[�V�����̍Đ����x
		EdgeControl*							m_edgeControl;									// �֊s���̐���
		int										m_numInstance = 0;											// �C���X�^���X�̐�
		int										m_maxInstance = 1;											// �ő�C���X�^���X��
		int										m_fixNumInstanceOnFrame = 0;								// ���̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔
		bool									m_isEnableInstancingDraw = false;							// �C���X�^���V���O�`�悪�L���H
		std::unique_ptr<Matrix[]>				m_worldMatrixArray;											// ���[���h�s��̔z��B
		StructuredBuffer						m_worldMatrixArraySB;										// ���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
		int										m_edgeColor;
	};

}

