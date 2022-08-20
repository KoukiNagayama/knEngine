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
	const float WALK_SPEED = 6.5f * 0.7f;							// 歩く速さ
	const float RUN_SPEED = 9.5f * 0.7f;							// 走る速さ
	const float SEARCH_RANGE_TO_BELL = 1000.0f;						// ベルの音に反応する範囲
	const float SEARCH_RANGE_TO_PLAYER_FOOTSTEP = 300.0f;			// プレイヤーの足音に反応する範囲
	const float SCREAM_VOLUME = 1.5f;								// 咆哮の音量
	const float RANGE_OF_OUTLINE_DRAWING_BY_FOOTSTEP = 45.0f;		// 足音による輪郭線の描画範囲
	const float ANIMATION_SPEED = 0.8f;								// アニメーションスピード
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;			// 歩き中の目標地点までの距離
	const float ENEMY_RADIUS = 30.0f;								// エネミーの半径
	const float ENEMY_HEIGHT = 200.0f;								// エネミーの高さ
	const float PATH_FINDING_TIMER = 0.7f;							// ナビメッシュによるパスを検索する時間の間隔
	const float ATTACKING_RANGE = 120.0f;							// 攻撃可能な距離
	const float MINIMUM_CHASE_TIME = 20.0f;							// 最低限追跡する時間
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;			// 期間中の目標地点までの距離
	const float TIME_TO_LOSE_SIGHT = 5.0f;							// プレイヤーを見失った時間
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;			// アニメーションの補間時間
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;				// 強制的にステート遷移する時間
	const int	EDGE_COLOR_RED = 1;									// 輪郭線の色
	const int	ENEMY_WALK_SOUND_NUMBER_TO_REGISTER = 2;			// 登録するエネミーの歩きの音の番号
	const int	ENEMY_RUN_SOUND_NUMBER_TO_REGISTER = 3;				// 登録するエネミーの走りの音の番号
	const int	ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER = 4;			// 登録するエネミーの咆哮の音の番号
	const float ENEMY_FOOTSTEP_SOUND_MAXIMUM_VOLUME = 1.0f;			// 足音の最大音量
	const float ENEMY_FOOTSTEP_SOUND_MINIMUM_VOLUME = 0.0f;			// 足音の最小音量
	const float FOOTSTEP_RANGE_TO_PLAYER = 1350.0f;					// 足音がプレイヤーに届く範囲
	const float TIME_TO_END_SCREAM_PER_SEC = 3.0f;					// 咆哮が終了するまでの時間
	const int   MATCH_ANIMATION_EVENT_NAME = 0;						// アニメーションイベントの名前が一致しているか
	const int	ENEMY_SOUND_PRIORITY = 0;							// エネミーに関する音の実行優先順位(早め)
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
	// アニメーション
	// 歩き時のアニメーションをロード(ループする)。
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	// 走り時のアニメーションをロード(ループする)。
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	// 叫び時のアニメーションをロード(ループしない)。
	m_animationClips[enAnimationClip_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Scream].SetLoopFlag(false);
	// 見回し時のアニメーションをロード(ループする)。
	m_animationClips[enAnimationClip_Survey].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Survey].SetLoopFlag(true);
	// 攻撃時のアニメーションをロード(ループする)。
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);

	// エネミーのモデルを初期化。
	m_enemyModel.Init(
		"Assets/modelData/enemy/enemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ,
		0,
		EDGE_COLOR_RED,
		m_edgeManagement->GetEdgeControl()
	);

	// アニメーションイベントを追加
	m_enemyModel.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		// 足音
		OnStepAnimationEvent(clipName, eventName);
		});

	// ファイルパスの文字列を作成
	std::string filePath;
	filePath = "Assets/enemypath/enemypath";
	filePath += std::to_string(m_enemyNumber);
	filePath += ".tkl";
	// 作成されたファイルパスをもとに初期化。
	m_enemyPath.Init(filePath.c_str());
	// 最初のパス移動のポイント情報を取得。
	m_point = m_enemyPath.GetFirstPoint();

	// モデルの初期の座標、回転、拡大率を設定。
	m_enemyModel.SetTRS(
		m_position,
		m_rotation,
		m_scale
	);

	// プレイヤーのインスタンスを検索。
	m_player = FindGO<Player>("player");
	// ベルのインスタンスを検索。
	m_bell = FindGO<Bell>("bell");
	// ゲームを総括するオブジェクトのインスタンスを検索
	m_game = FindGO<Game>("game");
	// ナビゲーションメッシュの初期化。
	m_nvmMesh.Init("Assets/nvm/stage2_mesh.tkn");
	
	// アニメーションのスピードを設定。
	m_enemyModel.SetAnimationSpeed(ANIMATION_SPEED);

	// 歩きの音の登録。
	g_soundEngine->ResistWaveFileBank(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER, "Assets/sound/enemy/walk1.wav");
	// 走りの音の登録。
	g_soundEngine->ResistWaveFileBank(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER, "Assets/sound/enemy/run2.wav");
	// 咆哮の音の登録。
	g_soundEngine->ResistWaveFileBank(ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER, "Assets/sound/enemy/scream.wav");
	
	// 初期ステートを歩きに指定
	m_iEnemyState = new EnemyWalkState( this );

	return true;
}

void Enemy::Update()
{
	// 回転
	Rotation();
	// ステートによる処理
	ProcessByState();
	// ステート管理
	ManageState();
	// アニメーション再生
	PlayAnimation();
	// 座標の更新。
	m_enemyModel.SetPosition(m_position);
	// モデルの更新。
	m_enemyModel.Update();
}

void Enemy::Rotation()
{
	// 回転角度を設定
	float angle = atan2(-m_moveVector.x, m_moveVector.z);

	// 回転を設定
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
	// 索敵範囲
	float searchRange = 0.0f;

	// プレイヤーの座標
	Vector3 playerPos = m_player->GetPosition();
	// プレイヤーとの距離
	Vector3 distance = m_position - playerPos;

	// ベルが鳴っているならば。
	if (m_bell->IsBellRinging()) {
		// 索敵範囲をベル用に設定。
		searchRange = SEARCH_RANGE_TO_BELL;
	}
	// 足音が鳴っているならば。
	else if (m_player->IsPlayerFootstepSound()) {
		// 索敵範囲をプレイヤーの足音用に設定。
		searchRange = SEARCH_RANGE_TO_PLAYER_FOOTSTEP;
	}
	
	// プレイヤーとの距離が索敵範囲内ならば。
	if (distance.Length() <= searchRange) {
		// プレイヤーを発見した。
		m_isFound = true;
		return;
	}

	// ここまで来たならば発見していない。
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
	//キーの名前が「walk1」の時。
	if (wcscmp(eventName, L"walk1") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「walk2」の時。
	else if (wcscmp(eventName, L"walk2") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「run1」の時。
	else if (wcscmp(eventName, L"run1") == MATCH_ANIMATION_EVENT_NAME) {
		m_stepSound = NewGO<SoundSource>(ENEMY_SOUND_PRIORITY);
		m_stepSound->Init(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「run2」の時。
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

	//プレイヤーとエネミーの距離
	Vector3 diff = m_position - playerPos;

	// 音の大きさ
	float soundLevel = ENEMY_FOOTSTEP_SOUND_MAXIMUM_VOLUME - (diff.Length() / FOOTSTEP_RANGE_TO_PLAYER * ENEMY_FOOTSTEP_SOUND_MAXIMUM_VOLUME);
	// 最小音量より小さくなったら固定する
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