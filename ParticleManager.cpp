#include "ParticleManager.h"
#include "DirectXCore.h"
#include "Model.h"
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* ParticleManager::device = nullptr;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;

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

	rootsignature.Reset();
	pipelinestate.Reset();

}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleManager::cmdList == nullptr);

	// コマンドリストをセット
	ParticleManager::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
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
	////加算合成
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
	//半透明合成
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
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

void ParticleManager::InitializeVerticeBuff()
{
	//VertexPos vertexs[1024];

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPos)) * 1024;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexPos);
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
}

void ParticleManager::Update()
{
	HRESULT result;
	Matrix4 matScale, matRot, matTrans;

	//// スケール、回転、平行移動行列の計算
	//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	//matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	//matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	//matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//// ワールド行列の合成
	//matWorld = XMMatrixIdentity(); // 変形をリセット
	//matWorld *= matScale; // ワールド行列にスケーリングを反映
	//matWorld *= matRot; // ワールド行列に回転を反映
	///*matWorld *= matBillboardY;*/
	//matWorld *= matTrans; // ワールド行列に平行移動を反映

	//// 親オブジェクトがあれば
	//if (parent != nullptr) {
	//	// 親オブジェクトのワールド行列を掛ける
	//	matWorld *= parent->matWorld;
	//}

	//寿命が尽きたパーティクルを全削除
	Particles.remove_if([](Particle& x) {
		return x.frame >= x.numFrame;
		});
	//全パーティクル
	for (std::list<Particle>::iterator it = Particles.begin(); it != Particles.end(); it++)
	{
		//経過フレーム数をカウント
		it->frame++;

		float f = 0.0f;

		switch (it->type)
		{
		case Type::Normal:

			f = (float)it->frame / it->numFrame;
			if (it->isBezier)
			{
				//速度による移動
				Vector3 p1 = it->startPosition * (1.0f - f) + it->controlPosition * f;
				Vector3 p2 = it->controlPosition * (1.0f - f) + it->endPosition * f;

				it->position = p1 * (1.0f - f) + p2 * f;
			}
			else
			{
				//速度による移動
				it->position = f * (it->endPosition - it->startPosition) + it->startPosition;
			}
			//スケールの線形補間
			it->scale = (it->endScale - it->startScale) * f;
			it->scale += it->startScale;

			//赤の線形補間
			it->color.x = (it->endColor.x - it->startColor.x) * f;
			it->color.x += it->startColor.x;
			//青の線形補間
			it->color.y = (it->endColor.y - it->startColor.y) * f;
			it->color.y += it->startColor.y;
			//緑の線形補間
			it->color.z = (it->endColor.z - it->startColor.z) * f;
			it->color.z += it->startColor.z;
			//緑の線形補間
			it->color.w = (it->endColor.w - it->startColor.w) * f;
			it->color.w += it->startColor.w;

			break;
		case Type::In:

			f = (float)it->frame / it->numFrame;
			if (it->isBezier)
			{
				//速度による移動
				Vector3 p1 = it->startPosition * (1.0f - easeInQuint(f)) + it->controlPosition * easeInQuint(f);
				Vector3 p2 = it->controlPosition * (1.0f - easeInQuint(f)) + it->endPosition * easeInQuint(f);

				it->position = p1 * (1.0f - easeInQuint(f)) + p2 * easeInQuint(f);
			}
			else
			{
				//速度による移動
				it->position = easeInQuint(f) * (it->endPosition - it->startPosition) + it->startPosition;
			}
			//スケールの線形補間
			it->scale = (it->endScale - it->startScale) * f;
			it->scale += it->startScale;

			//赤の線形補間
			it->color.x = (it->endColor.x - it->startColor.x) * f;
			it->color.x += it->startColor.x;
			//青の線形補間
			it->color.y = (it->endColor.y - it->startColor.y) * f;
			it->color.y += it->startColor.y;
			//緑の線形補間
			it->color.z = (it->endColor.z - it->startColor.z) * f;
			it->color.z += it->startColor.z;
			//緑の線形補間
			it->color.w = (it->endColor.w - it->startColor.w) * f;
			it->color.w += it->startColor.w;

			break;
		case Type::Out:

			f = (float)it->frame / it->numFrame;

			if (it->isBezier)
			{
				//速度による移動
				Vector3 p1 = it->startPosition * (1.0f - easeOutQuint(f)) + it->controlPosition * easeOutQuint(f);
				Vector3 p2 = it->controlPosition * (1.0f - easeOutQuint(f)) + it->endPosition * easeOutQuint(f);

				it->position = p1 * (1.0f - easeOutQuint(f)) + p2 * easeOutQuint(f);
			}
			else
			{
				//速度による移動
				it->position = easeOutQuint(f) * (it->endPosition - it->startPosition) + it->startPosition;
			}
			//スケールの線形補間
			it->scale = (it->endScale - it->startScale) * f;
			it->scale += it->startScale;

			//赤の線形補間
			it->color.x = (it->endColor.x - it->startColor.x) * f;
			it->color.x += it->startColor.x;
			//青の線形補間
			it->color.y = (it->endColor.y - it->startColor.y) * f;
			it->color.y += it->startColor.y;
			//緑の線形補間
			it->color.z = (it->endColor.z - it->startColor.z) * f;
			it->color.z += it->startColor.z;
			//緑の線形補間
			it->color.w = (it->endColor.w - it->startColor.w) * f;
			it->color.w += it->startColor.w;
			break;
		}
			//頂点バッファへデータ転送
			VertexPos* vertMap = nullptr;
			result = vertBuff->Map(0, nullptr, (void**)&vertMap);
			if (SUCCEEDED(result))
			{
				//パーティクルの情報を1つずつ反映
				for (std::list<Particle>::iterator it = Particles.begin(); it != Particles.end(); it++)
				{
					//座標
					vertMap->pos = it->position;
					vertMap->scale = it->scale;
					vertMap->color = it->color;
					//次の頂点へ
					vertMap++;
				}
				vertBuff->Unmap(0, nullptr);
			}
	}
}

	void ParticleManager::Draw(ViewProjection view)
	{
		HRESULT result;
		// 定数バッファへデータ転送
		ConstBufferData* constMap = nullptr;
		result = constBuff->Map(0, nullptr, (void**)&constMap);
		//constMap->color = color;
		//constMap->mat = matWorld * matView * matProjection;	// 行列の合成
		constMap->mat = view.matView * view.matProjection;	// 行列の合成
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
		cmdList->DrawInstanced(Particles.size(), 1, 0, 0);
	}

	void ParticleManager::Add(Type type, int life, bool isBezier, Vector3 startPosition, Vector3 controlPosition,Vector3 endPosition, float startScale, float endScale, Vector4 startColor, Vector4 endColor)
	{
		//リストに要素を追加
		Particles.emplace_front();
		//追加した要素の参照
		Particle& p = Particles.front();
		//値のセットt
		p.type = type;
		p.isBezier = isBezier;
		p.startPosition = startPosition;
		p.endPosition = endPosition;
		p.controlPosition =controlPosition;
		p.numFrame = life;
		p.startScale = startScale;
		p.endScale = endScale;
		p.startColor = startColor;
		p.endColor = endColor;
	}

