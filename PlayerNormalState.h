#pragma once
#include"PlayerState.h"
#include"PlayerMovement.h"
//待機
class PlayerNormalState : public PlayerState
{
public:
	void Initialize()override;
	void Update(Player* player,PlayerMovement* playerMovement) override;
	void AttackUpdate(Player* player,PlayerMovement* playerMovement,const Vector3& EnemyPos) override;
private:

};