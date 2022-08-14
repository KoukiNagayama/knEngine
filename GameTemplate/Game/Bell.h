#pragma once
#include "sound/SoundSource.h"

class Player;
class EdgeManagement;
class Bell : public IGameObject
{
public:
	Bell() {};
	~Bell();
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
	/// �x���̉������Ă��邩�B
	/// </summary>
	/// <returns>���Ă����true�B���Ă��Ȃ����false�B</returns>
	bool IsBellRinging() const
	{
		return m_isRing;
	}

private:
	/// <summary>
	/// �v���C���[�ɍ��킹�Ĉړ��B
	/// </summary>
	void MoveWithPlayer();
	/// <summary>
	/// �炷�B
	/// </summary>
	void Ring();
private:
	Player*				m_player = nullptr;						// �v���C���[�N���X
	ModelRender			m_bellModel;							// �x���̃��f��
	Vector3				m_position = Vector3::Zero;				// ���W
	EdgeManagement*		m_edgeManagement = nullptr;				// �֊s�����Ǘ�
	SoundSource*		m_bellSound = nullptr;					// �x���̉�			
	float				m_availableAgainTimer = 0.0f;		// �ēx�g�p�\�ɂȂ�^�C�}�[
	bool				m_isRing = false;						// �x���̉������Ă��邩�B
};

