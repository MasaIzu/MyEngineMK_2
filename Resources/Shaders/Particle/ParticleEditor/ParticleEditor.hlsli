
cbuffer ShaderViewParameters : register(b0)
{
    matrix mat : packoffset(c0); // 3D変換行列
    matrix matBillboard : packoffset(c4); //ビルボード行列
};

cbuffer ShaderDetailParameters : register(b1)
{
    float4 StartPos : packoffset(c0);
    float4 EndPos : packoffset(c1);
    float4 StartColor : packoffset(c2);
    float4 EndColor : packoffset(c3);
    float4 Angle : packoffset(c4);
    uint Shot : packoffset(c5);
    uint EndPointActive : packoffset(c5.y);
    uint RandomVelocity : packoffset(c5.z);
    uint RandomLife : packoffset(c5.w);
    uint RandomSpeed : packoffset(c6);
    uint RandomScale : packoffset(c6.y);
    float Speed : packoffset(c6.z);
    float LerpStrength : packoffset(c6.w);
    float Scale : packoffset(c7);
    float ScaleTinker : packoffset(c7.y);
    float MaxLife : packoffset(c7.z);
    uint MaxParticleCount : packoffset(c7.w);
    uint AdditiveSynthesis : packoffset(c8);
    uint isLoad : packoffset(c8.y);
    float2 RandomLifeMinMax : packoffset(c8.z);
    float2 RandomSpeedMinMax : packoffset(c9);
    float2 RandomScaleMinMax : packoffset(c9.z);
    float SpeedDivideSize : packoffset(c10);
    float ScaleDivideSize : packoffset(c10.y);
    float GravityStrength : packoffset(c10.z);
    uint Interlocking : packoffset(c10.w);
    float InterlockingStrength : packoffset(c11);
    float InterlockingLength : packoffset(c11.y);
    uint ScaleDownLifeTime : packoffset(c11.z);
    uint EmitParticles : packoffset(c11.w);
    uint ParticleGroup : packoffset(c12);
    uint ParticleGroupCount : packoffset(c12.y);
    float GroupTimer : packoffset(c12.z);
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 position : Position;
    float4 color : Color;
    float4 DownColor : DownColor;
    float4 velocity : Velocity;
    float scale : Scale;
    int Frame : Frame; // このパーティクルが生まれたフレーム
    int MaxFrame : MaxFrame; //このパーティクルの寿命
    uint alive : Alive; // このパーティクルが生きているかどうか
    float MinusAlpha : MinusAlpha;
};

//ジオメトリシェーダからピクセルシェーダーへの出力
struct GSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
	//float3 normal:NORMAL;//法線ベクトル
    float2 uv : TEXCOORD; //uv値
    float4 color : COLOR;
};

struct GpuParticleElement
{
    float4 position;
    float4 color;
    float4 velocity;
    uint isActive; // 生存フラグ.
    float lifeTime;
    float MaxLifeTime;
    float scale;
    float Speed;
    float graceOfTime;
    float ScaleKeep;
    uint GroupNumber;
    float GroupTimer;
};

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float Rand1(uint SEED, int MAX, int MIN)
{
    uint rand = wang_hash(SEED * 1847483629);
    float result;
    result = (rand % 1024) / 1024.0f;
    result = (MAX + abs(MIN)) * result - abs(MIN);
    return result;
}

float3 RandomVec3(uint SEED, int MAX, int MIN)
{
    float3 randomVec;
    
    randomVec.x = Rand1(SEED, MAX, MIN);
    randomVec.y = Rand1(SEED, MAX, MIN);
    randomVec.z = Rand1(SEED, MAX, MIN);
    
    return randomVec;
}

float nextRand(inout uint s)
{
    s = (1664525u * s + 1013904223u);
    return saturate(float(s & 0x00FFFFFF) / float(0x01000000)) - 0.5;
}

float nextRand1(inout uint s)
{
    s = (1664525u * s + 1013904223u);
    return saturate(float(s & 0x00FFFFFF) / float(0x01000000));
}

float3 lerp(const float3 start, const float3 end, const float t)
{
    return start * (1.0f - t) + end * t;
}

float4 lerp(const float4 start, const float4 end, const float t)
{
    return start * (1.0f - t) + end * t;
}

float4 QuaternionFromAxisAngle(float3 axis, float angle)
{
    float halfAngle = angle * 0.5;
    float s = sin(halfAngle);

    float4 quaternion;
    quaternion.w = cos(halfAngle);
    quaternion.xyz = axis * s;
    return quaternion;
}

// 四元数の積を計算する関数
float4 QuaternionMultiply(float4 q1, float4 q2)
{
    return float4(q1.w * q2.xyz + q2.w * q1.xyz + cross(q1.xyz, q2.xyz),
                  q1.w * q2.w - dot(q1.xyz, q2.xyz));
}

// 四元数によるベクトルの回転
float3 RotateVectorByQuaternion(float3 v, float4 q)
{
    float4 vQuat = float4(v, 0.0); // ベクトルを四元数に拡張
    float4 qConj = float4(-q.xyz, q.w); // 四元数の共役

    // 四元数の積を計算してベクトルに回転を適用
    float4 rotatedQuat = QuaternionMultiply(QuaternionMultiply(q, vQuat), qConj);

    return rotatedQuat.xyz; // 回転されたベクトルを返す
}