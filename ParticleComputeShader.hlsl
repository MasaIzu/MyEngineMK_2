struct Particle
{
    float3 position : Position;
    float scale : Scale;
    float4 color : Color;
    float3 velocity : Velocity;
    int Frame : Frame; // ���̃p�[�e�B�N�������܂ꂽ�t���[��
    int MaxFrame : MaxFrame; //���̃p�[�e�B�N���̎���
    bool alive : Alive; // ���̃p�[�e�B�N���������Ă��邩�ǂ���
};

RWStructuredBuffer<Particle> particles;

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    Particle p = particles[id.x];

    // Update particle life.
    p.Frame += 1;

    p.position += p.velocity;

    if (p.Frame > p.MaxFrame)
    {
        p.alive = false;
    }
    
    
    particles[id.x] = p;
}