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
    // �p�[�e�B�N���p���_���C�A�E�g
    struct VERTEX_3D_PARTICLE
    {
        Vector3 Position;
        Vector2 TexCoord;
    };
    //����g���p�[�e�B�N���̎�����
    struct ParticleCompute {
        // ���W
        Vector3 pos;
        // ���x
        Vector3 vel;
        // ���C�t
        float life;
    };

private:
    // �R���s���[�g�V�F�[�_�[
    ID3D11ComputeShader mComputeShader;
    // �p�[�e�B�N��
    ParticleCompute* mparticle;
    // �o�b�t�@
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