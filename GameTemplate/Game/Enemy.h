#pragma once

#include "EnemyPath.h"
#include "AI/PathFinding/PathFinding.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "sound/SoundSource.h"
#include "IEnemyState.h"


class Player;
class Bell;
class EdgeManagement;
class Game;
class Enemy : public IGameObject
{
private:
	// �X�e�[�g
	enum EnEnemyState {
		enEnemyState_Walk,				// ����	
		enEnemyState_Scream,			// ���K
		enEnemyState_Chase,				// �ǐ�
		enEnemyState_Survey,			// ����
		enEnemyState_ReturnToPath,		// �p�X�ړ��ɖ߂�
		enEnemyState_Attack,			// �U��
		enEnemyState_Num,				// �X�e�[�g�̐�
	};
public:
	Enemy() {};
	~Enemy();
	
	/// <summary>
	/// �J�n�����B
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �G�l�~�[�̔ԍ���ݒ�
	/// </summary>
	/// <param name="number">�ԍ�</param>
	void SetNumber(const int& number)
	{
		m_enemyNumber = number;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// �֊s���ɉe����^�������ݒ�B
	/// </summary>
	/// <param name="edgeManagement">�֊s���ɉe����^������</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
	/// <summary>
	/// ���K�����Ă��邩�B
	/// </summary>
	/// <returns></returns>
	const bool IsScream()
	{
		return m_isScream;
	}
	/// <summary>
	/// �����Ă��邩�B
	/// </summary>
	/// <returns></returns>
	const bool IsMoving()
	{
		return m_isMove;
	}
	/// <summary>
	/// �U�������Ă��邩�B
	/// </summary>
	/// <returns>���Ă���Ȃ��true�B���Ă��Ȃ��Ȃ��false�B</returns>
	const bool IsAttacking()
	{
		return m_isAttackable;
	}
	/// <summary>
	/// �ǐՂ��Ă��邩�B
	/// </summary>
	/// <returns>���Ă����true�B���Ă��Ȃ����false�B</returns>
	const bool IsChase()
	{
		return m_isChase;
	}
private:
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �X�e�[�g�ɂ�鏈��
	/// </summary>
	void ProcessByState();
	/// <summary>
	/// �A�j���[�V�����Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �v���C���[�̉���T��
	/// </summary>
	void SearchSoundOfPlayer();
	/// <summary>
	/// �����̃A�j���[�V�����C�x���g�B
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �����̉��ʐ���
	/// </summary>
	void StepVolumeControl();
	/// <summary>
	/// ��Ԃ�؂�ւ���B
	/// </summary>
	/// <param name="nextState"></param>
	void ChangeState(EnEnemyState nextState);
private:
	friend class EnemyWalkState;
	friend class EnemyChaseState;
	friend class EnemyReturnToPathState;
	friend class EnemyScreamState;
	friend class EnemySurveyState;
	friend class EnemyAttackState;

	

	// �A�j���[�V����
	enum EnAnimationClip {
		enAnimationClip_Walk,			// ����
		enAnimationClip_Run,			// ����
		enAnimationClip_Scream,			// ���K
		enAnimationClip_Survey,			// ����
		enAnimationClip_Attack,			// �U��
		enAnimationClip_Num,			// �A�j���[�V�����̐�
	};

	Vector3					m_position = Vector3::Zero;					// ���W
	Quaternion				m_rotation = Quaternion::Identity;			// ��]
	Vector3					m_scale = Vector3::One * 5.0f;				// �g�嗦
	int						m_enemyNumber;								// ���x����œo�^���ꂽ�G�l�~�[�̔ԍ�
	AnimationClip			m_animationClips[enAnimationClip_Num];		// �A�j���[�V�����N���b�v
	Vector3					m_moveVector;								// �ړ��������
	Vector3					m_moveSpeed;								// �ړ����鑬��
	EnEnemyState			m_enemyState = enEnemyState_Walk;			// �X�e�[�g
	ModelRender				m_enemyModel;								// �G�l�~�[�̃��f��
	EnemyPath				m_enemyPath;								// �p�X
	Point*					m_point = nullptr;							// �p�X�ړ��̃|�C���g
	Player*					m_player = nullptr;							// �v���C���[�N���X
	Bell*					m_bell = nullptr;							// �x���N���X
	nsAI::NaviMesh			m_nvmMesh;									// �i�r�Q�[�V�������b�V��
	nsAI::Path				m_path;										// �i�r�Q�[�V�������b�V���p�p�X
	nsAI::PathFinding		m_pathFinding;								// �i�r�Q�[�V�������b�V���p�p�X����
	float					m_animationSpeed;							// �A�j���[�V�����̃X�s�[�h
	float					m_pathFindingTimer = 0.0f;					// �ړI�n�܂ł̃p�X����������Ԃ̎���
	Vector3					m_lastPosition;								// ���O�̍��W
	float					m_chaseTime;								// �ǂ����������鎞��
	bool					m_isAttackable = false;						// �v���C���[�ւ̍U���͉\��
	bool					m_isFound = false;							// �v���C���[�������Ă��邩
	float					m_surveyTimer = 0.0f;						// ���񂷎���
	bool					m_isScream = false;							// ���K���Ă��邩
	bool					m_isMove = false;							// �����Ă��邩
	float					m_screamRateByTime = 0.0f;					// ���K�̎��Ԃɂ��e����
	float					m_timeToReturn = 0.0f;						// �A�҂��鎞��
	bool					m_isPathFindingWhileReturning = false;		// �p�X�ړ��ւ̋A�Ҏ��Ƀp�X������������
	float					m_attackingTimer = 1.5f;					// �U���̉��o�܂ł̎���
	bool					m_isCaughtPlayer = false;					// �v���C���[��߂܂��Ă��邩
	EdgeManagement*			m_edgeManagement = nullptr;					// �֊s�����
	SoundSource*			m_stepSound = nullptr;						// �����̉����I�u�W�F�N�g
	SoundSource*			m_screamSound = nullptr;					// ���K�̉����I�u�W�F�N�g
	Game*					m_game = nullptr;							// �Q�[���N���X
	bool					m_isChase = false;							// �ǐՂ��Ă��邩
	float					m_screamEndTimer = 0.0f;					// ���K�I���܂ł̃^�C�}�[
	IEnemyState*			m_iEnemyState = nullptr;

};

