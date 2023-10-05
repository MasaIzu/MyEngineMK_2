#include "BoxCollision.h"

bool BoxCollision::BoxCollision_1(Vector3 player, Vector3 enemy, Vector3 playerWidth, Vector3 enemyWidth)
{

	//プレイヤー
	float pXMin = player.x - playerWidth.x;

	float pXMax = player.x + playerWidth.x;

	float pYMin = player.y - playerWidth.y;

	float pYMax = player.y + playerWidth.y;

	float pZMin = player.z - playerWidth.z;

	float pZMax = player.z + playerWidth.z;

	//敵
	//平面
	float eXMin = enemy.x - enemyWidth.x;

	float eXMax = enemy.x + enemyWidth.x;

	float eYMin = enemy.y - enemyWidth.y;

	float eYMax = enemy.y + enemyWidth.y;

	float eZMin = enemy.z - enemyWidth.z;

	float eZMax = enemy.z + enemyWidth.z;

	if ((pXMin <= eXMax && pXMax >= eXMin) && (pYMin <= eYMax && pYMax >= eYMin) && (pZMin <= eZMax && pZMax >= eZMin)) {
		return true;
	}
	return false;
}

bool BoxCollision::BoxCollision_(Vector3 player, Vector3 playerWidth, Vector2 enemyPosX, Vector2 enemyPosY, Vector2 enemyPosZ)
{

	//プレイヤー
	float pXMin = player.x - playerWidth.x;

	float pXMax = player.x + playerWidth.x;

	float pYMin = player.y - playerWidth.y;

	float pYMax = player.y + playerWidth.y;

	float pZMin = player.z - playerWidth.z;

	float pZMax = player.z + playerWidth.z;

	//敵
	//平面
	float eXMin = enemyPosX.x;

	float eXMax = enemyPosX.y;

	float eYMin = enemyPosY.x;

	float eYMax = enemyPosY.y;

	float eZMin = enemyPosZ.x;

	float eZMax = enemyPosZ.y;

	if ((pXMin <= eXMax && pXMax >= eXMin) && (pYMin <= eYMax && pYMax >= eYMin) && (pZMin <= eZMax && pZMax >= eZMin)) {
		return true;
	}
	return false;
}

bool BoxCollision::BoxCollision_2(Vector3 player, Vector3 enemy, Vector3 playerWidth, Vector3 enemyWidth)
{

	///プレイヤー
	float pXMin = player.x - playerWidth.x;

	float pXMax = player.x + playerWidth.x;

	float pYMin = player.y - playerWidth.y;

	float pYMax = player.y + playerWidth.y;

	float pZMin = 0;

	float pZMax = player.z + playerWidth.z;

	//敵
	//平面
	float eXMin = enemy.x - enemyWidth.x;

	float eXMax = enemy.x + enemyWidth.x;

	float eYMin = enemy.y - enemyWidth.y;

	float eYMax = enemy.y + enemyWidth.y;

	float eZMin = enemy.z - enemyWidth.z;

	float eZMax = enemy.z + enemyWidth.z;

	if ((pXMin <= eXMax && pXMax >= eXMin) && (pYMin <= eYMax && pYMax >= eYMin) && (pZMin <= eZMax && pZMax >= eZMin)) {
		return true;
	}
	return false;
}


void BoxCollision::Contact(int playerMove, WorldTransform enemy,Vector3 enemyRotationMove, Vector3 enemyMove)
{
	if (playerMove == 1) {
		enemy.rotation_ += enemyRotationMove;
		enemy.translation_ += enemyMove;
	}
	else if (playerMove == 0) {
		enemy.translation_ += enemyMove;
	}
	else if (playerMove == -1) {
		enemy.rotation_ += enemyRotationMove;
		enemy.translation_ -= enemyMove;
	}
}
