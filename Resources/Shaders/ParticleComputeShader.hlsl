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

cbuffer SceneParameter : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4 lightDir;
    float4 cameraPosition;

    float4 forceCenter1;

    uint MaxParticleCount;
    float frameDeltaTime;
    uint padd0;
    uint padd1;

    float4 particleColors[8];
}

RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(32, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < 50000)
    {
        uint index = id.x;
        gParticles[index].isActive = 0;
        gDeadIndexList.Append(index);
    }
}

[numthreads(32, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    if (index >= 50000)
    {
        return;
    }
    if (gParticles[index].isActive == 0)
    {
        return;
    }
    const float dt = 0.01666;

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
    //velocity += gravity * dt;

    //if (position.y < 0)
    //{
    //    velocity.y *= -1;
    //    velocity *= 0.6;
    //    position.y = 0;

    //    if (length(velocity) < 0.001)
    //    {
    //        velocity = 0;
    //    }
    //}
#if 0
  // フォースの処理.
  float3 center = forceCenter1.xyz;
  float  radius = forceCenter1.w;

  float3 toCenter = center - position.xyz;
  float L = length(toCenter);
  if (L < radius) {
    float velocityLen = length(velocity);
    velocity = reflect(normalize(velocity), -normalize(toCenter));
    velocity *= velocityLen;

    position = radius * -normalize(toCenter) + center;
  }
#endif

    gParticles[index].position.xyz = position;
    //gParticles[index].velocity.xyz = velocity;
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);

float nextRand(inout uint s)
{
    s = (1664525u * s + 1013904223u);
    return saturate(float(s & 0x00FFFFFF) / float(0x01000000));
}

[numthreads(32, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = gFreeIndexList.Consume();
    if (gParticles[index].isActive > 0)
    {
        return;
    }

    float3 velocity = float3(0, 0.1, 0);
    float3 position = float3(0, 0, 0);

    uint seed = id.x + index * 1235;
    position.x = 0;
    position.z = 0;
    position.y = 10;

    float r = nextRand(seed) * 50;
    float theta = nextRand(seed) * 3.14192 * 2.0;
    velocity.x = 0;
    velocity.z = 0;
    velocity.y = 0.1f;

    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = position;
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = 300;
    //gParticles[index].colorIndex = floor(nextRand(seed) * 8) % 8;;
}
