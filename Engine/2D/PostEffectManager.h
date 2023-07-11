#pragma once
#include "PostEffect.h"
#include "MixPostEffect.h"

class PostEffectManager {
public:

    static void Initialize(DirectXCore* dxCore, const uint32_t& WinWidth, const uint32_t& WinHeight);

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h����</param>
    static void PostDrawScene();

    static void Draw(ID3D12GraphicsCommandList* cmdList);

    static void Finalize();

private:




};