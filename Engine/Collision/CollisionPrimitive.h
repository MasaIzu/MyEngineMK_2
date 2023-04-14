#pragma once
#pragma once
/// <summary>
///当たり判定プリミティブ
/// </summary>


#include"Vector4.h"

struct Sphere {
	Vector4 center = { 0,0,0,1 };

	float radius = 1.0f;

};

struct Plane {
	Vector4 normal = { 0,1,0,0 };

	float distance = 0.0f;

};

class Triangle {
public:
	Vector4 p0;
	Vector4 p1;
	Vector4 p2;

	Vector4 normal;

	void ComputeNormal();

};

struct Ray
{
	Vector4 start = { 0,0,0,1 };

	Vector4 dir = { 1,0,0,0 };

};

