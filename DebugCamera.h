#pragma once
#include "ViewProjection.h"
#include "Vector2.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Input.h"

class DebugCamera {
public:

	void Initialize(ViewProjection* viewProjection);

	void Update();

private:
	void MouseRot();

	void MouseUpdate();

	void MoveVector(const Vector3& move);

private:

	Input* input_ = nullptr;

	ViewProjection* viewProjection;

	float CameraDistance = 5.5f;
	//カメラの移動の速さ
	const float cameraSpeed = 0.0005f;

	Matrix4 CameraRot;
	Vector3 rot;
	Vector2 MouseMove;
	Vector2 mouseMoved;

	// マウスの入力を取得
	Vector3 mouseMove;

	float scaleX = 0.0f;
	float scaleY = 0.0f;

	bool isAction = false;
	float angleX = 0;
	float angleY = 0;
};