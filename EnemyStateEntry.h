#pragma once
#include "IEnemyState.h"
#include "Vector3.h"
#include "MT.h"

class Enemy;

// 画面外から
class EnemyStateEntry : public IEnemyState {
public:
	void Update(Enemy* pEnemy) override;
private:
	Vector3 move = {};

	Vector3 acceleration = {};
};