#pragma once
#include <d3d12.h>


class ParticleManager {

public:
	static void Initialize(ID3D12Device* device);

	static void Finalize();

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

};