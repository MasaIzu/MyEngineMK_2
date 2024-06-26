#include "Trail3D.hlsli"

static const int vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
    float4(-1.0f, -1.0f, 0, 0), //左下
	float4(-1.0f, +1.0f, 0, 0), //左上
	float4(+1.0f, -1.0f, 0, 0), //右下
	float4(+1.0f, +1.0f, 0, 0) //右上
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