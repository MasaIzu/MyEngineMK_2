#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include"DirectXCore.h"
#include "Vector3.h"
#include "Vector4.h"


/// <summary>
/// ライト
/// </summary>
class LightGroup
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public: // 定数
	// 平行光源の数
	static const int kDirLightNum = 3;
	// 点光源の数
	static const int kPointLightNum = 3;
	// スポットライトの数
	static const int kSpotLightNum = 3;
	// 丸影の数
	static const int kCircleShadowNum = 1;

public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferLightData
	{
		Vector4 lightv;
		Vector3 lightcolor;
		unsigned int active;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		// 環境光の色
		Vector3 ambientColor;
		float pad1;
		// 平行光源用
		ConstBufferLightData dirLights[kDirLightNum];
		
	};


public: // 静的メンバ関数
	// インスタンス生成
	static LightGroup* Create();

public: // メンバ関数
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	// 定数バッファ転送
	void TransferConstBuffer();

	

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// 定数バッファのマップ
	ConstBufferData* constMap_ = nullptr;

	// 環境光の色
	Vector3 ambientColor_ = { 1,1,1 };


	// ライト方向（単位ベクトル）
	Vector4 lightdir = { 1,0,0,0 };
	// ライト色
	Vector3 lightcolor = { 1,1,1 };

	// ダーティフラグ
	bool dirty_ = false;
};

