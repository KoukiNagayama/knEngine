#pragma once
#include "sound/SoundSource.h"

class EdgeManagement;
class Player;
class Game;
class Bell;
class CollectItem : public IGameObject
{
public:
	CollectItem() {};
	~CollectItem() {};

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
	/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �֊s���ɉe����^�������ݒ�B
	/// </summary>
	/// <param name="edgeManagement">�֊s���ɉe����^������B</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
	const void SetPosition(Vector3 position)
	{
		m_position = position;
	}
private:
	/// <summary>
	/// ��]�B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �v���C���[�Ƃ̋����𒲂ׂ�B
	/// </summary>
	void CheckDistanceToPlayer();
	/// <summary>
	/// �Ăяo������܂ł̃J�E���g�B
	/// </summary>
	void CountAppearsAgain();
	/// <summary>
	/// �A�C�e�����擾����B
	/// </summary>
	void RetrieveItem();

private:
	ModelRender				m_itemModel;							// �A�C�e���̃��f��
	Vector3					m_position = Vector3::Zero;				// ���W
	Quaternion				m_rotation = Quaternion::Identity;		// ��]
	Vector3					m_scale = Vector3::One;					// �g�嗦
	EdgeManagement*			m_edgeManagement = nullptr;				// �֊s������
	bool					m_isDraw = true;						// �`�悷�邩
	Player*					m_player = nullptr;						// �v���C���[
	float					m_drawAgainTimerPerSec = 0.0f;			// �Ăѕ`�悳���悤�ɂȂ�܂ł̃^�C�}�[(�P��:�b)
	Game*					m_game = nullptr;						// �Q�[��
	Vector3					m_playerPos = Vector3::Zero;			// �v���C���[�̍��W
	Vector3					m_distToPlayer = Vector3::Zero;			// �v���C���[�Ƃ̋���
	int						m_currentColor;							// ���݂̐F
	Bell*					m_bell = nullptr;						// �x��
	float					m_resetColorTimerPerSec = 0.0f;			// �F�����Z�b�g����^�C�}�[(�P��:�b)
	SoundSource*			m_pickUpBellSound = nullptr;
};

