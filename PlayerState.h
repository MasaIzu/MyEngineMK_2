#pragma once
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerMovement.h"

class Player;

class PlayerState
{
public:
	virtual void Initialize();
	std::string GetStateName() {
		return stateName;
	};
	virtual void Update(Player* player,PlayerMovement* playerMovement) = 0;
	virtual void AttackUpdate(Player* player,PlayerMovement* playerMovement,const Vector3& EnemyPos) = 0;
public:
	bool CanMove() {
		return canMove_;
	};

protected:
	Input* input_ = nullptr;
	bool canMove_;
	bool isAttack = false;
	std::string stateName;
};