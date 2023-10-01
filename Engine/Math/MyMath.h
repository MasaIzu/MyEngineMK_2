#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include "WinApp.h"
#include <DirectXMath.h>
#include <Vector4.h>
#include "Quaternion.h"
#include "Uint32Vector2.h"

#define PI 3.1415926534f

#define LENGTH      5.0                 // •R‚Ì’·‚³
#define CLENGTH     (LENGTH * 2.0 * PI)   // •R‚ğL‚Î‚µ‚Äˆêü‚³‚¹‚½ê‡‚Éo—ˆ‚é‰~‚Ì‰~ü‚Ì’·‚³
#define MASS        0.1346               // ‚Ô‚ç‰º‚ª‚Á‚Ä‚¢‚é•¨‚Ì¿—Ê
#define G           9.81                // d—Í‰Á‘¬“x

//•Ö—˜Œn‚Ü‚Æ‚ß
namespace MyMath {

	Matrix4 Initialize();
	Matrix4 Scale(const Vector3& scale);
	Matrix4 Rotation(const Vector3& rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Translation(const Vector3& move);

	//ƒxƒNƒgƒ‹‚Ì³‹K‰»(‚¢‚Á‚Ø‚ñ‚É‚â‚é)
	Vector3 vector3Normalize(const Vector3& v);
	//ƒxƒNƒgƒ‹‚Æs—ñ‚ÌŠ|‚¯Z(o—ÍVector3)
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

	// ’l‚ğ”ÍˆÍ“à‚É”[‚ß‚é
	float Clamp(float Value, const float low, const float high);

	Matrix4 Matrix4Orthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);

	//Matrix4 AssimpMatrix(aiMatrix4x4 mat4);

	//Vector3 AssimpVector3(aiVector3D assimpVec3);

	//Vector4 AssimpQuaternionVec4(aiQuaternion assimpaiQuaternion);
	//Quaternion AssimpQuaternion(aiQuaternion assimpaiQuaternion);

	//Vector4 QuaternionSlerp(aiQuaternion& assimpaiQuaternion, aiQuaternion& assimpaiQuaternion2, float t);

	float GetAngle(const float& angle);

	float GetRadAngle(float angle);

	void MatrixText(Matrix4& mat);

	//vec1‚ğ’†S‚Æ‚µ‚½‚Ìvec2‚ÌŠp“x‚ğo‚·
	float Get2VecAngle(const Vector3& vec1, const Vector3& vec2);
	float Get2FloatAngle(const float& x, const float& y);
	float Distance2Vec2(const Vector2& vec1, const Vector2& vec2);

	Vector3 HorizontalProjection(const Vector3& startSpeed, float& g, float& flame);

	const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

	//ƒ‰ƒ“ƒ_ƒ€ŠÖŒW
	uint32_t Random(const uint32_t& low, const uint32_t& high);
	Vector3 RandomVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh);
	Vector3 RandomCenterVec3(const uint32_t& low, const uint32_t& high);
	Vector3 RandomCenterVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh);
	Vector3 RandomCenterVec3Normalize(const uint32_t& low, const uint32_t& high);
	Vector3 castVec(const uint32_t& x, const uint32_t& y, const uint32_t& z);
};