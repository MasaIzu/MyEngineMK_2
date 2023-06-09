#include "Sprite.h"
#include "TextureManager.h"
#include <cassert>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include "MyMath.h"
#include "WinApp.h"
#include "DirectXCore.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Sprite::Device_ = nullptr;
UINT Sprite::DescriptorHandleSize_;
ID3D12GraphicsCommandList* Sprite::CommandList_ = nullptr;
ComPtr<ID3D12RootSignature> Sprite::RootSignature_;
std::array<ComPtr<ID3D12PipelineState>, 6> Sprite::PipelineStates_;
Matrix4 Sprite::Matrix4Projection_;

void Sprite::StaticInitialize(
	ID3D12Device* device,const std::wstring& directoryPath) {
	// nullptrチェック
	assert(device);

	Device_ = device;

	// デスクリプタサイズを取得
	DescriptorHandleSize_ =
		Device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	std::wstring vsFile = directoryPath + L"/shaders/SpriteVS.hlsl";
	result = D3DCompileFromFile(
		vsFile.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			(char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	std::wstring psFile = directoryPath + L"/shaders/SpritePS.hlsl";
	result = D3DCompileFromFile(
		psFile.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			(char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());

		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	  {// xy座標(1行で書いたほうが見やすい)
	   "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {// uv座標(1行で書いたほうが見やすい)
	   "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;                            // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc =
		CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR); // s0 レジスタ
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	// ルートシグネチャの生成
	result = Device_->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = RootSignature_.Get();

	// レンダーターゲットのブレンド設定。ブレンドなし
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = false;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// グラフィックスパイプラインの生成
	result = Device_->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&PipelineStates_[0]));
	assert(SUCCEEDED(result));

	// 通常αブレンド
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	result = Device_->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&PipelineStates_[1]));
	assert(SUCCEEDED(result));

	// 加算
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	result = Device_->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&PipelineStates_[2]));
	assert(SUCCEEDED(result));

	// 減算
	blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	result = Device_->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&PipelineStates_[3]));
	assert(SUCCEEDED(result));

	// 乗算
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ZERO;
	blenddesc.DestBlend = D3D12_BLEND_SRC_COLOR;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	result = Device_->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&PipelineStates_[4]));
	assert(SUCCEEDED(result));

	// スクリーン
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	result = Device_->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&PipelineStates_[5]));
	assert(SUCCEEDED(result));

	// 射影行列計算
	Matrix4Projection_ =
		MyMath::Matrix4Orthographic(
			0.0f, WinApp::GetInstance()->GetWindowSize().x, WinApp::GetInstance()->GetWindowSize().y, 0.0f, 0.0f, 1.0f);
}

void Sprite::StaticFinalize()
{

	RootSignature_.Reset();

	for (int i = 0; i < PipelineStates_.size(); i++) {
		PipelineStates_[i].Reset();
	}

}

void Sprite::PreDraw(ID3D12GraphicsCommandList* commandList, int blendMode) {

	// コマンドリストをセット
	CommandList_ = commandList;

	// パイプラインステートの設定
	CommandList_->SetPipelineState(PipelineStates_[blendMode].Get());
	// ルートシグネチャの設定
	CommandList_->SetGraphicsRootSignature(RootSignature_.Get());
	// プリミティブ形状を設定
	CommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw() {
	// コマンドリストを解除
	Sprite::CommandList_ = nullptr;
}

std::unique_ptr<Sprite> Sprite::Create(uint32_t textureHandle) {
	// 仮サイズ
	Vector2 size = { 100.0f, 100.0f };

	{
		// テクスチャ情報取得
		const D3D12_RESOURCE_DESC& resDesc =
			TextureManager::GetInstance()->GetResoureDesc(textureHandle);
		// スプライトのサイズをテクスチャのサイズに設定
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// Spriteのインスタンスを生成
	Sprite* sprite =
		new Sprite(textureHandle,size);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return std::unique_ptr<Sprite>(sprite);
}

Sprite::Sprite() {}

Sprite::Sprite(
	uint32_t textureHandle,Vector2 size) {
	size_ = size;
	anchorPoint_ = Vector2(0.5f,0.5f);
	matWorld_ = MyMath::MakeIdentity();
	textureHandle_ = textureHandle;
	texSize_ = size;
}

bool Sprite::Initialize() {
	// nullptrチェック
	assert(Device_);

	HRESULT result = S_FALSE;

	resourceDesc_ = TextureManager::GetInstance()->GetResoureDesc(textureHandle_);

	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * kVertNum);

		// 頂点バッファ生成
		result = Device_->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&vertBuff_));
		assert(SUCCEEDED(result));

		// 頂点バッファマッピング
		result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
		assert(SUCCEEDED(result));
	}

	// 頂点バッファへのデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUv);

	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

		// 定数バッファの生成
		result = Device_->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuff_));
		assert(SUCCEEDED(result));
	}

	// 定数バッファマッピング
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	return true;
}

