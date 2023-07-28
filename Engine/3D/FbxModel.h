#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <Mesh.h>
#include "Material.h"
#include <Light.h>
#include <WorldTransform.h>
#include <ViewProjection.h>


class FbxModel
{
public:
	// �t�����h�N���X
	friend class FbxLoader;

public://�萔
	static const int MAX_BONE_INDICES = 4;


public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUvFbxSkin
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W

		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};


private:
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Vector4 color;	// �F (RGBA)
		Matrix4 mat;	// �R�c�ϊ��s��
	};

	//1���b�V���Ɏ��Ă�{�[���̍ő��
	static const int MAX_BONES = 128;

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataSkin {
		Matrix4 bones[MAX_BONES];
	};

	//�A�j���[�V�������Ȃ��ꍇ
	struct ConstBufferDataInitialMatrix {
		Matrix4 InitialMatrix;
	};


	//�萔�o�b�t�@(�X�L��)
	static Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin_;
	//�萔�o�b�t�@(�A�j���[�V�����Ȃ�)
	static Microsoft::WRL::ComPtr<ID3D12Resource> constBuffNothing_;

private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultModelName;

private: // �ÓI�����o�ϐ�
	// �f�X�N���v�^�T�C�Y
	static UINT sDescriptorHandleIncrementSize_;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* sCommandList_;
	// ���[�g�V�O�l�`��
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;
	// ���C�g
	static std::unique_ptr<LightGroup> lightGroup;


public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize();

	// �ÓI�I������
	static void StaticFainalize();

	// �O���t�B�b�N�X�p�C�v���C���̏�����
	static void InitializeGraphicsPipeline();

	// 3D���f������
	static FbxModel* Create();

	// OBJ�t�@�C�����烁�b�V������
	static FbxModel* CreateFromFbx(const std::string& modelname, bool smoothing = false);

	// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	// �`��㏈��
	static void PostDraw();

public: // �����o�֐�
	// �f�X�g���N�^
	~FbxModel();

	// ������
	void Initialize();

	void FbxUpdate(float frem);


	// �`��
	//void Draw(
	//	const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHadle);


	//void ModelAnimation(float frame, aiAnimation* Animation);

	//void ReadNodeHeirarchy(Mesh* mesh, aiAnimation* Animation, FLOAT AnimationTime, Node* pNode, Matrix4& mxIdentity);

	//aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& strNodeName);

	//void CalcInterpolatedScaling(Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim);

	//bool FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nScalingIndex);

	//void CalcInterpolatedRotation(Vector4& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim);

	//bool FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nRotationIndex);

	//void CalcInterpolatedPosition(Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim);

	//bool FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nPosIndex);


	// ���b�V���R���e�i���擾
	inline const std::vector<Mesh*>& GetMeshes() { return meshes_; }

	void SetTextureHandle(uint32_t textureHandle) { modelTextureHandle = textureHandle; }

private:

	Matrix4 Test;


	// �m�[�h�z��
	std::vector<Node> nodes;


	Matrix4 globalInverseTransform;

	// ���O
	std::string name_;
	// ���b�V���R���e�i
	std::vector<Mesh*> meshes_;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials_;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial_ = nullptr;

	uint32_t modelTextureHandle = 0;

};

