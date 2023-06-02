#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include"DirectXCore.h"
#include "Vector3.h"
#include "Vector4.h"


/// <summary>
/// ���C�g
/// </summary>
class LightGroup
{
private: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public: // �萔
	// ���s�����̐�
	static const int kDirLightNum = 3;
	// �_�����̐�
	static const int kPointLightNum = 3;
	// �X�|�b�g���C�g�̐�
	static const int kSpotLightNum = 3;
	// �ۉe�̐�
	static const int kCircleShadowNum = 1;

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferLightData
	{
		Vector4 lightv;
		Vector3 lightcolor;
		unsigned int active;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		// �����̐F
		Vector3 ambientColor;
		// �����̐F
		Vector3 diffuseColor;
		// �����̐F
		Vector3 specularColor;
		// �����̐F
		Vector3 rimrightColor;

		float minThreshold = 0;
		float maxThreshold = 1;


		float rimLightPow = 10.0f;

		// ���s�����p
		ConstBufferLightData dirLights[kDirLightNum];
		
	};


public: // �ÓI�����o�֐�
	// �C���X�^���X����
	static LightGroup* Create();

public: // �����o�֐�
	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	// �萔�o�b�t�@�]��
	void TransferConstBuffer();

	void SetLightColor(Vector3 ambient,Vector3 diffuse,Vector3 specular, Vector3 rimrightColor);
	void SetLighPower(float minThreshold_, float maxThreshold_, float rimLightPow_);

private: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	// �萔�o�b�t�@�̃}�b�v
	ConstBufferData* constMap_ = nullptr;

	// �����̐F
	Vector3 ambientColor_ = { 1,1,1 };
	Vector3 diffuseColor_ = { 1,1,1 };
	Vector3 specularColor_ = { 1,1,1 };
	Vector3 rimrightColor_ = { 1,1,1 };

	float minThreshold = 0;
	float maxThreshold = 1;

	float rimLightPow = 10.0f;

	// ���C�g�����i�P�ʃx�N�g���j
	Vector4 lightdir = { 1,0,0,0 };
	// ���C�g�F
	Vector3 lightcolor = { 1,1,1 };

	// �_�[�e�B�t���O
	bool dirty_ = false;
};

