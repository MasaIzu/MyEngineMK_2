#include "Light.h"
#include <assert.h>


LightGroup* LightGroup::Create() {
	// 3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	// 初期化
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize() {

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	// 定数バッファとのデータリンク
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	// 定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::Update() {



}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList_, UINT rootParameterIndex) {
	// 定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer() {
	// 環境光
	constMap_->ambientColor = ambientColor_;
	// 平行光源

	constMap_->dirLights[0].lightv = -Vector4( 0.0f, -1.0f, 0.0f, 0 );
	constMap_->dirLights[0].lightcolor = Vector3(1.0f, 1.0f, 1.0f);

	constMap_->dirLights[1].lightv = -Vector4(+0.5f, +0.1f, +0.2f, 0);
	constMap_->dirLights[1].lightcolor = Vector3(1.0f, 1.0f, 1.0f);

	constMap_->dirLights[2].lightv = -Vector4(-0.5f, +0.1f, -0.2f, 0);
	constMap_->dirLights[2].lightcolor = Vector3(1.0f, 1.0f, 1.0f);


}

