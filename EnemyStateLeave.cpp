#include "EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetWorldPosition();
	if (position.x > 0) {
		acceleration.x = 0.01f;
	}
	if (position.x < 0) {
		acceleration.x = -0.01f;
	}
	if (position.y > 0) {
		acceleration.y = 0.01f;
	}
	if (position.y < 0) {
		acceleration.y = -0.01f;
	}
	
	acceleration.z = 0.01f;

	move = Add(move, acceleration);

	pEnemy->isFire = false;
	// 移動
	pEnemy->ChangePosition(move);
}