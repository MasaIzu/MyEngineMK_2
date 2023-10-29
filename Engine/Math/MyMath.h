#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include "WinApp.h"

#include "Vector4.h"
#include "Quaternion.h"
#include "Uint32Vector2.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <DirectXMath.h>
MY_SUPPRESS_WARNINGS_END

//便利系まとめ
namespace MyMath {

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

	Vector3 Vec4ToVec3(const Vector4& vec4);

	Vector4 Vec3ToVec4(const Vector3& vec3);
	Vector4 Vec4Mat4Mul(const Vector4& vec, const Matrix4& mat);

	Matrix4 setViewportMat(WinApp* window, const Vector3& v);
	Matrix4 MatrixInverse(Matrix4 pOut);
	Matrix4 MakeInverse(const Matrix4* mat);

	Matrix4 ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix);

	Matrix4 LookAtLH(Vector3 eye, Vector3 target, Vector3 up);
	Matrix4 PerspectiveFovLH(float fovAngleY, float  aspectRatio, float  nearZ, float farZ);

	Matrix4 MakeIdentity();

	float JudgeLeftorRight(const Vector3& A,const Vector3& B,const Vector3& C);

	// 値を範囲内に納める
	float Clamp(float Value, const float low, const float high);

	Matrix4 Matrix4Orthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);

	//Matrix4 AssimpMatrix(aiMatrix4x4 mat4);

	//Vector3 AssimpVector3(aiVector3D assimpVec3);

	//Vector4 AssimpQuaternionVec4(aiQuaternion assimpaiQuaternion);
	//Quaternion AssimpQuaternion(aiQuaternion assimpaiQuaternion);

	//Vector4 QuaternionSlerp(aiQuaternion& assimpaiQuaternion, aiQuaternion& assimpaiQuaternion2, float t);

	//度数法からラジアン
	float GetAngle(const float& angle);
	Vector3 GetAngleVec3(const Vector3& angle);

	//ラジアンから度数法
	float GetRadAngle(float angle);

	void MatrixText(Matrix4& mat);

	Vector4 Vec4MulPs(const Vector4& v4_1,const Vector4& v4_2);
	Vector4 Vec4AddPs(const Vector4& v4_1,const Vector4& v4_2);

	//vec1を中心とした時のvec2の角度を出す
	float Get2VecAngle(const Vector3& vec1, const Vector3& vec2);
	float Get2VecAngleY(const Vector3& vec1, const Vector3& vec2);
	float Get2FloatAngle(const float& x, const float& y);
	float Distance2Vec2(const Vector2& vec1, const Vector2& vec2);

	Vector3 HorizontalProjection(const Vector3& startSpeed, float& g, float& flame);
	const Vector2 lerpVec2(const Vector2& start,const Vector2& end,const float t);
	const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

	//ランダム関係
	uint32_t Random(const uint32_t& low, const uint32_t& high);
	Vector3 RandomVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh);
	Vector3 RandomCenterVec3(const uint32_t& low, const uint32_t& high);
	Vector3 RandomCenterVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh);
	Vector3 RandomCenterVec3Normalize(const uint32_t& low, const uint32_t& high);
	Vector3 castVec(const uint32_t& x, const uint32_t& y, const uint32_t& z);

	constexpr float PI = 3.1415926535897931f;

};