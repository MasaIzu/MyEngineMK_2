#pragma once
#include <d3d12.h>


class ParticleManager {

public:
	static void Initialize(ID3D12Device* device);

	static void Finalize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

};