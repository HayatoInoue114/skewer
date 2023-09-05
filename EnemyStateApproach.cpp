#include "EnemyStateApproach.h"
#include "Enemy.h"
#include "EnemyStateLeave.h"


void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 move = { 0.0f, 0, -0.1f };

	pEnemy->ChangePosition(move);
	pEnemy->isFire = true;
	//Vector3 position = pEnemy->GetWorldPosition();
	// 既定の位置に到達したら離脱
	if (pEnemy->IsLeaveChangeStatePosition()) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}