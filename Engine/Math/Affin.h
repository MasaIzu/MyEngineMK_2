#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "WorldTransform.h"

//ワールドトランスフォーム用
namespace AffinTrans {

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Translation(Vector3 move);
	void  affin(WorldTransform& affin);

}; // namespace AffinTrans