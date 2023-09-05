#include "EnemyStateEntry.h"
#include "Enemy.h"
#include "EnemyStateApproach.h"

void EnemyStateEntry::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetWorldPosition();

	
	if (position.x > 0) {
		acceleration.x = -0.01f;
	}
	if (position.x < 0) {
		acceleration.x = 0.01f;
	}
	if (position.y > 0) {
		acceleration.y = -0.01f;
	}
	if (position.y < 0) {
		acceleration.y = 0.01f;
	}

	move = Add(move, acceleration);

	pEnemy->isFire = false;
	pEnemy->ChangePosition(move);

	// 既定の位置に到達したら離脱
	if (pEnemy->IsApproachChangeStatePosition()) {
		pEnemy->ChangeState(new EnemyStateApproach());
	}
}