#pragma once
#include"CollisionPrimitive.h"
#include "RaycastHit.h"

#include "QueryCallback.h"

#include"Matrix4.h"
#include <d3d12.h>
#include <forward_list>


class BaseCollider;

class CollisionManager {
public:
	static CollisionManager* GetInstance();

public://メンバ関数

	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	inline void RemoveCollider(BaseCollider* collide) {
		colliders.remove(collide);
	}

	inline void AllClearCollider() {
		colliders.clear();
	}

	void CheckAllCollisions();

	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);


	//レイキャストオーバーロード
	bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	void QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute = static_cast<unsigned short>(0xffffffff));


	bool GetIsEnemyHit() { return isEnemyHit; }
	bool GetIsAttackHit() { return isAttackHit; }
	bool GetIsWakeEnemyAttackHit() { return isWakeEnemyAttackHit; }
	int GetHitNumber() { return hitNumber; }
	Matrix4 GetEnemyWorldPos() { return EnemyWorldPos; }
	Matrix4 GetAttackHitWorldPos() { return HitWorldPos; }

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator = (const CollisionManager&) = delete;

	//コライダーのリスト
	std::forward_list<BaseCollider*> colliders;

	bool isEnemyHit = false;
	bool isAttackHit = false;
	int hitNumber = 0;
	bool isWakeEnemyAttackHit = false;
	Matrix4 EnemyWorldPos;
	Matrix4 HitWorldPos;
};