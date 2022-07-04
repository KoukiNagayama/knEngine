#include "stdafx.h"
#include "Enemy.h"
#include "Bell.h"
#include "Player.h"
#include "EdgeManagement.h"
#include "sound/SoundEngine.h"
#include "Game.h"


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
	switch (m_enemyState) {
		// 歩き状態
	case enEnemyState_Walk:
		ProcessWalkStateTransition();
		break;
		// 咆哮状態
	case enEnemyState_Scream:
		ProcessScreamStateTransition();
		break;
		// 追跡状態
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		// 見回し状態
	case enEnemyState_Survey:
		ProcessSurveyStateTransition();
		break;
		// パス移動への帰還状態
	case enEnemyState_ReturnToPath:
		ProcessReturnToPathStateTransition();
		break;
		// 攻撃状態
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Enemy::ProcessByState()
{
	switch (m_enemyState) {
		// 歩き状態
	case enEnemyState_Walk:
		Walk();
		SearchSoundOfPlayer();
		break;
		// 咆哮状態
	case enEnemyState_Scream:
		Scream();
		break;
		// 追跡状態
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
		// 見回し状態
	case enEnemyState_Survey:
		Survey();
		SearchSoundOfPlayer();
		break;
		// パス移動への帰還状態
	case enEnemyState_ReturnToPath:
		ReturnToPath();
		SearchSoundOfPlayer();
		break;
		// 攻撃状態
	case enEnemyState_Attack:
		Attack();
		break;
	}
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

void Enemy::Walk()
{
	// 目的地までの距離
	Vector3 distance = m_point->s_position - m_position;

	// 目的地までの距離が近ければ
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
		// 現在最後のポイントにいるならば
		if (m_point->s_number == m_enemyPath.GetPointListSize() - 1) {
			// 最初のポイント情報を取得
			m_point = m_enemyPath.GetFirstPoint();
		}
		// 最後以外のポイントにいるならば
		else {
			// 次のポイント情報を取得
			m_point = m_enemyPath.GetNextPoint(m_point->s_number);
		}

	}

	// 移動する方向を設定
	m_moveVector = distance;
	m_moveVector.Normalize();

	// 歩き時の移動速度を設定
	m_moveSpeed = m_moveVector * WALK_SPEED;

	// 座標に移動速度を加算
	m_position += m_moveSpeed;
}

void Enemy::Scream()
{
	// 咆哮の音源を鳴らす
	if (m_screamSound == nullptr) {
		m_screamSound = NewGO<SoundSource>(0);
		m_screamSound->Init(ENEMY_SCREAM_SOUND_NUMBER_TO_REGISTER);
		m_screamSound->SetVolume(SCREAM_VOLUME);
		m_screamSound->Play(false);
		m_isScream = true;
	}

	if (m_enemyModel.IsPlayingAnimation() == false) {
		m_isScream = false;
		m_screamSound = nullptr;
	}
}


void Enemy::Chase()
{
	// ターゲットとなるプレイヤーの座標を取得
	Vector3	playerPos = m_player->GetPosition();

	if (m_pathFindingTimer <= 0.0f) {
		// プレイヤーまでのパスを検索
		m_pathFinding.Execute(
			m_path,
			m_nvmMesh,
			m_position,
			playerPos,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		// タイマーリセット
		m_pathFindingTimer = PATH_FINDING_TIMER;
	}

	m_pathFindingTimer -= g_gameTime->GetFrameDeltaTime();

	// 指定した座標への移動が完了したか
	bool isEnd;
	// 移動直前の座標を記録
	m_lastPosition = m_position;

	// 最低追跡する残り時間
	// 追跡する残り時間を減らす
	m_chaseTime -= g_gameTime->GetFrameDeltaTime();
	// 追跡する残り時間が0になったなら
	if (m_chaseTime < 0.0f) {
		m_chaseTime = 0.0f;
	}


	// 設定されたパスをもとに移動
	m_position = m_path.Move(
		m_position,
		RUN_SPEED,
		isEnd
	);

	// 方向
	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 momentDist = m_position - m_lastPosition;
	momentDist.Normalize();

	// 移動する方向を設定
	m_moveVector = momentDist;

	// プレイヤーとの距離
	Vector3 distanceToPlayer = m_position - playerPos;
	// プレイヤーとの距離が近ければ
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// 攻撃を行う
		m_isAttackable = true;
	}
}

void Enemy::Survey()
{
	// 見回した時間を加算
	m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void Enemy::ReturnToPath()
{
	bool isEnd;

	// 移動直前の座標を記録
	m_lastPosition = m_position;

	// パスを検索していないならば
	if (m_isPathFindingWhileReturning == false) {
		// 現在の座標から一番近い座標のポイントを取得
		m_point = m_enemyPath.GetNearPoint(m_position);
		// 帰還目標のパスまでのパスを検索
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

	// 設定されたパスをもとに移動
	m_position = m_path.Move(
		m_position,
		WALK_SPEED,
		isEnd
	);

	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 distance = m_position - m_lastPosition;
	distance.Normalize();

	// 移動する方向を設定
	m_moveVector = distance;
}

void Enemy::Attack()
{
	// ターゲットとなるプレイヤーの座標を取得
	Vector3	playerPos = m_player->GetPosition();

	Vector3 distance = playerPos - m_position;
	distance.Normalize();

	m_moveVector = distance;
}

void Enemy::ProcessWalkStateTransition()
{
	// 敵を発見していないならば
	if (m_isFound == false) {
		m_isMove = true;
		return;
	}
	// 咆哮をリセット
	m_screamRateByTime = 0.0f;
	m_isScream = true;
	m_isMove = false;
	// ステートを咆哮状態にする
	m_enemyState = enEnemyState_Scream;
}

void Enemy::ProcessScreamStateTransition()
{
	// 咆哮が終了していないならば
	if (m_isScream == true) {
		return;
	}
	m_isMove = true;
	// 最低限追跡する時間を指定
	m_chaseTime = MINIMUM_CHASE_TIME;
	// ステートを追跡状態にする
	m_enemyState = enEnemyState_Chase;
}

void Enemy::ProcessChaseStateTransition()
{
	// プレイヤーを攻撃可能な距離ならば
	if (m_isAttackable == true) {
		m_enemyState = enEnemyState_Attack;
		m_isMove = false;
		return;
	}
	// 敵を追跡する状態が維持されているならば
	if (m_chaseTime > 0.0f || m_isFound == true) {
		return;
	}
	// プレイヤーを見失っていたならば
	if (m_isFound == false) {
		// 見回す時間をリセット
		m_surveyTimer = 0.0f;
		// ステートを見回し状態にする
		m_enemyState = enEnemyState_Survey;
		// パスを検索する間隔をリセット
		m_pathFindingTimer = PATH_FINDING_TIMER;
		m_isMove = false;

	}

}

void Enemy::ProcessSurveyStateTransition()
{
	// 一定時間以内にプレイヤーを発見
	if (m_isFound == true) {
		// 最低限追跡する時間を指定
		m_chaseTime = MINIMUM_CHASE_TIME;
		// 追跡を開始
		m_enemyState = enEnemyState_Chase;

		m_isMove = true;
	}
	// 一定時間プレイヤーを見失っていたら
	if (m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// 帰還状態の時間を指定
		m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// ステートをパスへの帰還状態にする
		m_enemyState = enEnemyState_ReturnToPath;

		m_isMove = true;
	}

}

void Enemy::ProcessReturnToPathStateTransition()
{
	// 目標の座標と現在の座標の距離を測る
	Vector3 distance = m_point->s_position - m_position;

	m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// プレイヤーを発見したら
	if (m_isFound == true) {
		// 咆哮をリセット
		m_screamRateByTime = 0.0f;
		m_isScream = true;
		// ステートを咆哮状態にする
		m_enemyState = enEnemyState_Scream;

		m_isMove = false;
	}
	// 目標の座標に近くなったら
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// ステートを歩き状態にする
		m_enemyState = enEnemyState_Walk;

	}
	// 一定時間以内に帰還できなかった場合
	else if (m_timeToReturn <= 0.0f) {
		// 強制的にステートを歩き状態にする
		m_enemyState = enEnemyState_Walk;

	}
	m_isPathFindingWhileReturning = false;
}

void Enemy::ProcessAttackStateTransition()
{
	// ゲームオーバー
	m_attackingTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_attackingTimer <= 0.0f && m_isCaughtPlayer == false) {
		m_isCaughtPlayer = true;
		m_game->SetGameOver();
	}
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState) {
		// 歩き
	case enEnemyState_Walk:
		m_enemyModel.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// 咆哮
	case enEnemyState_Scream:
		m_enemyModel.PlayAnimation(enAnimationClip_Scream, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// 追跡
	case enEnemyState_Chase:
		m_enemyModel.PlayAnimation(enAnimationClip_Run, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// 見回し
	case enEnemyState_Survey:
		m_enemyModel.PlayAnimation(enAnimationClip_Survey, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// パスへの帰還
	case enEnemyState_ReturnToPath:
		m_enemyModel.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
		// 攻撃
	case enEnemyState_Attack:
		m_enemyModel.PlayAnimation(enAnimationClip_Attack, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	}
}

void Enemy::OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「walk1」の時。
	if (wcscmp(eventName, L"walk1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「walk2」の時。
	else if (wcscmp(eventName, L"walk2") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(ENEMY_WALK_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「run1」の時。
	else if (wcscmp(eventName, L"run1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(ENEMY_RUN_SOUND_NUMBER_TO_REGISTER);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「run2」の時。
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