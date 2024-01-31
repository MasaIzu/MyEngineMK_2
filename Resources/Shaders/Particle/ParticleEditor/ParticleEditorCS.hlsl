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
        float GraceOfTime = Rand(seed, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
        gDeadIndexList.Append(index);
    }
}

[numthreads(128, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    uint seed = id.x + index * 1235;
    if (index >= MaxParticleCount)
    {
        return;
    }
    if (gParticles[index].isActive == 0)
    {
        return;
    }
    //if (Shot == false)
    //{
    //    return;
    //}
    const float dt = 1;
    bool isScaleChange = false;
    float3 Position = gParticles[index].position.xyz;
    float3 Velocity = gParticles[index].velocity.xyz;
    
    gParticles[index].GroupTimer = gParticles[index].GroupTimer - dt;
    if (ParticleGroup == 1)
    {
        if (ParticleGroupCount > index)
        {
            if (gParticles[index].GroupTimer <= 0)
            {
                gParticles[index].scale = 0;
                isScaleChange = true;
            }
        }
        else
        {
            gParticles[index].position.xyz = Position;
            if (gParticles[gParticles[index].GroupNumber].GroupTimer <= 0)
            {
                gParticles[index].position.xyz = Position;
            }
            else
            {
                Position = gParticles[gParticles[index].GroupNumber].position;
            }
        }
    }
    
    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    gParticles[index].MaxGroupTimer = gParticles[index].MaxGroupTimer - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        if (ParticleGroup == 1)
        {
            if (gParticles[index].MaxGroupTimer <= 0)
            {
                gParticles[index].isActive = 0;
                return;
            }
        }
        else
        {
            gParticles[index].isActive = 0;
            return;
        }
    }
    
    if (EndPointActive)
    {
        float3 EndPosPoint = EndPos.xyz;
        float3 PosToEndVec = normalize(EndPosPoint - Position);
        if (Interlocking)
        {
            // 生き残っているパーティクルを動かす.
            float4 BladeEndPos = normalize(EndPos - StartPos);
            BladeEndPos = normalize((StartPos + (BladeEndPos * InterlockingLength)) - gParticles[index].position);
            Velocity = gParticles[index].velocity.xyz;
            Velocity = lerp(Velocity, BladeEndPos.xyz, InterlockingStrength);
            Velocity = normalize(Velocity) * gParticles[index].Speed;
            Position += Velocity;
            gParticles[index].velocity.xyz = Velocity;
        }
        else
        {
            Velocity = normalize(lerp(Velocity, PosToEndVec, LerpStrength));
            gParticles[index].velocity.xyz = Velocity;
            Position += Velocity * gParticles[index].Speed;
        }
    }
    else
    {
        if (ParticleGroup == 0)
        {
            Position += Velocity * gParticles[index].Speed;
            Velocity.y = Velocity.y - GravityStrength;
            gParticles[index].velocity.xyz = normalize(Velocity);
        }
        else
        {
            if (ParticleGroupCount > index)
            {
                Position += Velocity * gParticles[index].Speed;
            }
            else
            {
                if (gParticles[index].GroupTimer <= 0)
                {
                    Position += Velocity * gParticles[index].Speed;
                    Velocity.y = Velocity.y - GravityStrength;
                    gParticles[index].velocity.xyz = normalize(Velocity);
                }
            }
        }
    }
    
    float4 Color = EndColor - StartColor;
    float Life = gParticles[index].MaxLifeTime - gParticles[index].lifeTime;
    float Ratio = Life / gParticles[index].MaxLifeTime;
    Color *= Ratio;
    
    gParticles[index].ScaleKeep += ScaleTinker;
    float scale = Scale;
    if (ScaleDownLifeTime)
    {
        scale = Scale * (gParticles[index].lifeTime / gParticles[index].MaxLifeTime);
    }
    
    
    gParticles[index].color = StartColor + Color;
    gParticles[index].position.xyz = Position;
    gParticles[index].scale = scale + gParticles[index].ScaleKeep;
    
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
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    if (Shot == 0)
    {
        float GraceOfTimeMax = 100.0f;
        float GraceOfTimeMin = 1.0f;
        float GraceOfTime = Rand(seed, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
        return;
    }
    gParticles[index].isActive = 1;
    
    if (!EmitParticles)
    {
        const float dt = 1;
        if (gParticles[index].graceOfTime >= 0)
        {
            gParticles[index].graceOfTime = gParticles[index].graceOfTime - dt;
            return;
        }
    }
    
    if (isLoad)
    {
        float GraceOfTimeMax = 100.0f;
        float GraceOfTimeMin = 1.0f;
        float GraceOfTime = Rand(indexAdd, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
    }
    
    float3 velocity;
    
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
        float RandomAngleX = (Rand(seed, RadX, RandomAngleMin) - (RadX / 2)) / 100;
        float4 rotationX = QuaternionFromAxisAngle(float3(1, 0, 0), RandomAngleX);
        
        float RandomAngleY = (Rand(seed, RadY, RandomAngleMin) - (RadY / 2)) / 100;
        float4 rotationY = QuaternionFromAxisAngle(float3(0, 1, 0), RandomAngleY);
        
        float RandomAngleZ = (Rand(seed, RadZ, RandomAngleMin) - (RadZ / 2)) / 100;
        float4 rotationZ = QuaternionFromAxisAngle(float3(0, 0, 1), RandomAngleZ);
        
        float3 RotVelocity = RotateVectorByQuaternion(velocity, rotationX);
        RotVelocity = RotateVectorByQuaternion(RotVelocity, rotationY);
        RotVelocity = RotateVectorByQuaternion(RotVelocity, rotationZ);
        
        gParticles[index].velocity.xyz = RotVelocity;
    }
    
    if (ParticleGroup == 1)
    {
        gParticles[index].GroupNumber = Rand(seed, ParticleGroupCount, 0);
        gParticles[index].MaxGroupTimer = MaxGroupTimer;
        
        if (ParticleGroupCount > index)
        {
            if (RandomLife)
            {
                float RandomLife_ = Rand(seed, RandomLifeMinMax.y, RandomLifeMinMax.x);
                gParticles[index].lifeTime = RandomLife_ + GroupTimer;
            }
            else
            {
                gParticles[index].lifeTime = MaxGroupTimer;
            }
            
            if (RandomParticleExplosion == 0)
            {
                gParticles[index].GroupTimer = GroupTimer;
            }
            else
            {
                float RandomExplosionTimer_ = Rand(seed, RandomGroupTimerMinMax.y, RandomGroupTimerMinMax.x);
                gParticles[index].GroupTimer = RandomExplosionTimer_;
            }
        }
        else
        {
            if (RandomLife)
            {
                float RandomLife_ = Rand(seed, RandomLifeMinMax.y, RandomLifeMinMax.x);
                gParticles[index].lifeTime = RandomLife_ + GroupTimer;
            }
            else
            {
                gParticles[index].lifeTime = MaxGroupTimer;
            }
            
            if (RandomParticleExplosion == 0)
            {
                gParticles[index].GroupTimer = GroupTimer;
            }
            else
            {
                gParticles[index].GroupTimer = gParticles[gParticles[index].GroupNumber].GroupTimer;
            }
        }
        
    }
    else
    {
        if (RandomLife)
        {
            float RandomLife_ = Rand(seed, RandomLifeMinMax.y, RandomLifeMinMax.x);
            gParticles[index].lifeTime = RandomLife_;
        }
        else
        {
            gParticles[index].lifeTime = MaxLife;
        }
    }
    
    if (RandomSpeed)
    {
        float RandomSpeed_ = Rand(seed, RandomSpeedMinMax.y, RandomSpeedMinMax.x) / SpeedDivideSize;
        gParticles[index].Speed = RandomSpeed_;
    }
    else
    {
        gParticles[index].Speed = Speed;
    }
    
    if (RandomScale)
    {
        float RandomScale_ = Rand(seed, RandomScaleMinMax.y, RandomScaleMinMax.x) / ScaleDivideSize;
        gParticles[index].scale = RandomScale_;
    }
    else
    {
        gParticles[index].scale = Scale;
    }
    
    gParticles[index].MaxLifeTime = gParticles[index].lifeTime;
    gParticles[index].position.xyz = StartPos.xyz;
    gParticles[index].color = StartColor;
    gParticles[index].ScaleKeep = 0;

}
