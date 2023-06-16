#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <Input.h>
#include "ViewProjection.h"
#include "Easing.h"

class GameCamera {

public:

	GameCamera(ViewProjection* viewPro,float& window_width, float& window_height);

	~GameCamera();

	void Initialize();
	void InitializeCameraPosition();

	void Update();

	void PlaySceneCamera();

	void MultiplyMatrix(Matrix4& matrix);

	void Collision();

	void Reset();

	void MousePositionReset();

private:

	void CameraAngle(float& x, float& z);

public://�Q�b�^�[�Z�b�^�[

	Vector3 GetEye();
	Vector3 GetUp() { return vUp; }
	Vector3 GetTarget() { return target; }
	Matrix4 GetCameraRot() { return this->CameraRot; }
	Vector3 GetCameraRotVec3() { return this->rot; }
	float GetFovAngle() { return MyMath::GetAngle(Fov); }
	// �J�����̐��������̊p�x���v�Z����֐�
	float getPitch() {
		return -atan2(playerPos_.y - viewProjection_->eye.y, playerCameraDistance);
	}
	float GetCameraAngle() { return angle; }
	// �J�����̐��������̊p�x���v�Z����֐�
	float getYaw(Vector3 position, Vector3 lookAt) {
		Vector3 direction = lookAt - position;
		direction.norm();
		float playerYaw = atan2(direction.x, direction.z);
		return playerYaw;
	}

	void SetCameraPosition(Vector3 pos) { playerPos_ = pos; }
	void SetSpaceInput(bool isSpaceInput) { spaceInput = isSpaceInput; }
private:

	Easing* easing_;

	// �J���������_�܂ł̋���
	Vector2 mousePos = { 0.0f,0.0f };
	Vector2 oldMousePos = { 0.0f,0.0f };
	Input* input_ = nullptr;

	// ��]�s��
	Matrix4 matRot;
	Vector3 rot;

	bool spaceInput = false;

	// �X�P�[�����O
	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;

	Vector3 vUp;
	Vector3 playerPos_;
	Vector3 target;

	bool cameraMode = false;

	int cameraType = 0;

	int winWidth = 0;
	int winHeight = 0;
	Vector2 MouseMove;
	Vector2 mouseMoved;

	float angleAroundPlayer; // �v���C���[�̎������]����p�x
	Matrix4 CameraRot;


	float playerCameraDistance = 5.5f;

	int cameraTime = 0;
	int MaxCameraTime = 0;


	//�J�������[�h(tekito-)
	int cameraMode_ = 0;
	//�J���������֌W
	const float  MAX_CAMERA_DISTANCE = 20.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;

	bool isShake = false;
	int shakeTime = 0;

	float angle = 0.0f;

	//�x���J����
	// �J�����̌��݂̈ʒu
	Vector3 cameraPos;

	// �J�����̑��x
	float cameraSpeed_ = 3;

	// �J�������ǐՂ���ۂ̒x����
	float cameraDelay = 0.1f;

	float cameraDis = 45.0f;

	Vector3 oldCameraPos;

	Vector3 LatePlayerPos = { 0,0,0 };
	float playerCameraDelay = 0.1f;
	float playerCameraSpeed_ = 3;

	Vector3 PlayerMoveMent;

	Vector3 CameraTarget;
	float TargetCameraDelay = 0.05f;
	float TargetCameraSpeed_ = 1.0;

	Vector3 higth = { 0,10,0 };

	float Fov = 45.0f;

	float CameraDistanceMinus = 0.0f;

	float CameraMouseMoved = 0.0f;

	float OldMouseMoved = 0.0f;

	Vector2 MovementMous;

	bool cameraDown = false;
	bool cameraUp = false;

	ViewProjection* viewProjection_;

};