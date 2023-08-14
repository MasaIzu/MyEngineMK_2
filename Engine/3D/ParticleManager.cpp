#include "ParticleManager.h"
#include "DirectXCore.h"
#include "Model.h"
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <d3d12.h>
#include "d3dx12.h"
#include <CreateResource.h>
#include <combaseapi.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* ParticleManager::device = nullptr;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12RootSignature> ParticleManager::rootSignature;//コンピュートシェーダー用
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12PipelineState> ParticleManager::pipelineState;//コンピュートシェーダー用

std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> ParticleManager::m_pipelines;
ComPtr<ID3D12DescriptorHeap> ParticleManager::m_cbvSrvUavHeap;

const std::string ParticleManager::PSO_DEFAULT = "PSO_DEFAULT";
const std::string ParticleManager::PSO_CS_INIT = "PSO_CS_INIT";
const std::string ParticleManager::PSO_CS_EMIT = "PSO_CS_EMIT";
const std::string ParticleManager::PSO_CS_UPDATE = "PSO_CS_UPDATE";
const std::string ParticleManager::PSO_DRAW_PARTICLE = "PSO_DRAW_PARTICLE";
const std::string ParticleManager::PSO_DRAW_PARTICLE_USE_TEX = "PSO_DRAW_PARTICLE_USE_TEX";

float easeOutQuint(float x)
{
	return sin((x * PI) / 2);
}
float easeInQuint(float x)
{
	return x * x * x * x * x;
}

void ParticleManager::StaticInitialize(ID3D12Device* device)
{
	// nullptrチェック
	assert(device);

	ParticleManager::device = device;

	// パイプライン初期化
	InitializeGraphicsPipeline();

}

void ParticleManager::StaticFinalize()
{

}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleManager::cmdList == nullptr);

	// コマンドリストをセット
	ParticleManager::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(m_pipelines[PSO_DEFAULT].Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw()
{
	// コマンドリストを解除
	ParticleManager::cmdList = nullptr;
}

void ParticleManager::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; //ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	//gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度グリッピングを有効に
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う
	//加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない
	////半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング




	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_pipelines[PSO_DEFAULT]));
	assert(SUCCEEDED(result));

	{
		// カウンタ付き UAV はルートパラメータとして設定できない.
		CD3DX12_DESCRIPTOR_RANGE uavIndexList{};
		uavIndexList.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		// ルートシグネチャの作成
		std::array<CD3DX12_ROOT_PARAMETER, 3> rootParameters;
		rootParameters[0].InitAsConstantBufferView(0); // b0: Params
		rootParameters[1].InitAsUnorderedAccessView(0);// u0: Particles
		rootParameters[2].InitAsDescriptorTable(1, &uavIndexList); // u1: ParticleIndexList

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Init(
			UINT(rootParameters.size()), rootParameters.data(),
			1, &samplerDesc);

			ComPtr<ID3DBlob> signature, errBlob;
		D3D12SerializeRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
		device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	}

	ComPtr<ID3DBlob> csBlobInit; //コンピュートシェーダオブジェクト
	ComPtr<ID3DBlob> csBlobEmit;
	ComPtr<ID3DBlob> csBlobUpdate;
	// コンピュートシェーダーのコンパイル
	D3DCompileFromFile(
		L"Resources/Shaders/ShaderGpuParticle.hlsl",
		nullptr, nullptr,
		"initParticle", "cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobInit,
		nullptr);
	D3DCompileFromFile(
		L"Resources/Shaders/ShaderGpuParticle.hlsl",
		nullptr, nullptr,
		"emitParticle", "cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobEmit,
		nullptr);
	D3DCompileFromFile(
		L"Resources/Shaders/ShaderGpuParticle.hlsl",
		nullptr, nullptr,
		"updateParticle", "cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobUpdate,
		nullptr);

	// PSOの作成
	ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = rootSignature.Get();

	//initialize用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobInit.Get());
	device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	m_pipelines[PSO_CS_INIT] = pipelineState;

	//emit用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobEmit.Get());
	device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	m_pipelines[PSO_CS_EMIT] = pipelineState;

	//update用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobUpdate.Get());
	device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	m_pipelines[PSO_CS_UPDATE] = pipelineState;


	//デスクリプタヒープ
	// Describe and create a constant buffer view (CBV), Shader resource
	// view (SRV), and unordered access view (UAV) descriptor heap.
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
	cbvSrvUavHeapDesc.NumDescriptors = 3 * 3;
	cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&m_cbvSrvUavHeap));

}

