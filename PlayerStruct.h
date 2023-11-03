#pragma once

struct PlayerMoveRot
{
	float Front = 0.0f;
	float Right = 90.0f;
	float Back = 180.0f;
	float Left = 270.0f;
	float FrontDiagonal = 45.0f;
	float RightDiagonal = 135.0f;
	float BackDiagonal = 225.0f;
	float LeftDiagonal = 315.0f;
	float AllRot = 360.0f;
	float AddRot = 15.0f;
};

struct PushKey
{
	bool isPushW = false;
	bool isPushA = false;
	bool isPushS = false;
	bool isPushD = false;
	bool isPushMoveKey = false;
};