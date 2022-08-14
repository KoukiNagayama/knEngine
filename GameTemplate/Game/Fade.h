#pragma once
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	/// <summary>
	/// �t�F�[�h�C�����J�n�B
	/// </summary>
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g���J�n�B
	/// </summary>
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/// <summary>
	/// �t�F�[�h���Ă��邩�B
	/// </summary>
	/// <returns>���Ă���Ȃ�true�B���Ă��Ȃ����false�B</returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	/// <summary>
	/// ���l���擾�B
	/// </summary>
	/// <returns>���݂̃��l�B</returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
	/// <summary>
	/// �t�F�[�h�̃X�s�[�h��ݒ�B
	/// </summary>
	/// <param name="fadeSpeed">�t�F�[�h�̃X�s�[�h�B�ʏ��1.0</param>
	void SetFadeSpeed(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
	}
private:
	enum EnState {
		enState_FadeIn,			// �t�F�[�h�C����
		enState_FadeOut,		// �t�F�[�h�A�E�g��
		enState_Idle,			// �A�C�h����
	};
	SpriteRender		m_fadeSprite;				// �t�F�[�h�Ŏg�p����X�v���C�g
	EnState				m_state = enState_Idle;		// �X�e�[�g
	float				m_currentAlpha = 1.0f;		// ���݂̃��l
	float				m_fadeSpeed = 1.0f;			// �t�F�[�h�̃X�s�[�h
};


