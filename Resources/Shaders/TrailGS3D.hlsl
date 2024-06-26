#include "Trail3D.hlsli"

static const int vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
    float4(-1.0f, -1.0f, 0, 0), //����
	float4(-1.0f, +1.0f, 0, 0), //����
	float4(+1.0f, -1.0f, 0, 0), //�E��
	float4(+1.0f, +1.0f, 0, 0) //�E��
};


[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput>output
)
{
	
	float4 offset;
    matrix mat = mul(view, projection);
	GSOutput element;
	for (int i = 0; i < vnum; i++)
	{
		//offset = mul(matBillboard, offset_array[i]);
		offset = offset_array[i] * 1;
		//offset = mul(matBillboard, offset);
		element.svpos = input[0].svpos + offset;
		//element.svpos = mul(mat, element.svpos);
        element.uv = input[0].uv;
		element.color = input[0].color;
		output.Append(element);
	}
}