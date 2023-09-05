#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <sstream>
#include "TimedCall.h"

class Enemy;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定を応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	// 敵を発生させる
	void EnemySpawn(Vector3 position);

	/// <summary>
	/// 敵弾発射
	/// </summary>
	void EnemyFire();

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireAndResetCallback();
	
	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// //テクスチャハンドル
	

	// 3Dモデルデータ
	Model* model = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection;

	// 自キャラ
	Player* player_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;

	// 天球
	Skydome* skydome_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	bool isWaiting_ = false;
	int waitTimer_ = 0;

	// 弾を発射する間隔
	static const uint32_t kFireInterval = 60;
	// 次元発動のリスト
	std::list<TimedCall*> timedCalls_;

	Model* modelPlayer_ = nullptr;

	int countDown;

	int gameScene;

	uint32_t soundDataHandle_ = 0;

	uint32_t enemyFire_ = 0;
	uint32_t ex_ = 0;

	XINPUT_STATE joyState;
};
