#pragma once

#include "IEnemyState.h"

class EnemySurveyState : public IEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="enemy">�G�l�~�[</param>
	EnemySurveyState(Enemy* enemy) :
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
	/// ����
	/// </summary>
	void Survey();
	/// <summary>
	/// �A�j���[�V�����Đ�����
	/// </summary>
	void PlayAnimation() override;
};

