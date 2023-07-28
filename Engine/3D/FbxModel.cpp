#include "DirectXCore.h"
#include "FbxModel.h"
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

#include<atldef.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const std::string FbxModel::kBaseDirectory = "Resources/";
const std::string FbxModel::kDefaultModelName = "cube";
UINT FbxModel::sDescriptorHandleIncrementSize_ = 0;
ID3D12GraphicsCommandList* FbxModel::sCommandList_ = nullptr;
ComPtr<ID3D12RootSignature> FbxModel::sRootSignature_;
ComPtr<ID3D12PipelineState> FbxModel::sPipelineState_;
std::unique_ptr<LightGroup> FbxModel::lightGroup;

Microsoft::WRL::ComPtr<ID3D12Resource> FbxModel::constBuffSkin_;
Microsoft::WRL::ComPtr<ID3D12Resource> FbxModel::constBuffNothing_;

void FbxModel::StaticInitialize() {

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

	// ���C�g����
	lightGroup.reset(LightGroup::Create());
}

void FbxModel::StaticFainalize()
{

	sRootSignature_.Reset();
	sPipelineState_.Reset();

	constBuffSkin_.Reset();
	constBuffNothing_.Reset();
}

void FbxModel::InitializeGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g


	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/FBXVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			(char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/FBXPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			(char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	  {// xy���W(1�s�ŏ������ق������₷��)
	   "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {// �@���x�N�g��(1�s�ŏ������ق������₷��)
	   "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {// uv���W(1�s�ŏ������ق������₷��)
	   "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

	  {//�e�����󂯂�{�[���ԍ�
		"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	  },
	  {//�{�[���̃X�L���E�F�C�g(4��)
		"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	  },

	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	//gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;


	// gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//  �f�v�X�X�e���V���X�e�[�g
	//gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//  �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;                       // �`��Ώۂ�2��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[6];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[5].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v��
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = DirectXCore::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootSignature_.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = DirectXCore::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&sPipelineState_));
	assert(SUCCEEDED(result));


	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDescNothing = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataInitialMatrix) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffSkin_));

	// �萔�o�b�t�@�̐���
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDescNothing, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffNothing_));


	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++) {
		constMapSkin->bones[i] = MyMath::MakeIdentity();
	}
	constBuffSkin_->Unmap(0, nullptr);

}

FbxModel* FbxModel::Create() {
	// �������m��
	FbxModel* instance = new FbxModel;
	instance->Initialize();

	return instance;
}

FbxModel* FbxModel::CreateFromFbx(const std::string& modelname, bool smoothing) {
	// �������m��
	FbxModel* instance = new FbxModel;
	instance->Initialize();

	return instance;
}

void FbxModel::PreDraw(ID3D12GraphicsCommandList* commandList) {
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(FbxModel::sCommandList_ == nullptr);

	// �R�}���h���X�g���Z�b�g
	sCommandList_ = commandList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	commandList->SetPipelineState(sPipelineState_.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	commandList->SetGraphicsRootSignature(sRootSignature_.Get());
	// �v���~�e�B�u�`���ݒ�
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void FbxModel::PostDraw() {
	// �R�}���h���X�g������
	sCommandList_ = nullptr;
}

FbxModel::~FbxModel() {
	for (auto m : meshes_) {
		delete m;
	}
	meshes_.clear();

	for (auto m : materials_) {
		delete m.second;
	}
	materials_.clear();

	delete defaultMaterial_;
}

void FbxModel::Initialize() {
	// ���f���ǂݍ���
	//LoadModelFromFile(modelname);

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes_) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial_ == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial_ = Material::Create();
				defaultMaterial_->name_ = "no material";
				materials_.emplace(defaultMaterial_->name_, defaultMaterial_);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial_);
		}
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes_) {

		Vector3 ambient_ = { 1.0f, 1.0f, 1.0f };
		Vector3 diffuse_ = { 0.0f, 0.0f, 0.0f };
		Vector3 specular_ = { 0.0f, 0.0f, 0.0f };
		float alpha_ = 1.0f;

		m->SetLight(ambient_, diffuse_, specular_, alpha_);

		m->CreateBuffers();


	}

	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials_) {

		m.second->Update();
	}

	for (auto& m : meshes_) {
		for (auto& b : m->vecBones) {

			m->bones[b.name] = &b;

		}

	}



}

void FbxModel::FbxUpdate(float frem)
{
	HRESULT result = S_FALSE;

	//std::unordered_map<std::string, Mesh::Bone*> bones = model->GetBones();

	////�萔�o�b�t�@�ւ̃f�[�^�]��
	//ConstBufferDataSkin* constMapSkin = nullptr;
	//result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	//for (int i = 0; i < bones.size(); i++) {
	//	//���̎p���s��
	//	XMMATRIX matCurrentPose;
	//	//���̎p���s����擾
	//	FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
	//	//XMMATRIX�ɕϊ�
	//	FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
	//	//�������ăX�L�j���O�s���
	//	constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	//}
	//constBuffSkin->Unmap(0, nullptr);

}


