#include "EnemyBullet.h"
#include "Player.h"

EnemyBullet::EnemyBullet() { 
	velocity_ = {};
	player_ = {};
	speed_ = {};
	t_ = {};
}

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("ball.png");

	worldTransform_.Initialize();

	// Z方向に伸びた形状
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {position};

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	t_ = 0.02f;

	speed_ = 0.8f;
}

void EnemyBullet::Update() {

	// 座標を移動させる（1フレーム分の移動量を足しこむ)
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);

	//ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);

	velocity_ = Multiply(speed_, Slerp(velocity_, toPlayer, t_));

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// Y軸周り角度(0y)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	velocityXZ_ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	// X軸周り角度(0x)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ_);

	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = {
	    worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	    worldTransform_.matWorld_.m[3][2]};
	return worldPos;
}

void EnemyBullet::OnCollision() { isDead_ = true; }