#pragma once
#include "EnemyBullet.h"
#include "IEnemyState.h"
#include "EnemyStateEntry.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "GameScene.h"
#include "MT.h"
#include "Model.h"
#include "TimedCall.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <assert.h>

class GameScene;
class Enemy;
class Player;
class IEnemyState;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/*/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void InitializeApproachPhase();*/

	/*/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireAndResetCallback();*/

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }

	//enum class Phase {
	//	Approach, // 接近する
	//	Leave,    // 離脱する
	//};

	/*void Approach();
	void Leave();*/
	void ChangeState(IEnemyState* newState);

	void ChangePosition(Vector3 vector);
	Vector3 GetTranslation() { return worldTransform_.translation_; }

	bool IsLeaveChangeStatePosition();
	
	bool IsApproachChangeStatePosition();

	void OnDeath();

	bool GetIsAlive() { return isAlive_; }

	/*bool PostDeath();*/

	bool isFire;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// フェーズ
	//Phase phase_ = Phase::Leave;

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	// メンバ関数ポインタ
	static void (Enemy::* situation[])();

	IEnemyState* state_;

	//// 弾を発射する間隔
	// static const uint32_t kFireInterval = 30;

	// 発射タイマー
	int32_t fireCount = 0;

	//// 次元発動のリスト
	// std::list<TimedCall*> timedCalls_;

	Player* player_ = nullptr;

	Vector3 velocity_;

	Vector3 deltaVector_;

	bool isAlive_;

	int health_;

	int reviveCount_;

	//敵がパターンを変えるPosition(Z)
	float changeStatePositionZ_;
};
