#pragma once

class Enemy;

class IEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
};
