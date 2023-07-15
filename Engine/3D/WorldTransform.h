#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>
#include "Quaternion.h"

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ���[�J�� �� ���[���h�ϊ��s��
	float alpha=1;       // �A���t�@
};

struct WorldTarnsLook {

	//���̕��̂̌����Ă������
	Vector3 look = { 0,0,0 };
	Vector3 lookBack = { 0,0,0 };
	Vector3 lookRight = { 0,0,0 };
	Vector3 lookLeft = { 0,0,0 };

	Vector3 lookUp = { 0,0,0 };
	Vector3 lookDown = { 0,0,0 };

	Vector3 look_lookRight = { 0,0,0 };
	Vector3 look_lookLeft = { 0,0,0 };
	Vector3 lookBack_lookRight = { 0,0,0 };
	Vector3 lookBack_lookLeft = { 0,0,0 };

	Vector3 lookUp_look = { 0,0,0 };
	Vector3 lookDown_look = { 0,0,0 };
};

/// <summary>
/// ���[���h�ϊ��f�[�^
/// </summary>
struct WorldTransform {
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ���[�J���X�P�[��
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z�����̃��[�J����]�p
	Vector3 rotation_ = { 0, 0, 0 };
	// ���[�J�����W
	Vector3 translation_ = { 0, 0, 0 };
	//�C�ӎ��̃��[�J����]
	Quaternion quaternion = { 0,0,0,0 };
	// ���[�J�� �� ���[���h�ϊ��s��
	Matrix4 matWorld_;
	//�A���t�@�l
	float alpha = 1;
	//���̕��̂̌����Ă������
	WorldTarnsLook LookVelocity;

	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	const WorldTransform* parent_ = nullptr;

	Matrix4 matRot;

	//��]���I�C���[�p�ɂ���]��
	bool isEuler = false;

	bool IsLookOnlyMatRot = false;

	//������
	void Initialize();
	//�萔�o�b�t�@����
	void CreateConstBuffer();
	//�}�b�s���O����
	void Map();
	//�s���]������
	void TransferMatrix();

	void SetRot(const Vector3& rot);

	void SetMatRot(const Matrix4& mat);

	void MoveRot(const Vector3& move);

	void SetQuater(const Quaternion& quater);

	void MoveQuater(const Quaternion& move);
	
	Quaternion& GetQuaternion();
	
	Vector3 GetLook(Matrix4 matRot,Vector3 at);

	void SetLookMatRot(const Matrix4& mat);

private:
	Matrix4 worldLookMatRot;
};