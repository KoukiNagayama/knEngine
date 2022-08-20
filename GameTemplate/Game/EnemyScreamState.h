#pragma once

#include "IEnemyState.h"

class EnemyScreamState : public IEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="enemy">�G�l�~�[</param>
	EnemyScreamState(Enemy* enemy) :
		IEnemyState(enemy) {}
private:
	/// <summary>
	/// �X�e�[�g�Ǘ�����
	/// </summary>
	void ManageState() override;
	/// <summary>
	/// �X�e�[�g���Ƃ̏���
	/// </summary>
	void ProcessByState() override;
	/// <summary>
	/// ���K
	/// </summary>
	void Scream();
	/// <summary>
	/// �A�j���[�V�����Đ�����
	/// </summary>
	void PlayAnimation() override;
};

