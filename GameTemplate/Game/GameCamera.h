#pragma once
class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera() {};
	/// <summary>
	/// �J�n����
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
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
		SetPosition({ x,y,z });
	}
	/// <summary>
	/// �����_���王�_�Ɍ������x�N�g���̎擾
	/// </summary>
	/// <returns>�����_���王�_�Ɍ������x�N�g��</returns>
	const Vector3 GetToCameraPos()
	{
		return m_toCameraPos;
	}
	/// <summary>
	/// �ړ��\�t���O��ݒ�
	/// </summary>
	/// <param name="movable">�ړ��\���ǂ���</param>
	void SetIsMovable(const bool movable)
	{
		m_isMovable = movable;
	}
private:
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	/// <summary>
	/// �����_�̏����B
	/// </summary>
	void ViewPoint();
	/// <summary>
	/// ��]�̏��
	/// </summary>
	bool RotationLimit();
private:
	Player*			m_player = nullptr;					// �v���C���[
	Vector3			m_toCameraPos = Vector3::Zero;		// �����_���王�_�ւ̃x�N�g��
	Vector3			m_position = Vector3::Zero;			// �J�����̍��W
	Quaternion		m_rotation = Quaternion::Identity;	// �J�����̉�]
	//bool			m_isTargetInRange = true;			// �����_���J�����̉�]�͈͓̔��ł���Ȃ��
	Vector3			m_target = Vector3::Zero;			// �����_
	bool			m_isMovable = true;					// �ړ����\���H

};	