void FbxModel::Draw(
	const WorldTransform& worldTransform, const ViewProjection& viewProjection) {

	for (int i = 0; i < meshes_.size(); i++) {

		// ���C�g�̕`��
		lightGroup->Draw(sCommandList_, 4);

		// CBV���Z�b�g�i���[���h�s��j
		sCommandList_->SetGraphicsRootConstantBufferView(0, worldTransform.constBuff_->GetGPUVirtualAddress());

		// CBV���Z�b�g�i�r���[�v���W�F�N�V�����s��j
		sCommandList_->SetGraphicsRootConstantBufferView(1, viewProjection.constBuff_->GetGPUVirtualAddress());

		if (meshes_[i]->node) {

			HRESULT result = S_FALSE;

			//�萔�o�b�t�@�փf�[�^�]��
			ConstBufferDataInitialMatrix* constMapSkin = nullptr;
			result = constBuffNothing_->Map(0, nullptr, (void**)&constMapSkin);
			constMapSkin->InitialMatrix = meshes_[i]->node->globalTransform;

			constBuffNothing_->Unmap(0, nullptr);


			sCommandList_->SetGraphicsRootConstantBufferView(5, constBuffNothing_->GetGPUVirtualAddress());

		}

		// CBV���Z�b�g�i�{�[���s��j
		sCommandList_->SetGraphicsRootConstantBufferView(4, constBuffSkin_->GetGPUVirtualAddress());

		// �S���b�V����`��
		meshes_[i]->Draw(sCommandList_, 2, 3, modelTextureHandle);
	}
}


void FbxModel::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHadle) {

	// �S���b�V����`��
	for (auto& mesh : meshes_) {

		// ���C�g�̕`��
		lightGroup->Draw(sCommandList_, 4);

		// CBV���Z�b�g�i���[���h�s��j
		sCommandList_->SetGraphicsRootConstantBufferView(0, worldTransform.constBuff_->GetGPUVirtualAddress());

		// CBV���Z�b�g�i�r���[�v���W�F�N�V�����s��j
		sCommandList_->SetGraphicsRootConstantBufferView(1, viewProjection.constBuff_->GetGPUVirtualAddress());

		if (mesh->node) {

			HRESULT result = S_FALSE;

			//�萔�o�b�t�@�փf�[�^�]��
			ConstBufferDataInitialMatrix* constMapSkin = nullptr;
			result = constBuffNothing_->Map(0, nullptr, (void**)&constMapSkin);
			constMapSkin->InitialMatrix = mesh->node->globalTransform;

			constBuffNothing_->Unmap(0, nullptr);


			sCommandList_->SetGraphicsRootConstantBufferView(5, constBuffNothing_->GetGPUVirtualAddress());

		}

		// CBV���Z�b�g�i�{�[���s��j
		sCommandList_->SetGraphicsRootConstantBufferView(4, constBuffSkin_->GetGPUVirtualAddress());


		mesh->Draw(sCommandList_, 2, 3, textureHadle);
	}
}


void FbxModel::ModelAnimation(float frame, aiAnimation* Animation) {

	HRESULT result = S_FALSE;

	Matrix4 mxIdentity = MyMath::MakeIdentity();
	Node* pNode = &nodes[0];


	FLOAT TicksPerSecond = (FLOAT)(Animation->mTicksPerSecond != 0 ? Animation->mTicksPerSecond : 25.0f);

	FLOAT TimeInTicks = frame * TicksPerSecond;
	FLOAT AnimationTime = fmod(TimeInTicks, (FLOAT)Animation->mDuration);

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin);

	for (Mesh* mesh : meshes_)
	{
		ReadNodeHeirarchy(mesh, Animation, AnimationTime, pNode, mxIdentity);

		UINT nNumBones = (UINT)mesh->bones.size();

		for (UINT i = 0; i < nNumBones; i++)
		{
			mesh->vecBones[i].matrix = mesh->bones[mesh->vecBones[i].name]->matrix;

			constMapSkin->bones[i] = mesh->vecBones[i].matrix;
		}
	}


	constBuffSkin_->Unmap(0, nullptr);
}


