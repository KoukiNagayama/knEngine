#include "stdafx.h"
#include "Enemy.h"
#include "Bell.h"
#include "Player.h"
#include "EdgeManagement.h"
#include "sound/SoundEngine.h"
#include "Game.h"


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
}

Enemy::~Enemy()
{
	if (m_stepSound != nullptr) {
		DeleteGO(m_stepSound);
	}
	if (m_screamSound != nullptr) {
		DeleteGO(m_screamSound);
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
	switch (m_enemyState) {
		// �������
	case enEnemyState_Walk:
		ProcessWalkStateTransition();
		break;
		// ���K���
	case enEnemyState_Scream:
		ProcessScreamStateTransition();
		break;
		// �ǐՏ��
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		// ���񂵏��
	case enEnemyState_Survey:
		ProcessSurveyStateTransition();
		break;
		// �p�X�ړ��ւ̋A�ҏ��
	case enEnemyState_ReturnToPath:
		ProcessReturnToPathStateTransition();
		break;
		// �U�����
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Enemy::ProcessByState()
{
	switch (m_enemyState) {
		// �������
	case enEnemyState_Walk:
		Walk();
		SearchSoundOfPlayer();
		break;
		// ���K���
	case enEnemyState_Scream:
		Scream();
		break;
		// �ǐՏ��
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
		// ���񂵏��
	case enEnemyState_Survey:
		Survey();
		SearchSoundOfPlayer();
		break;
		// �p�X�ړ��ւ̋A�ҏ��
	case enEnemyState_ReturnToPath:
		ReturnToPath();
		SearchSoundOfPlayer();
		break;
		// �U�����
	case enEnemyState_Attack:
		Attack();
		break;
	}
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

void Enemy::Walk()
{
	// �ړI�n�܂ł̋���
	Vector3 distance = m_point->s_position - m_position;

	// �ړI�n�܂ł̋������߂����
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
		// ���ݍŌ�̃|�C���g�ɂ���Ȃ��
		if (m_point->s_number == m_enemyPath.GetPointListSize() - 1) {
			// �ŏ��̃|�C���g�����擾
			m_point = m_enemyPath.GetFirstPoint();
		}
		// �Ō�ȊO�̃|�C���g�ɂ���Ȃ��
		else {
			// ���̃|�C���g�����擾
			m_point = m_enemyPath.GetNextPoint(m_point->s_number);
		}

	}

	// �ړ����������ݒ�
	m_moveVector = distance;
	m_moveVector.Normalize();

	// �������̈ړ����x��ݒ�
	m_moveSpeed = m_moveVector * WALK_SPEED;

	// ���W�Ɉړ����x�����Z
	m_position += m_moveSpeed;
}

void Enemy::Scream()
{
	// ���K�̉�����炷
	if (m_screamSound == nullptr) {
		m_screamSound = NewGO<SoundSource>(0);
		m_screamSound->Init(ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER);
		m_screamSound->SetVolume(SCREAM_VOLUME);
		m_screamSound->Play(false);
		m_isScream = true;
	}

	// �^�C�}�[�B
	m_screamEndTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_screamEndTimer <= 0.0f) {
		m_isScream = false;
		m_screamSound = nullptr;
	}
}


void Enemy::Chase()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_player->GetPosition();

	if (m_pathFindingTimer <= 0.0f) {
		// �v���C���[�܂ł̃p�X������
		m_pathFinding.Execute(
			m_path,
			m_nvmMesh,
			m_position,
			playerPos,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		// �^�C�}�[���Z�b�g
		m_pathFindingTimer = PATH_FINDING_TIMER;
	}

	m_pathFindingTimer -= g_gameTime->GetFrameDeltaTime();

	// �w�肵�����W�ւ̈ړ�������������
	bool isEnd;
	// �ړ����O�̍��W���L�^
	m_lastPosition = m_position;

	// �Œ�ǐՂ���c�莞��
	// �ǐՂ���c�莞�Ԃ����炷
	m_chaseTime -= g_gameTime->GetFrameDeltaTime();
	// �ǐՂ���c�莞�Ԃ�0�ɂȂ����Ȃ�
	if (m_chaseTime < 0.0f) {
		m_chaseTime = 0.0f;
	}


	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_position = m_path.Move(
		m_position,
		RUN_SPEED,
		isEnd
	);

	// ����
	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 momentDist = m_position - m_lastPosition;
	momentDist.Normalize();

	// �ړ����������ݒ�
	m_moveVector = momentDist;

	// �v���C���[�Ƃ̋���
	Vector3 distanceToPlayer = m_position - playerPos;
	// �v���C���[�Ƃ̋������߂����
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// �U�����s��
		m_isAttackable = true;
	}
}

void Enemy::Survey()
{
	// ���񂵂����Ԃ����Z
	m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void Enemy::ReturnToPath()
{
	bool isEnd;

	// �ړ����O�̍��W���L�^
	m_lastPosition = m_position;

	// �p�X���������Ă��Ȃ��Ȃ��
	if (m_isPathFindingWhileReturning == false) {
		// ���݂̍��W�����ԋ߂����W�̃|�C���g���擾
		m_point = m_enemyPath.GetNearPoint(m_position);
		// �A�ҖڕW�̃p�X�܂ł̃p�X������
		m_pathFinding.Execute(
			m_path,
			m_nvmMesh,
			m_position,
			m_point->s_position,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		m_isPathFindingWhileReturning = true;
	}

	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_position = m_path.Move(
		m_position,
		WALK_SPEED,
		isEnd
	);

	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 distance = m_position - m_lastPosition;
	distance.Normalize();

	// �ړ����������ݒ�
	m_moveVector = distance;
}

void Enemy::Attack()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_player->GetPosition();

	Vector3 distance = playerPos - m_position;
	distance.Normalize();

	m_moveVector = distance;
}

void Enemy::ProcessWalkStateTransition()
{
	// �G�𔭌����Ă��Ȃ��Ȃ��
	if (m_isFound == false) {
		m_isMove = true;
		return;
	}
	// ���K�����Z�b�g
	m_screamRateByTime = 0.0f;
	m_isScream = true;
	m_isMove = false;
	// ���K���I������܂ł̃^�C�}�[�����Z�b�g�B
	m_screamEndTimer = TIME_TO_END_SCREAM_PER_SEC;
	// �X�e�[�g����K��Ԃɂ���
	m_enemyState = enEnemyState_Scream;

}

void Enemy::ProcessScreamStateTransition()
{
	// ���K���I�����Ă��Ȃ��Ȃ��
	if (m_isScream == true) {
		return;
	}
	m_isMove = true;
	// �Œ���ǐՂ��鎞�Ԃ��w��
	m_chaseTime = MINIMUM_CHASE_TIME;
	// �X�e�[�g��ǐՏ�Ԃɂ���
	m_enemyState = enEnemyState_Chase;
	// �ǐՂ��Ă���悤�ɋL�^�B
	m_isChase = true;
}

void Enemy::ProcessChaseStateTransition()
{
	// �v���C���[���U���\�ȋ����Ȃ��
	if (m_isAttackable == true) {
		m_enemyState = enEnemyState_Attack;
		// �Q�[���I�[�o�[�G�t�F�N�g�̊J�n
		m_game->StartGameOverEffect();
		m_isMove = false;
		return;
	}
	// �G��ǐՂ����Ԃ��ێ�����Ă���Ȃ��
	if (m_chaseTime > 0.0f || m_isFound == true) {
		return;
	}
	// �v���C���[���������Ă����Ȃ��
	if (m_isFound == false) {
		// ���񂷎��Ԃ����Z�b�g
		m_surveyTimer = 0.0f;
		// �X�e�[�g�����񂵏�Ԃɂ���
		m_enemyState = enEnemyState_Survey;
		// �p�X����������Ԋu�����Z�b�g
		m_pathFindingTimer = PATH_FINDING_TIMER;
		m_isMove = false;
		m_isChase = false;

	}

}

void Enemy::ProcessSurveyStateTransition()
{
	// ��莞�Ԉȓ��Ƀv���C���[�𔭌�
	if (m_isFound == true) {
		// �Œ���ǐՂ��鎞�Ԃ��w��
		m_chaseTime = MINIMUM_CHASE_TIME;
		// �ǐՂ��J�n
		m_enemyState = enEnemyState_Chase;
		// �ǐՂ���B
		m_isChase = true;

		m_isMove = true;
	}
	// ��莞�ԃv���C���[���������Ă�����
	if (m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// �A�ҏ�Ԃ̎��Ԃ��w��
		m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// �X�e�[�g���p�X�ւ̋A�ҏ�Ԃɂ���
		m_enemyState = enEnemyState_ReturnToPath;

		m_isMove = true;
	}

}

void Enemy::ProcessReturnToPathStateTransition()
{
	// �ڕW�̍��W�ƌ��݂̍��W�̋����𑪂�
	Vector3 distance = m_point->s_position - m_position;

	m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// �v���C���[�𔭌�������
	if (m_isFound == true) {
		// ���K�����Z�b�g
		m_screamRateByTime = 0.0f;
		m_isScream = true;
		// ���K���I������܂ł̃^�C�}�[�����Z�b�g�B
		m_screamEndTimer = TIME_TO_END_SCREAM_PER_SEC;
		// �X�e�[�g����K��Ԃɂ���
		m_enemyState = enEnemyState_Scream;


		m_isMove = false;
	}
	// �ڕW�̍��W�ɋ߂��Ȃ�����
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// �X�e�[�g�������Ԃɂ���
		m_enemyState = enEnemyState_Walk;

	}
	// ��莞�Ԉȓ��ɋA�҂ł��Ȃ������ꍇ
	else if (m_timeToReturn <= 0.0f) {
		// �����I�ɃX�e�[�g�������Ԃɂ���
		m_enemyState = enEnemyState_Walk;

	}
	m_isPathFindingWhileReturning = false;
}

void Enemy::ProcessAttackStateTransition()
{
	// �Q�[���I�[�o�[
	m_attackingTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_attackingTimer <= 0.0f && m_isCaughtPlayer == false) {
		m_isCaughtPlayer = true;
		m_game->SetGameOver();
	}
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState) {
		// ����
	case enEnemyState_Walk:
		m_enemyModel.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// ���K
	case enEnemyState_Scream:
		m_enemyModel.PlayAnimation(enAnimationClip_Scream, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// �ǐ�
	case enEnemyState_Chase:
		m_enemyModel.PlayAnimation(enAnimationClip_Run, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// ����
	case enEnemyState_Survey:
		m_enemyModel.PlayAnimation(enAnimationClip_Survey, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// �p�X�ւ̋A��
	case enEnemyState_ReturnToPath:
		m_enemyModel.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// �U��
	case enEnemyState_Attack:
		m_enemyModel.PlayAnimation(enAnimationClip_Attack, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	}
}

void Enemy::OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uwalk1�v�̎��B
	if (wcscmp(eventName, L"walk1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���uwalk2�v�̎��B
	else if (wcscmp(eventName, L"walk2") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���urun1�v�̎��B
	else if (wcscmp(eventName, L"run1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���urun2�v�̎��B
	else if (wcscmp(eventName, L"run2") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
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