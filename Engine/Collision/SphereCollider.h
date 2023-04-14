#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include "Vector4.h"
#include "Matrix4.h"

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(Vector4 offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Matrix4& worldPos) override;

	inline void SetRadius(float radius) { this->radius = radius; }

	inline const Vector4& GetOffset() { return offset; }
	inline void SetOffset(const Vector4& offset) { this->offset = offset; }
	inline float GetRadius() { return radius; }

private:
	// オブジェクト中心からのオフセット
	Vector4 offset;
	// 半径
	float radius;
};

