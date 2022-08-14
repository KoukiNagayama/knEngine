#pragma once
class Fade;
class Result : public IGameObject
{
public:
	Result();
	~Result();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() {};
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="score">�\������X�R�A</param>
	void Init(const int score);

private:
	void ScoreUpdate();

	SpriteRender			m_resultSprite_BG;				// �w�i�摜
	SpriteRender			m_scoreNumberSprite[3];			// �X�R�A�̐���

	int						m_score = 0;					// �X�R�A
	int						m_numberNum = -1;				// �����̐��i�ŏ��Ɍv�Z�j
	bool					m_initFlag = false;				// �������t���O
	Fade*					m_fade = nullptr;

};

