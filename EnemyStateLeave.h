#pragma once
#include "IEnemyState.h"
#include "MT.h"


class Enemy;

// 逃げてく
class EnemyStateLeave : public IEnemyState {
public:
	void Update(Enemy* pEnemy) override;
private:
	Vector3 move = {};

	Vector3 acceleration = {};
};