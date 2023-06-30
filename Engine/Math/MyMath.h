#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include "WinApp.h"
#include <DirectXMath.h>
#include <Vector4.h>
#include<assimp/scene.h>
#include "Quaternion.h"

//便利系まとめ
namespace MyMath {

	const float PI = 3.141592f;

	Matrix4 Initialize();
	Matrix4 Scale(const Vector3& scale);
	Matrix4 Rotation(const Vector3& rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Translation(const Vector3& move);

	//ベクトルの正規化(いっぺんにやる)
	Vector3 vector3Normalize(const Vector3& v);
	//ベクトルと行列の掛け算(出力Vector3)
	Vector3 MatVector(Matrix4 matrix4, Vector3 vector3);
	Vector3 MulVector3(Vector3 vector3, Vector3 s);

	const Vector3 SubVec(Vector3 v, Vector3 v2);

	Vector3 GetWorldTransform(const Matrix4& matrix4);
	const Vector3 AddVector3(const Vector3 v1, const Vector3 v2);
	Vector3 DivVecMat(const Vector3& vector3, const Matrix4& matrix4);


	Vector4 Vec4Mat4Mul(const Vector4& vec, const Matrix4& mat);

	Matrix4 setViewportMat(WinApp* window, const Vector3& v);
	Matrix4 MatrixInverse(Matrix4 pOut);
	Matrix4 MakeInverse(const Matrix4* mat);

	Matrix4 ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix);

	Matrix4 LookAtLH(Vector3 eye, Vector3 target, Vector3 up);
	Matrix4 PerspectiveFovLH(float fovAngleY, float  aspectRatio, float  nearZ, float farZ);

	Matrix4 MakeIdentity();

	// 値を範囲内に納める
	float Clamp(float Value, const float low, const float high);

	Matrix4 Matrix4Orthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);

	Matrix4 AssimpMatrix(aiMatrix4x4 mat4);

	Vector3 AssimpVector3(aiVector3D assimpVec3);

	Vector4 AssimpQuaternionVec4(aiQuaternion assimpaiQuaternion);
	Quaternion AssimpQuaternion(aiQuaternion assimpaiQuaternion);

	Vector4 QuaternionSlerp(aiQuaternion& assimpaiQuaternion, aiQuaternion& assimpaiQuaternion2, float t);

	float GetAngle(const float& angle);

	float GetRadAngle(float angle);

	void MatrixText(Matrix4& mat);


};