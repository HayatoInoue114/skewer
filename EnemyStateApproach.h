#pragma once
#include "IEnemyState.h"

class Enemy;
// 近づく
class EnemyStateApproach : public IEnemyState {
public:
	void Update(Enemy* pEnemy) override;
};