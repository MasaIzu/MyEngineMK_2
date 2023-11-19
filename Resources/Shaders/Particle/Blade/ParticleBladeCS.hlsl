#include "ParticleBlade.hlsli"

struct GpuParticleElement
{
    float4 position;
    float scale;
    float4 color;
    uint isActive; // 生存フラグ.
    float lifeTime;
    float elapsed;
    uint colorIndex;
    float4 velocity;
};

RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(128, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < MaxParticleCount)
    {
        uint index = id.x;
        gParticles[index].isActive = 0;

        gDeadIndexList.Append(index);
    }
}

[numthreads(128, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    if (index >= MaxParticleCount)
    {
        return;
    }
    if (gParticles[index].isActive == 0)
    {
        return;
    }
    const float dt = 1;

    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        gParticles[index].isActive = 0;
        gDeadIndexList.Append(index);
        return;
    }

  // 生き残っているパーティクルを動かす.
    float3 velocity = gParticles[index].velocity.xyz;
    float3 position = gParticles[index].position.xyz;

    float3 gravity = float3(0, -98.0, 0);
    position += velocity;

    
    
    gParticles[index].position.xyz = position.xyz;
    //gParticles[index].velocity.xyz = velocity;
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);


[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    if (gParticles[index].isActive > 0)
    {
        return;
    }

    
    
    float a = index;
    
    uint seed = id.x + index * 1235;

    float3 velocity;
    
    float r = nextRand(seed) * 50;
    float theta = nextRand(seed) * 3.14192 * 2.0;
    velocity.x = nextRand(seed) / 8;
    velocity.z = nextRand(seed) / 16;
    velocity.y = (nextRand1(seed) / 16) + 0.2;

    float4 BladeEndPos = normalize(EndPos - StartPos);
    BladeEndPos = (BladeEndPos * 10);
    
    float LifeTime = Rand1(seed,10,7) * 5;
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = float3(StartPos.xyz);
    gParticles[index].scale = 0.8f;
    gParticles[index].velocity.xyz = BladeEndPos.xyz / LifeTime;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].color = float4(1, 0.05, 0.05, 0.5);
    //gParticles[index].colorIndex = floor(nextRand(seed) * 8) % 8;;
}
