#pragma once

#include "EnemyPath.h"
#include "AI/PathFinding/PathFinding.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "sound/SoundSource.h"


class Player;
class Bell;
class EdgeManagement;
class Game;
class Enemy : public IGameObject
{
public:
	Enemy() {};
	~Enemy();
	
	/// <summary>
	/// 開始処理。
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// エネミーの番号を設定
	/// </summary>
	/// <param name="number">番号</param>
	void SetNumber(const int& number)
	{
		m_enemyNumber = number;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// 輪郭線に影響を与える情報を設定。
	/// </summary>
	/// <param name="edgeManagement">輪郭線に影響を与える情報</param>
	const void SetEdgeManagement(EdgeManagement* edgeManagement)
	{
		m_edgeManagement = edgeManagement;
	}
	/// <summary>
	/// 咆哮をしているか。
	/// </summary>
	/// <returns></returns>
	const bool IsScream()
	{
		return m_isScream;
	}
	/// <summary>
	/// 動いているか。
	/// </summary>
	/// <returns></returns>
	const bool IsMoving()
	{
		return m_isMove;
	}
	const bool IsAttacking()
	{
		return m_isAttackable;
	}
private:
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// ステートによる処理
	/// </summary>
	void ProcessByState();
	/// <summary>
	/// 通常移動
	/// </summary>
	void Walk();
	/// <summary>
	/// 咆哮
	/// </summary>
	void Scream();
	/// <summary>
	/// 追跡
	/// </summary>
	void Chase();
	/// <summary>
	/// 見回し
	/// </summary>
	void Survey();
	/// <summary>
	/// パス移動への帰還
	/// </summary>
	void ReturnToPath();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// 歩き時のステート遷移
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 追跡時のステート遷移
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 攻撃時のステート遷移
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 叫び時のステート遷移
	/// </summary>
	void ProcessScreamStateTransition();
	/// <summary>
	/// 見回し時のステート遷移
	/// </summary>
	void ProcessSurveyStateTransition();
	/// <summary>
	/// パス移動への帰還時のステート遷移
	/// </summary>
	void ProcessReturnToPathStateTransition();
	/// <summary>
	/// アニメーション再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// プレイヤーの音を探索
	/// </summary>
	void SearchSoundOfPlayer();
	/// <summary>
	/// 足音のアニメーションイベント。
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// 足音の音量制御
	/// </summary>
	void StepVolumeControl();
private:
	// ステート
	enum EnEnemyState {
		enEnemyState_Walk,				// 歩き	
		enEnemyState_Scream,			// 咆哮
		enEnemyState_Chase,				// 追跡
		enEnemyState_Survey,			// 見回し
		enEnemyState_ReturnToPath,		// パス移動に戻る
		enEnemyState_Attack,			// 攻撃
		enEnemyState_Num,				// ステートの数
	};

	// アニメーション
	enum EnAnimationClip {
		enAnimationClip_Walk,			// 歩き
		enAnimationClip_Run,			// 走り
		enAnimationClip_Scream,			// 咆哮
		enAnimationClip_Survey,			// 見回し
		enAnimationClip_Attack,			// 攻撃
		enAnimationClip_Num,			// アニメーションの数
	};

	Vector3					m_position = Vector3::Zero;					// 座標
	Quaternion				m_rotation = Quaternion::Identity;			// 回転
	Vector3					m_scale = Vector3::One * 5.0f;				// 拡大率
	int						m_enemyNumber;								// レベル上で登録されたエネミーの番号
	AnimationClip			m_animationClips[enAnimationClip_Num];		// アニメーションクリップ
	Vector3					m_moveVector;								// 移動する方向
	Vector3					m_moveSpeed;								// 移動する速さ
	EnEnemyState			m_enemyState = enEnemyState_Walk;			// ステート
	ModelRender				m_enemyModel;								// エネミーのモデル
	EnemyPath				m_enemyPath;								// パス
	Point*					m_point = nullptr;							// パス移動のポイント
	Player*					m_player = nullptr;							// プレイヤークラス
	Bell*					m_bell = nullptr;							// ベルクラス
	nsAI::NaviMesh			m_nvmMesh;									// ナビゲーションメッシュ
	nsAI::Path				m_path;										// ナビゲーションメッシュ用パス
	nsAI::PathFinding		m_pathFinding;								// ナビゲーションメッシュ用パス検索
	float					m_animationSpeed;							// アニメーションのスピード
	float					m_pathFindingTimer = 0.0f;					// 目的地までのパスを検索する間の時間
	Vector3					m_lastPosition;								// 直前の座標
	float					m_chaseTime;								// 追いかけ続ける時間
	bool					m_isAttackable = false;						// プレイヤーへの攻撃は可能か
	bool					m_isFound = false;							// プレイヤーを見つけているか
	float					m_surveyTimer = 0.0f;						// 見回す時間
	bool					m_isScream = false;							// 咆哮しているか
	bool					m_isMove = false;							// 動いているか
	float					m_screamRateByTime = 0.0f;					// 咆哮の時間による影響率
	float					m_timeToReturn = 0.0f;						// 帰還する時間
	bool					m_isPathFindingWhileReturning = false;		// パス移動への帰還時にパス検索をしたか
	float					m_attackingTimer = 1.5f;					// 攻撃の演出までの時間
	bool					m_isCaughtPlayer = false;					// プレイヤーを捕まえているか
	EdgeManagement*			m_edgeManagement = nullptr;					// 輪郭線情報
	SoundSource*			m_stepSound = nullptr;
	SoundSource*			m_screamSound = nullptr;
	Game*					m_game = nullptr;



};

