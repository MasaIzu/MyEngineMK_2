#pragma once

#include "Vector3.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <wrl.h>

/// <summary>
/// �}�e���A��
/// </summary>
class Material {
public: // �T�u�N���X
  // �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Vector3 ambient;  // �A���r�G���g�W��
		float pad1;        // �p�f�B���O
		Vector3 diffuse;  // �f�B�t���[�Y�W��
		float pad2;        // �p�f�B���O
		Vector3 specular; // �X�y�L�����[�W��
		float alpha;       // �A���t�@
	};

public: // �ÓI�����o�֐�
  /// <summary>
  /// �}�e���A������
  /// </summary>
  /// <returns>�������ꂽ�}�e���A��</returns>
	static Material* Create();

public:
	std::string name_;            // �}�e���A����
	Vector3 ambient_;            // �A���r�G���g�e���x
	Vector3 diffuse_;            // �f�B�t���[�Y�e���x
	Vector3 specular_;           // �X�y�L�����[�e���x
	float alpha_;                 // �A���t�@
	std::string textureFilename_; // �e�N�X�`���t�@�C����

public:
	/// <summary>
	/// �萔�o�b�t�@�̎擾
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }

	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�ǂݍ��݃f�B���N�g���p�X</param>
	void LoadTexture(const std::string& directoryPath);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �O���t�B�b�N�X�R�}���h�̃Z�b�g
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	/// <param name="rooParameterIndexMaterial">�}�e���A���̃��[�g�p�����[�^�ԍ�</param>
	/// <param name="rooParameterIndexTexture">�e�N�X�`���̃��[�g�p�����[�^�ԍ�</param>
	void SetGraphicsCommand(
		ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
		UINT rooParameterIndexTexture);

	/// <summary>
	/// �O���t�B�b�N�X�R�}���h�̃Z�b�g�i�e�N�X�`�������ւ��Łj
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	/// <param name="rooParameterIndexMaterial">�}�e���A���̃��[�g�p�����[�^�ԍ�</param>
	/// <param name="rooParameterIndexTexture">�e�N�X�`���̃��[�g�p�����[�^�ԍ�</param>
	/// <param name="textureHandle">�����ւ���e�N�X�`���n���h��</param>
	void SetGraphicsCommand(
		ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
		UINT rooParameterIndexTexture, uint32_t textureHandle);

	// �Z�b�g�e�N�X�`���n���h��
	void SetTextureHadle(uint32_t& textureHandles) { textureHandle_ = textureHandles; }

	// �e�N�X�`���n���h��
	uint32_t GetTextureHadle() { return textureHandle_; }

	void SetLight(Vector3 ambient, Vector3 diffuse, Vector3 specular, float alpha);

private:
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// �萔�o�b�t�@�̃}�b�v
	ConstBufferData* constMap_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

private:
	// �R���X�g���N�^
	Material() {
		ambient_ = { 0.3f, 0.3f, 0.3f };
		diffuse_ = { 0.0f, 0.0f, 0.0f };
		specular_ = { 0.0f, 0.0f, 0.0f };
		alpha_ = 1.0f;
	}

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�̐���
	/// </summary>
	void CreateConstantBuffer();
};
