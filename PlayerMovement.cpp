#include "PlayerMovement.h"
#include "Numbers.h"

PlayerMovement::PlayerMovement()
{
	input_ = Input::GetInstance();
}

PlayerMovement::~PlayerMovement()
{
}

Vector3 PlayerMovement::Move(const WorldTransform& worldTransform)
{
	playerAllMoveMent = Vec3Number(fNumbers::fZero);

	if ( pushKey.isPushMoveKey )
	{
		playerAllMoveMent = worldTransform.LookVelocity.look.norm();

		if ( !isRotFinish )
		{
			playerAllMoveMent = worldTransform.LookVelocity.look.norm() * PlayerTrunSpeed;
		}
	}
	else
	{
		if ( input_->PushKey(DIK_W) )
		{
			pushKey.isPushW = true;
			pushKey.isPushMoveKey = true;
			playerAllMoveMent = worldTransform.LookVelocity.look.norm();
		}
		if ( input_->PushKey(DIK_S) )
		{
			pushKey.isPushS = true;
			pushKey.isPushMoveKey = true;
			playerAllMoveMent = worldTransform.LookVelocity.lookBack.norm();
		}
		if ( input_->PushKey(DIK_A) )
		{
			pushKey.isPushA = true;
			pushKey.isPushMoveKey = true;
			playerAllMoveMent = worldTransform.LookVelocity.lookLeft.norm();
		}
		if ( input_->PushKey(DIK_D) )
		{
			pushKey.isPushD = true;
			pushKey.isPushMoveKey = true;
			playerAllMoveMent = worldTransform.LookVelocity.lookRight.norm();
		}

		if ( input_->PushKey(DIK_W) == true && input_->PushKey(DIK_A) == true )
		{
			playerAllMoveMent = worldTransform.LookVelocity.look_lookLeft.norm();
		}
		if ( input_->PushKey(DIK_W) == true && input_->PushKey(DIK_D) == true )
		{
			playerAllMoveMent = worldTransform.LookVelocity.look_lookRight.norm();
		}
		if ( input_->PushKey(DIK_S) == true && input_->PushKey(DIK_A) == true )
		{
			playerAllMoveMent = worldTransform.LookVelocity.lookBack_lookLeft.norm();
		}
		if ( input_->PushKey(DIK_S) == true && input_->PushKey(DIK_D) == true )
		{
			playerAllMoveMent = worldTransform.LookVelocity.lookBack_lookRight.norm();
		}
	}
	
	return playerAllMoveMent;
}

void PlayerMovement::PlayerAngle(const bool& isAtack)
{
	pushKey.isPushW = false;
	pushKey.isPushA = false;
	pushKey.isPushS = false;
	pushKey.isPushD = false;
	pushKey.isPushMoveKey = false;
	isRotFinish = false;

	if ( !isAtack )
	{
		if ( input_->PushKey(DIK_W) )
		{
			pushKey.isPushW = true;
			pushKey.isPushMoveKey = true;
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.Front);
		}
		if ( input_->PushKey(DIK_S) )
		{
			pushKey.isPushS = true;
			pushKey.isPushMoveKey = true;
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.Back);
		}
		if ( input_->PushKey(DIK_A) )
		{
			pushKey.isPushA = true;
			pushKey.isPushMoveKey = true;
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.Left);
		}
		if ( input_->PushKey(DIK_D) )
		{
			pushKey.isPushD = true;
			pushKey.isPushMoveKey = true;
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.Right);
		}

		if ( input_->PushKey(DIK_W) == true && input_->PushKey(DIK_A) == true )
		{
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.LeftDiagonal);
		}
		if ( input_->PushKey(DIK_W) == true && input_->PushKey(DIK_D) == true )
		{
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.FrontDiagonal);
		}
		if ( input_->PushKey(DIK_S) == true && input_->PushKey(DIK_A) == true )
		{
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.BackDiagonal);
		}
		if ( input_->PushKey(DIK_S) == true && input_->PushKey(DIK_D) == true )
		{
			isRotFinish = true;
			PlayerAngleSetter(playerMoveRot.RightDiagonal);
		}
	}
	else
	{
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.Front);
	}
}

void PlayerMovement::PlayerAngleSetter(const float& angle)
{
	float RotAngle = ( PlayerMoveRotation - angle ) + playerMoveRot.Back;
	if ( RotAngle >= FloatNumber(fNumbers::fZero) )
	{
		if ( PlayerMoveRotation - angle > playerMoveRot.Back )
		{
			PlayerMoveRotation += playerMoveRot.AddRot;
			if ( PlayerMoveRotation >= playerMoveRot.AllRot )
			{
				isRotFinish = false;
				PlayerMoveRotation -= playerMoveRot.AllRot;
			}
		}
		else
		{
			if ( PlayerMoveRotation < angle )
			{
				isRotFinish = false;
				PlayerMoveRotation += playerMoveRot.AddRot;
			}
			else if ( PlayerMoveRotation > angle )
			{
				isRotFinish = false;
				PlayerMoveRotation -= playerMoveRot.AddRot;
			}
		}
	}
	else
	{
		isRotFinish = false;
		PlayerMoveRotation = playerMoveRot.AllRot - playerMoveRot.AddRot;
	}
}

bool PlayerMovement::GetIsRotFinish()
{
	return isRotFinish;
}

bool PlayerMovement::GetIsPushMoveKey()
{
	return pushKey.isPushMoveKey;
}

float PlayerMovement::GetPlayerAngle()
{
	return PlayerMoveRotation;
}

PushKey PlayerMovement::GetPushedKey()
{
	return pushKey;
}
