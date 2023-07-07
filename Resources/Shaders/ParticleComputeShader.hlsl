struct Particle
{
    float3 position : Position;
    float scale : Scale;
    float4 color : Color;
    float4 DownColor : DownColor;
    float3 velocity : Velocity;
    float3 FinalVelocity : FinalVelocity;
    int Frame : Frame; // ���̃p�[�e�B�N�������܂ꂽ�t���[��
    int MaxFrame : MaxFrame; //���̃p�[�e�B�N���̎���
    bool alive : Alive; // ���̃p�[�e�B�N���������Ă��邩�ǂ���
    float MinusAlpha : MinusAlpha;
};

RWStructuredBuffer<Particle> particles;

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    
    Particle p = particles[id.x];

    // Update particle life.
    p.Frame += 1;
    p.position += p.velocity;
    p.color.a -= p.MinusAlpha;
    p.velocity -= p.FinalVelocity;
    p.color -= p.DownColor;
    if (p.Frame > p.MaxFrame)
    {
        p.alive = false;
    }
    
    
    particles[id.x] = p;
}