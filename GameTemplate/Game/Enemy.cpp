#include "stdafx.h"
#include "Enemy.h"
#include "Bell.h"
#include "Player.h"
#include "EdgeManagement.h"
#include "sound/SoundEngine.h"
#include "Game.h"
#include "EnemyWalkState.h"
#include "EnemyChaseState.h"
#include "EnemyScreamState.h"
#include "EnemyReturnToPathState.h"
#include "EnemySurveyState.h"
#include "EnemyAttackState.h"


namespace
{
	const float WALK_SPEED = 6.5f * 0.7f;							// ��������
	const float RUN_SPEED = 9.5f * 0.7f;							// ���鑬��
	const float SEARCH_RANGE_TO_BELL = 1000.0f;						// �x���̉��ɔ�������͈�
	const float SEARCH_RANGE_TO_PLAYER_FOOTSTEP = 300.0f;			// �v���C���[�̑����ɔ�������͈�
	const float SCREAM_VOLUME = 1.5f;								// ���K�̉���
	const float RANGE_OF_OUTLINE_DRAWING_BY_FOOTSTEP = 45.0f;		// �����ɂ��֊s���̕`��͈�
	const float ANIMATION_SPEED = 0.8f;								// �A�j���[�V�����X�s�[�h
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;			// �������̖ڕW�n�_�܂ł̋���
	const float ENEMY_RADIUS = 30.0f;								// �G�l�~�[�̔��a
	const float ENEMY_HEIGHT = 200.0f;								// �G�l�~�[�̍���
	const float PATH_FINDING_TIMER = 0.7f;							// �i�r���b�V���ɂ��p�X���������鎞�Ԃ̊Ԋu
	const float ATTACKING_RANGE = 120.0f;							// �U���\�ȋ���
	const float MINIMUM_CHASE_TIME = 20.0f;							// �Œ���ǐՂ��鎞��
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;			// ���Ԓ��̖ڕW�n�_�܂ł̋���
	const float TIME_TO_LOSE_SIGHT = 5.0f;							// �v���C���[��������������
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;			// �A�j���[�V�����̕�Ԏ���
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;				// �����I�ɃX�e�[�g�J�ڂ��鎞��
	const int	EDGE_COLOR_RED = 1;									// �֊s���̐F
	const int	ENEMY_WALK_SOUND_NUMBER_TO_REGISTER = 2;			// �o�^����G�l�~�[�̕����̉��̔ԍ�
	const int	ENEMY_RUN_SOUND_NUMBER_TO_REGISTER = 3;				// �o�^����G�l�~�[�̑���̉��̔ԍ�
	const int	ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER = 4;			// �o�^����G�l�~�[�̙��K�̉��̔ԍ�
	const float ENEMY_FOOTSTEP_SOUND_MAXIMUM_VOLUME = 1.0f;			// �����̍ő剹��
	const float ENEMY_FOOTSTEP_SOUND_MINIMUM_VOLUME = 0.0f;			// �����̍ŏ�����
	const float FOOTSTEP_RANGE_TO_PLAYER = 1350.0f;					// �������v���C���[�ɓ͂��͈�
	const float TIME_TO_END_SCREAM_PER_SEC = 3.0f;					// ���K���I������܂ł̎���
	const int   MATCH_ANIMATION_EVENT_NAME = 0;						// �A�j���[�V�����C�x���g�̖��O����v���Ă��邩
	const int	ENEMY_SOUND_PRIORITY = 0;							// �G�l�~�[�Ɋւ��鉹�̎��s�D�揇��(����)
}


Enemy::~Enemy()
{
	if (m_stepSound != nullptr) {
		DeleteGO(m_stepSound);
	}
	if (m_screamSound != nullptr) {
		DeleteGO(m_screamSound);
	}
	if (m_iEnemyState != nullptr) {
		delete m_iEnemyState;
	}
}

bool Enemy::Start()
{
	// �A�j���[�V����
	// �������̃A�j���[�V���������[�h(���[�v����)�B
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	// ���莞�̃A�j���[�V���������[�h(���[�v����)�B
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	// ���ю��̃A�j���[�V���������[�h(���[�v���Ȃ�)�B
	m_animationClips[enAnimationClip_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Scream].SetLoopFlag(false);
	// ���񂵎��̃A�j���[�V���������[�h(���[�v����)�B
	m_animationClips[enAnimationClip_Survey].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Survey].SetLoopFlag(true);
	// �U�����̃A�j���[�V���������[�h(���[�v����)�B
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);

	// �G�l�~�[�̃��f�����������B
	m_enemyModel.Init(
		"Assets/modelData/enemy/enemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ,
		0,
		EDGE_COLOR_RED,
		m_edgeManagement->GetEdgeControl()
	);

	// �A�j���[�V�����C�x���g��ǉ�
	m_enemyModel.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		// ����
		OnStepAnimationEvent(clipName, eventName);
		});

	// �t�@�C���p�X�̕�������쐬
	std::string filePath;
	filePath = "Assets/enemypath/enemypath";
	filePath += std::to_string(m_enemyNumber);
	filePath += ".tkl";
	// �쐬���ꂽ�t�@�C���p�X�����Ƃɏ������B
	m_enemyPath.Init(filePath.c_str());
	// �ŏ��̃p�X�ړ��̃|�C���g�����擾�B
	m_point = m_enemyPath.GetFirstPoint();

	// ���f���̏����̍��W�A��]�A�g�嗦��ݒ�B
	m_enemyModel.SetTRS(
		m_position,
		m_rotation,
		m_scale
	);

	// �v���C���[�̃C���X�^���X�������B
	m_player = FindGO<Player>("player");
	// �x���̃C���X�^���X�������B
	m_bell = FindGO<Bell>("bell");
	// �Q�[���𑍊�����I�u�W�F�N�g�̃C���X�^���X������
	m_game = FindGO<Game>("game");
	// �i�r�Q�[�V�������b�V���̏������B
	m_nvmMesh.Init("Assets/nvm/stage2_mesh.tkn");
	
	// �A�j���[�V�����̃X�s�[�h��ݒ�B
	m_enemyModel.SetAnimationSpeed(ANIMATION_SPEED);

	// �����̉��̓o�^�B
	g_soundEngine->ResistWaveFileBank(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER, "Assets/sound/enemy/walk1.wav");
	// ����̉��̓o�^�B
	g_soundEngine->ResistWaveFileBank(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER, "Assets/sound/enemy/run2.wav");
	// ���K�̉��̓o�^�B
	g_soundEngine->ResistWaveFileBank(ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER, "Assets/sound/enemy/scream.wav");
	
	// �����X�e�[�g������Ɏw��
	m_iEnemyState = new EnemyWalkState( this );

	return true;
}

