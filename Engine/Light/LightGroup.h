#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>

class LightGroup
{
public://サブクラス
	struct ConstBufferData
	{
		Vector3 ambientColor;
		float pad1;

		uint32_t pointLightCount = 0;
		uint32_t spotLightCount = 0;
		uint32_t circleShadowCount = 0;
		float pad2;

		//点光源用
		std::vector<PointLight::ConstBufferData> pointLights;
		//スポットライト用
		std::vector<SpotLight::ConstBufferData> spotLights;
		//丸影用
		std::vector<CircleShadow::ConstBufferData> circleShadows;
	};
	HRESULT result;

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;

public:
	//静的メンバ関数

	static void StaticInitialize(ID3D12Device* device);

public:
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns></returns>
	static LightGroup* Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	//平行
	void SetAmbientColor(const Vector3& color) {
		ambientColor = color;
		dirty = true;
	}

	uint32_t SetPointLight(const PointLight& pointLight) {
		dirty = true;
		pointLights.push_back(pointLight);
		return static_cast< uint32_t >( pointLights.size() - 1 );
	}

	uint32_t SetSpotLight(const SpotLight& spotLight) {
		dirty = true;
		spotLights.push_back(spotLight);
		return static_cast< uint32_t >( spotLights.size() - 1 );
	}

	uint32_t SetCircleShadow(const CircleShadow& circleShadow) {
		dirty = true;
		circleShadows.push_back(circleShadow);
		return static_cast< uint32_t >( circleShadows.size() - 1 );
	}

private:
	//メンバ変数
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	//環境光の色
	Vector3 ambientColor = { 0.3f,0.3f,0.3f };

	//点光源の配列
	std::vector<PointLight> pointLights;
	//スポットライト配列
	std::vector<SpotLight> spotLights;
	//丸影の配列
	std::vector<CircleShadow> circleShadows;

	//ダーティフラグ
	bool dirty = false;

};