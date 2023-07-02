#pragma once
#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include"Vector3.h"
#include"Vector2.h"

class Particle {

public:
    // パーティクル用頂点レイアウト
    struct VERTEX_3D_PARTICLE
    {
        Vector3 Position;
        Vector2 TexCoord;
    };
    //今回使うパーティクルの資料例
    struct ParticleCompute {
        // 座標
        Vector3 pos;
        // 速度
        Vector3 vel;
        // ライフ
        float life;
    };

private:
    // コンピュートシェーダー
    ID3D11ComputeShader mComputeShader;
    // パーティクル
    ParticleCompute* mparticle;
    // バッファ
    ID3D11Buffer* mVertexBuffer;
    ID3D11Buffer* mpParticleBuffer;
    ID3D11Buffer* mpResultBuffer;
    ID3D11Buffer* mpPositionBuffer;
    // SRV
    ID3D11ShaderResourceView* mpParticleSRV;
    ID3D11ShaderResourceView* mpPositionSRV;
    // UAV
    ID3D11UnorderedAccessView* mpResultUAV;

};