struct Particle
{
    float3 position;
    float3 velocity;
    int Frame; // ���̃p�[�e�B�N�������܂ꂽ�t���[��
    int MaxFrame; //���̃p�[�e�B�N���̎���
    bool alive; // ���̃p�[�e�B�N���������Ă��邩�ǂ���
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