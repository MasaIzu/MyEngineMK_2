#include "DebugCamera.h"
#include "MyMath.h"
#include "WinApp.h"

void DebugCamera::Initialize(ViewProjection* viewProjection)
{

	this->viewProjection = viewProjection;

	input_ = Input::GetInstance();

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / static_cast<float>(WinApp::window_width);
	scaleY = 1.0f / static_cast<float>(WinApp::window_height);
}

void DebugCamera::Update()
{
	isAction = false;
	angleX = 0;
	angleY = 0;
	mouseMove = Vector3(0, 0, 0);

	// �}�E�X�̓��͂��擾
	mouseMove = input_->GetMouseWheelMove();

	MouseUpdate();
	MouseRot();

	viewProjection->UpdateMatrix();
}

void DebugCamera::MouseRot()
{
	if (input_->MouseInputing(1)) {
		// �}�E�X�̉E�{�^����������Ă�����J��������]������

		float dy = mouseMove.x * scaleY;
		float dx = mouseMove.y * scaleX;

		angleX = -dx * MyMath::PI;
		angleY = -dy * MyMath::PI;
		isAction = true;

	}

	if (isAction == true) {
		// �ǉ���]���̉�]�s��𐶐�
		Matrix4 matRotNew = MyMath::Initialize();
		matRotNew *= MyMath::Rotation(Vector3(-angleX, 0, 0), 1);
		matRotNew *= MyMath::Rotation(Vector3(0, -angleY, 0), 2);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		CameraRot = matRotNew * CameraRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		Vector3 vTargetEye = { 0.0f, 0.0f, -CameraDistance };
		Vector3 vUp = { 0.0f, 1.0f, 0.0f };

		// �x�N�g������]
		vTargetEye = MyMath::MatVector(CameraRot, vTargetEye);
		vUp = MyMath::MatVector(CameraRot, vUp);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		const Vector3& target = viewProjection->target;
		viewProjection->eye = Vector3(target.x + vTargetEye.x, target.y + vTargetEye.y, target.z + vTargetEye.z);
		viewProjection->up = Vector3(vUp.x, vUp.y, vUp.z);
	}

}

void DebugCamera::MouseUpdate()
{

	// �z�C�[�����͂ŋ�����ύX
	if (mouseMove.z != 0) {
		CameraDistance -= mouseMove.z / 50.0f;
		CameraDistance = max(CameraDistance, 1.0f);
		isAction = true;
	}

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�������
	if (input_->MouseInputing(2)) {
		float dx = mouseMove.x / 100.0f;
		float dy = mouseMove.y / 100.0f;

		Vector3 move = { -dx, +dy, 0 };
		move = MyMath::MatVector(CameraRot, move);

		MoveVector(move);
		isAction = true;
	}

}

void DebugCamera::MoveVector(const Vector3& move)
{
	// ���_�ƒ����_���W���ړ�
	viewProjection->eye.x += move.x;
	viewProjection->eye.y += move.y;
	viewProjection->eye.z += move.z;

	viewProjection->target.x += move.x;
	viewProjection->target.y += move.y;
	viewProjection->target.z += move.z;

}
