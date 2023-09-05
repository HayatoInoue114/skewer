#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void GameScene::Initialize() {
	XINPUT_STATE joyState;
	switch (gameScene)
	{
	case 0:

		break;
	case 1:
		dxCommon_ = DirectXCommon::GetInstance();
		input_ = Input::GetInstance();
		audio_ = Audio::GetInstance();

		//レティクルのテクスチャ
		TextureManager::Load("reticle.png");

		// ファイル名を指定してテクスチャを読み込む
		soundDataHandle_ = audio_->LoadWave("Apocalypse_Doors.wav");

		modelPlayer_ = Model::CreateFromOBJ("player", true);
		// 3Dモデルの生成
		model = Model::Create();

		worldTransform_.Initialize();

		// 3Dモデルの生成
		modelSkydome_ = Model::CreateFromOBJ("skydome", true);

		// ビュープロジェクションの初期化
		viewProjection.farZ = 500;
		viewProjection.Initialize();

		// レールカメラの生成
		railCamera_ = new RailCamera();
		// レールカメラの初期化
		railCamera_->Initialize({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

		// 自キャラの生成
		player_ = new Player();
		Vector3 playerPosition(0, 0, 50.0f);
		// 自キャラの初期化
		player_->Initialize(modelPlayer_, playerPosition);

		// デバッグカメラの生成
		debugCamera_ = new DebugCamera(1280, 720);

		// 軸方向の表示を有効にする
		AxisIndicator::GetInstance()->SetVisible(true);
		// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
		AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);

		// 敵の生成
		/*EnemySpawn({0.5f, 0.0f, 60.0f});*/

		LoadEnemyPopData();

		// 天球の生成
		skydome_ = new Skydome();
		// 天球の初期化
		skydome_->Initialize(modelSkydome_);

		// 自キャラとレールカメラの親子関係を結ぶ
		player_->SetParent(&railCamera_->GetWorldTransform());

		FireAndResetCallback();

		countDown = 0;

		gameScene = 0;

		audio_->PlayWave(soundDataHandle_);

		audio_->SetVolume(soundDataHandle_, 0.1f);
		break;
	}
	
}

void GameScene::Update() {
	//ゲームパッドの状態を得る変数（XINPUT）
	
	switch (gameScene)
	{
	case 0:
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			gameScene = 1;
		}
		break;
	case 1:
		countDown++;
		viewProjection.UpdateMatrix();
		// レールカメラの更新
		railCamera_->Update();
		viewProjection.matView = railCamera_->GetViewProjection().matView;
		viewProjection.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection.TransferMatrix();



		// 敵の発生処理
		UpdateEnemyPopCommands();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			if (enemy->GetIsAlive()) {
				enemy->Update();
			}
		}

		// 自キャラの更新
		player_->Update(viewProjection);



		// 範囲for文でリストの全要素について回す
		for (TimedCall* timedCall : timedCalls_) {
			timedCall->Update();
		}

		// 終了したタイマーを削除
		timedCalls_.remove_if([](TimedCall* timedCall) {
			if (timedCall->IsFinished()) {
				delete timedCall;
				return true;
			}
			return false;
			});

		// デスフラグの立った弾を削除
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
			});

		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->SetPlayer(player_);

			bullet->Update();
		}

		// 当たり判定
		CheckAllCollisions();

		// 天球の更新
		skydome_->Update();

		// デバッグカメラの更新
		//	debugCamera_->Update();
		// #ifdef _DEBUG
		//	if (input_->TriggerKey(DIK_LALT)) {
		//		if (isDebugCameraActive_ == false) {
		//			isDebugCameraActive_ = true;
		//		} else {
		//			isDebugCameraActive_ = false;
		//		}
		//
		//	}
		// #endif // DEBUG
		//
		//	//カメラの処理
		//	if (isDebugCameraActive_) {
		//		debugCamera_->Update();
		//		viewProjection.matView = debugCamera_->GetViewProjection().matView;
		//		viewProjection.matProjection = debugCamera_->GetViewProjection().matProjection;
		//		//ビュープロジェクション行列の転送
		//		viewProjection.TransferMatrix();
		//	} else {
		//		//ビュープロジェクション行列の更新と転送
		//		viewProjection.UpdateMatrix();
		//	}
		break;
	}

	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	switch (gameScene)
	{
	case 0:
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			gameScene = 1;
		}
		break;
	case 1:
		break;
	}
	
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw(viewProjection);

	player_->Draw(viewProjection);

	
	for (Enemy* enemy : enemies_) {
		if (enemy->GetIsAlive()) {
			enemy->Draw(viewProjection);
		}
	}

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		Vector3 distance = Subtract(posA, posB);
		if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <= 3 * 3) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemies_) {
		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPosition();

			Vector3 distance = Subtract(posA, posB);
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <=
			    3 * 3) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* eBullet : enemyBullets) {
			posA = playerBullet->GetWorldPosition();
			posB = eBullet->GetWorldPosition();

			Vector3 distance = Subtract(posA, posB);
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <=
			    3 * 3) {
				playerBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemhyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemhyBullet);
}

void GameScene::EnemySpawn(Vector3 position) {
	Enemy* newEnemy = new Enemy();
	newEnemy->SetPlayer(player_);
	newEnemy->Initialize(model, position);
	enemies_.push_back(newEnemy);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources./csv./enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaiting_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWaiting_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数名
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の戦闘文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemySpawn(Vector3(x, y, z));

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyFire() {
	// 差分ベクトルを求める
	for (Enemy* enemy : enemies_) {
		if (enemy->isFire && enemy->GetIsAlive()) {
			assert(player_);

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity_ = { 1, 1, kBulletSpeed };

			velocity_ = Subtract(player_->GetWorldPosition(), enemy->GetWorldPosition());
			// ベクトルの正規化
			velocity_ = Normalize(velocity_);

			// ベクトルの長さを速さに合わせる
			velocity_ = Multiply(kBulletSpeed, velocity_);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);
			// 弾を生成し、初期化
			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize(model, enemy->GetWorldPosition(), velocity_);
			// 弾を登録する
			AddEnemyBullet(newBullet);
		}
	}
}



void GameScene::FireAndResetCallback() {
	EnemyFire();

	timedCalls_.push_back(
	    new TimedCall(std::bind(&GameScene::FireAndResetCallback, this), kFireInterval));
}