void Sprite::SetTextureHandle(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
	resourceDesc_ = TextureManager::GetInstance()->GetResoureDesc(textureHandle_);
}

void Sprite::SetRotation(float rotation) {
	rotation_ = rotation;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetPosition(const Vector2& position) {
	position_ = position;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetSize(const Vector2& size) {
	size_ = size;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetAnchorPoint(const Vector2& anchorpoint) {
	anchorPoint_ = anchorpoint;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipX(bool isFlipX) {
	isFlipX_ = isFlipX;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipY(bool isFlipY) {
	isFlipY_ = isFlipY;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetTextureRect(const Vector2& texBase, const Vector2& texSize) {
	texBase_ = texBase;
	texSize_ = texSize;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetDrawUpdate(Vector2 Position, Vector4 Color){

	color_ = Color;

	SetPosition(Position);
}

void Sprite::Draw(Vector2 Position, Vector4 Color, int blendMode) {

	SetDrawUpdate(Position, Color);

	// ワールド行列の更新
	matWorld_ = MyMath::MakeIdentity();
	matWorld_ *= MyMath::Rotation(Vector3(0,0,rotation_),3);
	matWorld_ *= MyMath::Translation(Vector3(position_.x, position_.y, 0.0f));

	// 定数バッファにデータ転送
	constMap_->color = color_;
	constMap_->mat = matWorld_ * Matrix4Projection_; // 行列の合成


	// コマンドリストをセット
	CommandList_ = DirectXCore::GetInstance()->GetCommandList();

	// パイプラインステートの設定
	CommandList_->SetPipelineState(PipelineStates_[blendMode].Get());
	// ルートシグネチャの設定
	CommandList_->SetGraphicsRootSignature(RootSignature_.Get());
	// プリミティブ形状を設定
	CommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファの設定
	CommandList_->IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファビューをセット
	CommandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(CommandList_, 1, textureHandle_);
	// 描画コマンド
	CommandList_->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices() {
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;
	if (isFlipX_) { // 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_) { // 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VertexPosUv vertices[kVertNum];

	vertices[LB].pos = { left, bottom, 0.0f };  // 左下
	vertices[LT].pos = { left, top, 0.0f };     // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top, 0.0f };    // 右上

	// テクスチャ情報取得
	{
		float tex_left = texBase_.x / resourceDesc_.Width;
		float tex_right = (texBase_.x + texSize_.x) / resourceDesc_.Width;
		float tex_top = texBase_.y / resourceDesc_.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resourceDesc_.Height;

		vertices[LB].uv = { tex_left, tex_bottom };  // 左下
		vertices[LT].uv = { tex_left, tex_top };     // 左上
		vertices[RB].uv = { tex_right, tex_bottom }; // 右下
		vertices[RT].uv = { tex_right, tex_top };    // 右上
	}

	// 頂点バッファへのデータ転送
	memcpy(vertMap_, vertices, sizeof(vertices));
}