void FbxModel::ReadNodeHeirarchy(Mesh* mesh, aiAnimation* pAnimation, FLOAT AnimationTime, Node* pNode, Matrix4& mxParentTransform) {

	Matrix4 mxNodeTransformation = MyMath::MakeIdentity();
	mxNodeTransformation = pNode->transform;

	Matrix4 mxThisTrans = mxNodeTransformation;

	std::string strNodeName(pNode->name);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, strNodeName);

	if (pNodeAnim)
	{
		//�X�P�[�����O
		Vector3 vScaling = {};
		CalcInterpolatedScaling(vScaling, AnimationTime, pNodeAnim);
		Matrix4 mxScaling;
		mxScaling = MyMath::Scale(vScaling);

		//��]�p
		Vector4 vRotationQ = {};
		CalcInterpolatedRotation(vRotationQ, AnimationTime, pNodeAnim);
		Matrix4 mxRotationM = Quaternion(vRotationQ).Rotate();

		//�ړ�
		Vector3 vTranslation = {};
		CalcInterpolatedPosition(vTranslation, AnimationTime, pNodeAnim);
		Matrix4 mxTranslationM;
		mxTranslationM = MyMath::Translation(vTranslation);

		Matrix4 affin = MyMath::Initialize();
		/*affin *= mxScaling;
		affin *= mxRotationM;
		affin *= mxTranslationM;*/

		affin = mxScaling.MatMul(mxRotationM).MatMul(mxTranslationM);

		mxNodeTransformation = affin;
	}

	Matrix4 mxGlobalTransformation = mxNodeTransformation.MatMul(mxParentTransform);

	Matrix4 offsetMatirx;
	Matrix4 matirx;

	if (mesh->bones.find(strNodeName) != mesh->bones.end())
	{
		offsetMatirx = mesh->bones[strNodeName]->offsetMatirx;

		matirx = offsetMatirx.MatMul(mxGlobalTransformation).MatMul(globalInverseTransform);

		mesh->bones[strNodeName]->matrix = matirx;

	}

	for (UINT i = 0; i < pNode->childrens.size(); i++)
	{
		ReadNodeHeirarchy(mesh
			, pAnimation
			, AnimationTime
			, pNode->childrens[i]
			, mxGlobalTransformation);
	}



}


aiNodeAnim* FbxModel::FindNodeAnim(const aiAnimation* pAnimation, const std::string& strNodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++)
	{
		if (std::string(pAnimation->mChannels[i]->mNodeName.data) == strNodeName)
		{
			return pAnimation->mChannels[i];
		}
	}

	return nullptr;
}

void FbxModel::CalcInterpolatedScaling(Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		mxOut = MyMath::AssimpVector3(pNodeAnim->mScalingKeys[0].mValue);
		return;
	}

	UINT ScalingIndex = 0;
	if (!FindScaling(AnimationTime, pNodeAnim, ScalingIndex))
	{
		mxOut = Vector3(1.0f, 1.0f, 1.0f);
		return;
	}

	UINT NextScalingIndex = (ScalingIndex + 1);
	ATLASSERT(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = Vector3::lerp(MyMath::AssimpVector3(pNodeAnim->mScalingKeys[ScalingIndex].mValue), MyMath::AssimpVector3(pNodeAnim->mScalingKeys[NextScalingIndex].mValue), Factor);

}

void FbxModel::CalcInterpolatedRotation(Vector4& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		mxOut = MyMath::AssimpQuaternionVec4(pNodeAnim->mRotationKeys[0].mValue);
		return;
	}

	UINT RotationIndex = 0;
	if (!FindRotation(AnimationTime, pNodeAnim, RotationIndex))
	{
		mxOut = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	UINT NextRotationIndex = (RotationIndex + 1);
	ATLASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime
		- pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = MyMath::QuaternionSlerp(
		pNodeAnim->mRotationKeys[RotationIndex].mValue
		, pNodeAnim->mRotationKeys[NextRotationIndex].mValue
		, Factor);

}

void FbxModel::CalcInterpolatedPosition(Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		mxOut = MyMath::AssimpVector3(pNodeAnim->mPositionKeys[0].mValue);
		return;
	}

	UINT PositionIndex = 0;
	if (!FindPosition(AnimationTime, pNodeAnim, PositionIndex))
	{
		mxOut = Vector3(0.0f, 0.0f, 0.0f);
		return;
	}

	UINT NextPositionIndex = (PositionIndex + 1);

	ATLASSERT(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = Vector3::lerp(MyMath::AssimpVector3(pNodeAnim->mPositionKeys[PositionIndex].mValue), MyMath::AssimpVector3(pNodeAnim->mPositionKeys[NextPositionIndex].mValue), Factor);
}

bool FbxModel::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nPosIndex)
{
	nPosIndex = 0;
	if (!(pNodeAnim->mNumPositionKeys > 0))
	{
		return FALSE;
	}

	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if ((AnimationTime >= (float)pNodeAnim->mPositionKeys[i].mTime)
			&& (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime))
		{
			nPosIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

bool FbxModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nRotationIndex)
{
	nRotationIndex = 0;
	if (!(pNodeAnim->mNumRotationKeys > 0))
	{
		return FALSE;
	}

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{

		if ((AnimationTime >= (float)pNodeAnim->mRotationKeys[i].mTime)
			&& (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime))
		{
			nRotationIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

bool FbxModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nScalingIndex)
{
	nScalingIndex = 0;
	if (!(pNodeAnim->mNumScalingKeys > 0))
	{
		return FALSE;
	}

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if ((AnimationTime >= (float)pNodeAnim->mScalingKeys[i].mTime)
			&& (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime))
		{
			nScalingIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}