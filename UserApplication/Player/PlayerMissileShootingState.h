#pragma once
#include"PlayerState.h"
#include"PlayerMovement.h"

class PlayerMissileShootingState : public PlayerState
{
public:
	void Initialize() override;
	void Update(Player* player,PlayerMovement* playerMovement) override;
	void AttackUpdate(Player* player,PlayerMovement* playerMovement,const Vector3& EnemyPos) override;
private:

};