void Enemy::Update()
{
	// ��]
	Rotation();
	// �X�e�[�g�ɂ�鏈��
	ProcessByState();
	// �X�e�[�g�Ǘ�
	ManageState();
	// �A�j���[�V�����Đ�
	PlayAnimation();
	// ���W�̍X�V�B
	m_enemyModel.SetPosition(m_position);
	// ���f���̍X�V�B
	m_enemyModel.Update();
}

void Enemy::Rotation()
{
	// ��]�p�x��ݒ�
	float angle = atan2(-m_moveVector.x, m_moveVector.z);

	// ��]��ݒ�
	m_rotation.SetRotationY(-angle);
	m_enemyModel.SetRotation(m_rotation);
}

void Enemy::ManageState()
{
	m_iEnemyState->ManageState();
}

void Enemy::ProcessByState()
{
	m_iEnemyState->ProcessByState();
}

void Enemy::PlayAnimation()
{
	m_iEnemyState->PlayAnimation();
}

void Enemy::SearchSoundOfPlayer()
{
	// ���G�͈�
	float searchRange = 0.0f;

	// �v���C���[�̍��W
	Vector3 playerPos = m_player->GetPosition();
	// �v���C���[�Ƃ̋���
	Vector3 distance = m_position - playerPos;

	// �x�������Ă���Ȃ�΁B
	if (m_bell->IsBellRinging()) {
		// ���G�͈͂��x���p�ɐݒ�B
		searchRange = SEARCH_RANGE_TO_BELL;
	}
	// ���������Ă���Ȃ�΁B
	else if (m_player->IsPlayerFootstepSound()) {
		// ���G�͈͂��v���C���[�̑����p�ɐݒ�B
		searchRange = SEARCH_RANGE_TO_PLAYER_FOOTSTEP;
	}
	
	// �v���C���[�Ƃ̋��������G�͈͓��Ȃ�΁B
	if (distance.Length() <= searchRange) {
		// �v���C���[�𔭌������B
		m_isFound = true;
		return;
	}

	// �����܂ŗ����Ȃ�Δ������Ă��Ȃ��B
	m_isFound = false;
}

void Enemy::ChangeState(EnEnemyState nextState)
{
	if (m_iEnemyState != nullptr) {
		delete m_iEnemyState;
	}
	switch (nextState) {
	case enEnemyState_Walk:
		m_iEnemyState = new EnemyWalkState(this);
		break;
	case enEnemyState_Scream:
		m_iEnemyState = new EnemyScreamState(this);
		break;
	case enEnemyState_Chase:
		m_iEnemyState = new EnemyChaseState(this);
		break;
	case enEnemyState_Survey:
		m_iEnemyState = new EnemySurveyState(this);
		break;
	case enEnemyState_ReturnToPath:
		m_iEnemyState = new EnemyReturnToPathState(this);
		break;
	case enEnemyState_Attack:
		m_iEnemyState = new EnemyAttackState(this);
		break;
	}
	m_enemyState = nextState;
}

void Enemy::OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uwalk1�v�̎��B
	if (wcscmp(eventName, L"walk1") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���uwalk2�v�̎��B
	else if (wcscmp(eventName, L"walk2") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���urun1�v�̎��B
	else if (wcscmp(eventName, L"run1") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���urun2�v�̎��B
	else if (wcscmp(eventName, L"run2") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
}

void Enemy::StepVolumeControl()
{
	Vector3 playerPos = m_player->GetPosition();
	playerPos.y = m_position.y;

	//�v���C���[�ƃG�l�~�[�̋���
	Vector3 diff = m_position - playerPos;

	// ���̑傫��
	float soundLevel = ENEMY_FOOTSTEP_SOUND_MAXIMUM_VOLUME - (diff.Length() / FOOTSTEP_RANGE_TO_PLAYER * ENEMY_FOOTSTEP_SOUND_MAXIMUM_VOLUME);
	// �ŏ����ʂ�菬�����Ȃ�����Œ肷��
	if (soundLevel <= ENEMY_FOOTSTEP_SOUND_MINIMUM_VOLUME) {
		soundLevel = ENEMY_FOOTSTEP_SOUND_MINIMUM_VOLUME;
	}

	if (m_stepSound != nullptr) {
		m_stepSound->SetVolume(soundLevel);
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_enemyModel.Draw();
}