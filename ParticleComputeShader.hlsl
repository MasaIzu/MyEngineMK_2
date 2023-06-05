struct Particle
{
    float3 position;
    float3 velocity;
    int Frame; // このパーティクルが生まれたフレーム
    int MaxFrame; //このパーティクルの寿命
    bool alive; // このパーティクルが生きているかどうか
    float scale;
    float4 color;
};

RWStructuredBuffer<Particle> particles;

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    Particle p = particles[id.x];

    // Update particle life.
    p.Frame += 1;

    p.position += p.velocity;

    particles[id.x] = p;
}