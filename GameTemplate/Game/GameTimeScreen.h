#pragma once

/// <summary>
/// �c�莞�Ԃ�\������N���X
/// 3���Ή�
/// </summary>
class GameTimeScreen : public IGameObject
{
public:
	GameTimeScreen();
	~GameTimeScreen();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(){};
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);

	/// <summary>
	/// �^�C�}�[�̏�Ԃ��X�V
	/// </summary>
	/// <param name="time">�\�����鎞��</param>
	void GameTimerUpdate(const float time);

	/// <summary>
	/// �`��t���O��ݒ�
	/// </summary>
	/// <param name="flag">�t���O</param>
	void SetDrawFlag(const bool flag) {
		m_drawFlag = flag;
	}

	/// <summary>
	/// �ϐ����Z�b�g
	/// </summary>
	void Reset() {
		m_colorChangeFlag = false;
		m_oldTime = -1;
	}

private:
	
	SpriteRender m_numberSprite[3];		// �����摜
	int m_numberNum = -1;				// �����̐��i�ŏ��Ɍv�Z�j

	bool m_colorChangeFlag = false;		// �F�ύX�t���O

	int m_oldTime = -1;					// ������

	bool m_drawFlag = true;				// �`��t���O


};