void ParticleManager::InitializeVerticeBuff()
{

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPos)) * numParticles;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProps.CreationNodeMask = 1;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProps.VisibleNodeMask = 1;


	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;


	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexPos);



	UINT64 bufferSize;
	bufferSize = sizeof(GpuParticleElement) * MaxParticleCount;
	auto resDescParticleElement = CD3DX12_RESOURCE_DESC::Buffer(
		bufferSize,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
	);
	m_gpuParticleElement = MyFunction::CreateResource(resDescParticleElement, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, D3D12_HEAP_TYPE_DEFAULT);
	m_gpuParticleElement->SetName(L"ParticleElement");

	bufferSize = sizeof(UINT) * MaxParticleCount;
	UINT uavCounterAlign = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1;
	bufferSize = UINT64(bufferSize + uavCounterAlign) & ~static_cast<UINT64>(uavCounterAlign);
	bufferSize += sizeof(Vector4);   // カウンタをこの場所先頭に配置.

	auto resDescParticleIndexList = CD3DX12_RESOURCE_DESC::Buffer(
		bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	m_gpuParticleIndexList = MyFunction::CreateResource(resDescParticleIndexList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, D3D12_HEAP_TYPE_DEFAULT);
	m_gpuParticleIndexList->SetName(L"ParticleIndexList");

	UINT64 offsetToCounter = bufferSize - sizeof(Vector4);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = MaxParticleCount;
	// インデックス用バッファの後方でカウンタを設置する.
	uavDesc.Buffer.CounterOffsetInBytes = offsetToCounter;
	uavDesc.Buffer.StructureByteStride = sizeof(UINT);

	CD3DX12_CPU_DESCRIPTOR_HANDLE processedCommandsHandle(m_cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart(), 2, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	
	processedCommandsHandle_ = processedCommandsHandle;
	device->CreateUnorderedAccessView(
		m_gpuParticleIndexList.Get(),
		m_gpuParticleIndexList.Get(),
		&uavDesc, processedCommandsHandle_
	);

}

void ParticleManager::SetTextureHandle(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
}

void ParticleManager::Initialize()
{
	HRESULT result;

	InitializeVerticeBuff();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = numParticles;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.StructureByteStride = sizeof(VertexPos);

	device->CreateUnorderedAccessView(
		vertBuff.Get(),
		nullptr,
		&uavDesc,
		descriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);


	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES UploadHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC UploadResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(numParticles * sizeof(VertexPos));

	// アップロードバッファの作成
	device->CreateCommittedResource(
		&UploadHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&UploadResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)
	);

}

void ParticleManager::Update()
{

	// 死んでいるパーティクルを削除
	Particles.erase(std::remove_if(Particles.begin(), Particles.end(),
		[](const VertexPos& p) { return !p.alive; }), Particles.end());

	//// スケールが0以下になったものを削除
	//Particles.erase(std::remove_if(Particles.begin(), Particles.end(),
	//	[](const VertexPos& p) { return !p.scale; }), Particles.end());

}

void ParticleManager::Draw(const ViewProjection& view)
{
	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	//constMap->mat = matWorld * matView * matProjection;	// 行列の合成
	Matrix4 constMatToSend = view.matView;
	constMatToSend *= view.matProjection;
	constMap->mat = constMatToSend;	// 行列の合成
	constMap->matBillboard = view.matBillboard;
	constBuff->Unmap(0, nullptr);

	// nullptrチェック
	assert(device);
	assert(ParticleManager::cmdList);


	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmdList, 1, textureHandle_);
	// 描画コマンド
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	cmdList->DrawInstanced(static_cast<UINT>(Particles.size()), 1, 0, 0);

	CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(vertBuff.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	cmdList->ResourceBarrier(1, &transitionBarrier);
}

void ParticleManager::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{

	if (Particles.size()) {

		CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(vertBuff.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
		cmdList->ResourceBarrier(1, &transitionBarrier);

		// パーティクルデータをアップロードバッファにコピー
		vertBuff->Map(0, nullptr, &mappedData);
		memcpy(mappedData, Particles.data(), Particles.size() * sizeof(VertexPos));
		vertBuff->Unmap(0, nullptr);

		// パイプラインステートの設定
		cmdList->SetPipelineState(pipelineState.Get());
		// ルートシグネチャの設定
		cmdList->SetComputeRootSignature(rootSignature.Get());
		// Set the particle buffer as a UAV.
		cmdList->SetComputeRootUnorderedAccessView(0, vertBuff->GetGPUVirtualAddress());

		// コンピュートシェーダーを実行
		cmdList->Dispatch(static_cast<UINT>(Particles.size() / 256 + 1), 1, 1);

	}

}

void ParticleManager::Add(Vector3& position, Vector3& velocity, uint32_t& MaxFrame)
{
	if (numParticles > Particles.size()) {
		//追加した要素の参照
		VertexPos p;
		//値のセットt
		p.position = position;
		p.velocity = velocity;
		p.FinalVelocity = { 0,0,0 };
		p.Frame = 0;
		p.MaxFrame = MaxFrame;
		p.alive = 1;
		p.scale = 1;
		p.color = { 1,1,1,1 };
		p.MinusAlpha = 0.0f;
		Particles.push_back(p);
	}
}

void ParticleManager::Add(Vector3& position, Vector3& velocity, uint32_t& MaxFrame, Vector4& color, Vector4& DownColor, const float& scale)
{
	if (numParticles > Particles.size()) {
		//追加した要素の参照
		VertexPos p;
		//値のセットt
		p.position = position;
		p.velocity = velocity;
		p.FinalVelocity = velocity / static_cast<float>(MaxFrame);
		p.Frame = 0;
		p.MaxFrame = MaxFrame;
		p.alive = 1;
		p.scale = scale;
		p.color = color;
		p.DownColor = DownColor;
		p.MinusAlpha = color.w / static_cast<float>(MaxFrame);
		Particles.push_back(p);
	}
}

void ParticleManager::Add(Vector3& position, Vector3& velocity, uint32_t& MaxFrame, Vector4& color, Vector4& DownColor, const float& scale, const float& DownScale)
{
	if (numParticles > Particles.size()) {
		//追加した要素の参照
		VertexPos p;
		//値のセットt
		p.position = position;
		p.velocity = velocity;
		p.FinalVelocity = velocity / static_cast<float>(MaxFrame);
		p.Frame = 0;
		p.MaxFrame = MaxFrame;
		p.alive = 1;
		p.scale = scale;
		p.color = color;
		p.DownColor = DownColor;
		p.MinusAlpha = color.w / static_cast<float>(MaxFrame);
		p.DownScale = DownScale;
		Particles.push_back(p);
	}
}

void ParticleManager::CopyData()
{

	VertexPos* outPutDeta = nullptr;
	vertBuff->Map(0, nullptr, (void**)&outPutDeta);
	memcpy(Particles.data(), outPutDeta, Particles.size() * sizeof(VertexPos));
	vertBuff->Unmap(0, nullptr);

}

