#include "ParticleBoost.hlsli"

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
    float4 BladeEndPos = normalize(boostPos.EndPos[number] - boostPos.StartPos[number]);
    BladeEndPos = normalize((boostPos.EndPos[number] + (BladeEndPos * 15)) - gParticles[index].position);
    float3 velocity = gParticles[index].velocity.xyz;
    velocity = lerp(velocity, BladeEndPos.xyz, 0.4f);
    velocity *= speed;
    
    float3 position = boostPos.EndPos[number].xyz + gParticles[index].keepVelocity.xyz;

    position += velocity;

    float scale = 0.8f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    gParticles[index].keepVelocity.xyz += velocity;
    gParticles[index].scale = scale;
    gParticles[index].position.xyz = position;
    gParticles[index].velocity.xyz = velocity;
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);


[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    
    if (Shot == 0)
    {
        return;
    }
    
    if (gParticles[index].isActive > 0)
    {
        return;
    }
    
    uint ParticleCounter = MaxParticleCount / 4;
    
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    
    float velSize = 0.05f;
    
    float3 velocity;
    velocity.x = nextRand(indexAdd) + nextRand1(seed) * velSize;
    velocity.z = nextRand(indexAdd) + nextRand1(seed) * velSize;
    velocity.y = nextRand(indexAdd) + nextRand1(seed) * velSize;
    
    float4 BladeEndPos;
    if (ParticleCounter > index)
    {
        gParticles[index].boostNumber = 0;
    }
    else if (ParticleCounter * 2 > index)
    {
        gParticles[index].boostNumber = 1;
    }
    else if (ParticleCounter * 3 > index)
    {
        gParticles[index].boostNumber = 2;
    }
    else if (ParticleCounter * 4 > index)
    {
        gParticles[index].boostNumber = 3;
    }
    float TimerMax = 30.0f;
    float TimerMin = 10.0f;
    
    float LifeTime = Rand1(seed, TimerMax, TimerMin);
    
    float speed = (TimerMax / LifeTime) * 0.1f;
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = float3(boostPos.EndPos[gParticles[index].boostNumber].xyz);
    gParticles[index].scale = 0.8f;
    gParticles[index].keepVelocity = float4(0, 0, 0, 0);
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(1, 0.05, 0.05, 0.5);
    gParticles[index].speed = speed;
    
}
