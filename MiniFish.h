#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include<memory>
#include"Boss.h"
#include"ViewProjection.h"

#include"BaseCollider.h"
#include <SphereCollider.h>

class MiniFish
{
private:
	std::unique_ptr<Model> bodyModel = nullptr;
	std::unique_ptr<Model> eyeModel = nullptr;

	WorldTransform world;	//ƒ[ƒ‹ƒh•ÏŠ·
	float direction;	//Œü‚«

	Vector3 positions[4];

	
	float spdPerSec = 120.0f;
	float moveRad = 0;

	int spdResetInterval = 2;
	int speedResetCount = 0;
	Vector3 move;
	float spdtemp = 1.0f;


	float fishRadius = 3;
	BaseCollider* collider;

	bool isAlive = false;

public:

	/// <summary>
/// ŠÔŒ‡ò‚É–ß‚éXV(ƒ`ƒ…[ƒgƒŠƒAƒ‹I—¹‚Ì‡})
/// </summary>
/// 

	EasingData easeMove;
	void LeaveGayser(Vector3 gayserPos);

	void Initialize(const Vector3& pos, unsigned short attribute);

	void Update(const Vector3& stagePos,float stageRadius);

	bool GetAlive()const { return isAlive; }

	void OnCollision();

	void Draw(ViewProjection viewPro);

	void SetMovePos(const Vector3& stagePos, float stageRadius);

	void SetAttribute(unsigned short attribute);

	Vector3 GetPosition()const { return world.translation_; }

	WorldTransform GetWorldTransform()const { return world; }



};

