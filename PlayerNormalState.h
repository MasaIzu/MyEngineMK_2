#pragma once
#include"PlayerState.h"

//待機
class PlayerNormalState : public PlayerState
{
public:
	void Initialize()override;
	void Update(Player* player,const Vector3& EnemyPos) override;

private:

};