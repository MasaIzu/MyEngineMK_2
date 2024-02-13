#pragma once
#include"PlayerState.h"

class PlayerMissileShootingState : public PlayerState
{
public:
	void Initialize() override;
	void Update(Player* player,const Vector3& EnemyPos) override;

private:

};