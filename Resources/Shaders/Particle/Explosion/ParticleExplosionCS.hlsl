#include "ParticleExplosion.hlsli"

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

    uint number = gParticles[index].boostNumber;
    float speed = gParticles[index].speed;
    // 生き残っているパーティクルを動かす.
    float3 velocity = gParticles[index].velocity.xyz;
    
    float3 position = gParticles[index].position.xyz;

    position += velocity;

    //float scale = 0.8f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    //gParticles[index].keepParticleVelocity.xyz += velocity;
    //gParticles[index].scale = scale;
    gParticles[index].position.xyz = position;
    gParticles[index].velocity.xyz = velocity;
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
    uint ParticleCounter = MaxParticleCount / 4;
    if (Shot == 0)
    {
        return;
    }
   
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    
    float velSize = 0.2f;
    float particleBoostSize = velSize;
    
    float3 velocity;
    velocity.x = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.z = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.y = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    
    float4 BladeEndPos;
    
    float TimerMax = 400.0f;
    float TimerMin = 300.0f;
    
    float LifeTime = Rand1(seed, TimerMax, TimerMin);
    
    float speed = (TimerMax / LifeTime);
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = pos.xyz;
    gParticles[index].scale = 2.0f;
    gParticles[index].keepParticleVelocity = float4(0, 0, 0, 0);
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(1, 1, 1, 1);
    gParticles[index].speed = speed;
    
}
