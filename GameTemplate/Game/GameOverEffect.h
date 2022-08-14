#pragma once

class Player;
class Enemy;
class GameCamera;

/// <summary>
/// �Q�[���I�[�o�[�G�t�F�N�g
/// �Q�[���I�[�o�[���m�肵����Q�[���I�[�o�[�ɂȂ�܂ł̊Ԃ������s�����
/// </summary>
class GameOverEffect
{
public:

	/// <summary>
	/// ������
	/// �i�|�C���^�n���j
	/// </summary>
	/// <param name="pl">�v���C���[�̃|�C���^</param>
	/// <param name="en">�G�̃|�C���^</param>
	/// <param name="gc">�Q�[���J�����̃|�C���^</param>
	void Init(Player* pl, Enemy* en, GameCamera* gc) {
		m_player = pl;
		m_enemy = en;
		m_gameCamera = gc;
	}

	/// <summary>
	/// �G�t�F�N�g���J�n����
	/// </summary>
	void StartGameOverEffect();

	/// <summary>
	/// �G�t�F�N�g���I��点��
	/// </summary>
	void EndGameOverEffect() {
		m_isPlayEffect = false;
	}

	/// <summary>
	/// �Q�[���I�[�o�[�G�t�F�N�g���Đ�
	/// </summary>
	void PlayGameOverEffect();

	/// <summary>
	/// �Q�[���I�[�o�[�G�t�F�N�g���H
	/// </summary>
	/// <returns>true�Ȃ�Q�[���I�[�o�[�G�t�F�N�g��</returns>
	bool IsPlayEffect() {
		return m_isPlayEffect;
	}

	/// <summary>
	/// �z���[�����~������
	/// �i���o�p�j
	/// </summary>
	void StopHorrorSound() {
		m_horrorSound->Stop();
	}

private:

	bool m_isPlayEffect = false;					// �Q�[���I�[�o�[�G�t�F�N�g�Đ��t���O

	Player* m_player = nullptr;						// �v���C���[
	Enemy* m_enemy = nullptr;						// �G�l�~�[
	GameCamera* m_gameCamera = nullptr;				// �Q�[���J����
	SoundSource* m_catchSound = nullptr;			// �ߊl�̉���
	SoundSource* m_horrorSound = nullptr;			// �z���[�̉���

	// �������牉�o�p�����o�ϐ�
	Vector3 m_cameraNextTarget = Vector3::Zero;		// ���t���[���̒����_��ۑ����Ă���

	Vector3 m_cameraTargetMove = Vector3::Zero;		// �J�����̈ړ���
	float m_cameraTargetMoveTimer = 0.0f;			// �J������G�Ɍ�����^�C�}�[

	Vector3 m_cameraShake = Vector3::Zero;			// �J�����V�F�C�N��
	float m_cameraCatchShakeTimer = 0.0f;			// �߂܂����u�ԑ傫���V�F�C�N����p�^�C�}�[

};

