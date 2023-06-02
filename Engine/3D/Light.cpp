#include "Light.h"
#include <assert.h>


LightGroup* LightGroup::Create() {
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LightGroup* instance = new LightGroup();

	// ������
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize() {

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�Ƃ̃f�[�^�����N
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void LightGroup::Update() {



}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList_, UINT rootParameterIndex) {
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList_->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer() {
	// ����
	constMap_->ambientColor = ambientColor_;
	constMap_->diffuseColor = diffuseColor_;
	constMap_->specularColor = specularColor_;
	constMap_->rimrightColor = rimrightColor_;
	// ���s����

	constMap_->dirLights[0].lightv = -Vector4( 0.0f, -1.0f, 0.0f, 0 );
	constMap_->dirLights[0].lightcolor = Vector3(1.0f, 1.0f, 1.0f);

	constMap_->dirLights[1].lightv = -Vector4(+0.5f, +0.1f, +0.2f, 0);
	constMap_->dirLights[1].lightcolor = Vector3(1.0f, 1.0f, 1.0f);

	constMap_->dirLights[2].lightv = -Vector4(-0.5f, +0.1f, -0.2f, 0);
	constMap_->dirLights[2].lightcolor = Vector3(1.0f, 1.0f, 1.0f);


}

void LightGroup::SetLightColor(Vector3 ambient, Vector3 diffuse, Vector3 specular,Vector3 rimrightColor)
{
	ambientColor_ = ambient;
	diffuseColor_ = diffuse;
	specularColor_ = specular;
	rimrightColor_ = rimrightColor;

	TransferConstBuffer();
}

void LightGroup::SetLighPower(float minThreshold_, float maxThreshold_, float rimLightPow_)
{
	minThreshold = minThreshold_;
	maxThreshold = maxThreshold_;
	rimLightPow = rimLightPow_;

	constMap_->minThreshold = minThreshold;
	constMap_->maxThreshold = maxThreshold;
	constMap_->rimLightPow = rimLightPow;
}

