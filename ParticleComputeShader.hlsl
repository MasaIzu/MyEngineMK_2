struct Particle
{
    float3 position : Position;
    float3 velocity : Velocity;
    int Frame : Frame; // このパーティクルが生まれたフレーム
    int MaxFrame : MaxFrame; //このパーティクルの寿命
    bool alive : Alive; // このパーティクルが生きているかどうか
    float scale : Scale;
    float4 color : Color;
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