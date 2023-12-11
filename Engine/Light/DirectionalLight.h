#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

class DirectionalLight {
private: //�ÓI�����o�ϐ�
//�f�o�C�X
	static ID3D12Device* device;

private://�G�C���A�X
		//Microsoft::WRL::���ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirecX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X�@
	//�萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferData
	{
		XMVECTOR lightv; //���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor; //���C�g�̐F
		unsigned int active;
	};

	HRESULT result;

public://�ÓI���C�o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	void SetLightDir(const XMVECTOR& lightdir);

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	void SetLightColor(const XMFLOAT3& lightcolor);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(bool active) { this->active = active; }

	
	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
	inline bool IsActive() { return active; }

	XMVECTOR GetLightDir() { return lightdir; }
	XMFLOAT3 GetLightColor() { return lightcolor; }

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static DirectionalLight* Create();
private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���C�g��������
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1, };
	//�_�[�e�B�t���O
	bool dirty = false;

	//�L���t���O
	bool active = false;
};

