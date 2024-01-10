#include "ParticleEditor.hlsli"

RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(128, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < MaxParticleCount)
    {
        uint index = id.x;
        uint seed = id.x + index * 1235;
        gParticles[index].isActive = 0;
        float GraceOfTimeMax = 100.0f;
        float GraceOfTimeMin = 1.0f;
        float GraceOfTime = Rand1(seed, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
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
    
    if (gParticles[index].graceOfTime >= 0)
    {
        gParticles[index].graceOfTime = gParticles[index].graceOfTime - dt;
        return;
    }
    
    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        gParticles[index].isActive = 0;
        return;
    }

    float3 Position = gParticles[index].position.xyz;
    
    if (EndPointActive)
    {
        float3 Velocity = gParticles[index].velocity.xyz;
        float3 EndPosPoint = EndPos.xyz;
        float3 PosToEndVec = normalize(EndPosPoint - Position);
        Velocity = normalize(lerp(Velocity, PosToEndVec, LerpStrength));
        gParticles[index].velocity.xyz = Velocity;
        Position += Velocity * gParticles[index].Speed;
    }
    else
    {
        float3 Velocity = gParticles[index].velocity.xyz;
        Position += Velocity * gParticles[index].Speed;
    }
    
    float4 Color = EndColor - StartColor;
    float Life = gParticles[index].MaxLifeTime - gParticles[index].lifeTime;
    float Ratio = Life / gParticles[index].MaxLifeTime;
    Color *= Ratio;
    gParticles[index].color = StartColor + Color;
    gParticles[index].position.xyz = Position;
    gParticles[index].scale += ScaleTinker;
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
    if (Shot == false)
    {
        return;
    }
   
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    
    float3 velocity = (0, 0, 0);
    if (RandomVelocity)
    {   
        velocity.x = nextRand(indexAdd) * nextRand1(seed);
        velocity.z = nextRand(indexAdd) * nextRand1(seed);
        velocity.y = nextRand(indexAdd) * nextRand1(seed);
    
        velocity = normalize(velocity);
        gParticles[index].velocity.xyz = velocity;
    }
    else
    {
        velocity = normalize(EndPos.xyz - StartPos.xyz);
        
        float PI = 3.141592;
        
        float RadX = (Angle.x * (PI / 180)) * 100;
        float RadY = (Angle.y * (PI / 180)) * 100;
        float RadZ = (Angle.z * (PI / 180)) * 100;
        
        float RandomAngleMin = 0.0f;
        float RandomAngleX = (Rand1(seed, RadX, RandomAngleMin) - (RadX / 2)) / 100;
        float4 rotationX = QuaternionFromAxisAngle(float3(1, 0, 0), RandomAngleX);
        
        float RandomAngleY = (Rand1(seed, RadY, RandomAngleMin) - (RadY / 2)) / 100;
        float4 rotationY = QuaternionFromAxisAngle(float3(0, 1, 0), RandomAngleY);
        
        float RandomAngleZ = (Rand1(seed, RadZ, RandomAngleMin) - (RadZ / 2)) / 100;
        float4 rotationZ = QuaternionFromAxisAngle(float3(0, 0, 1), RandomAngleZ);
        
        float3 RotVelocity = RotateVectorByQuaternion(velocity, rotationX);
        RotVelocity = RotateVectorByQuaternion(RotVelocity, rotationY);
        RotVelocity = RotateVectorByQuaternion(RotVelocity, rotationZ);
        
        gParticles[index].velocity.xyz = RotVelocity;
    }
    
    if (RandomLife)
    {
        float RandomLifeMin = 10.0f;
        float RandomLife_ = Rand1(seed, MaxLife, RandomLifeMin);
        gParticles[index].lifeTime = RandomLife_;
    }
    else
    {
        gParticles[index].lifeTime = MaxLife;
    }
    
    if (RandomSpeed)
    {
        float RandomSpeedMin = 10.0f;
        float RandomSpeedMax = 20.0f;
        float RandomSpeed_ = Rand1(seed, RandomSpeedMax, RandomSpeedMin);
        gParticles[index].Speed = RandomSpeed_ * 0.9f;
    }
    else
    {
        gParticles[index].Speed = Speed;
    }
    
    if (RandomScale)
    {
        float RandomScaleMin = 10.0f;
        float RandomScaleMax = 20.0f;
        float RandomScale_ = Rand1(seed, RandomScaleMax, RandomScaleMin) / 10;
        gParticles[index].scale = RandomScale_;
    }
    else
    {
        gParticles[index].scale = Scale;
    }
    
    gParticles[index].MaxLifeTime = gParticles[index].lifeTime;
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = StartPos.xyz;
    gParticles[index].color = StartColor;
    
}
