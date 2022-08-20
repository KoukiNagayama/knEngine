#pragma once
class Enemy;
class Game;

class IEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="enemy">�G�l�~�[</param>
	IEnemyState(Enemy* enemy) : 
		m_enemy(enemy)
	{

	}
	/// <summary>
	/// �X�e�[�g�Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �X�e�[�g���Ƃ̏���
	/// </summary>
	virtual void ProcessByState() = 0;
	/// <summary>
	/// �A�j���[�V�����Đ�����
	/// </summary>
	virtual void PlayAnimation() = 0;
protected:
	Enemy* m_enemy = nullptr;
};

