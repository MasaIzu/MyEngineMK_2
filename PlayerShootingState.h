#pragma once
#include"PlayerState.h"
#include"PlayerMovement.h"

class PlayerShootingState : public PlayerState
{
public:
	void Initialize() override;
	void Update(Player* player,PlayerMovement* playerMovement) override;
	void AttackUpdate(Player* player,PlayerMovement* playerMovement,const Vector3& EnemyPos) override;
private:

};