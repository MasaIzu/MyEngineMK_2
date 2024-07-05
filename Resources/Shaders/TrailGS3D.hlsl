#include "Trail3D.hlsli"

static const int vnum = 4;
static const int vnum111 = 3;

static const float4 offset_array1111[vnum111] =
{
    float4(-1.000000, 1.000000, -1.000000, 0),
    float4(1.000000, 1.000000, -1.000000, 0),
    float4(1.000000, 1.000000, 1.000000, 0),
};

static const float4 offset_array2222[vnum111] =
{
    float4(1.000000, 1.000000, 1.000000, 0),
    float4(1.000000, -1.000000, 1.000000, 0),
    float4(-1.000000, 1.000000, -1.000000, 0),
};

//センターからのオフセット
static const float4 offset_array[vnum] =
{
 //   float4(-1.0f, -1.0f, 0, 0), //左下
	//float4(-1.0f, +1.0f, 0, 0), //左上
	//float4(+1.0f, -1.0f, 0, 0), //右下
	//float4(+1.0f, +1.0f, 0, 0) //右上
	
	float4(-1.000000, -1.000000, -1.000000, 0),
    float4(1.000000, -1.000000, -1.000000, 0),
    float4(-1.000000, 1.000000, -1.000000, 0),
    float4(1.000000, 1.000000, -1.000000, 0)
    
    //float4(-1.000000, -1.000000, 1.000000, 0),
    //float4(1.000000, -1.000000, 1.000000, 0),
    //float4(-1.000000, 1.000000, 1.000000, 0),
    //float4(1.000000, 1.000000, 1.000000, 0)
};

//センターからのオフセット
static const float4 offset_array2[vnum] =
{   
    float4(-1.000000, 1.000000, 1.000000, 0),
    float4(1.000000, 1.000000, 1.000000, 0),
    float4(-1.000000, -1.000000, 1.000000, 0),
    float4(1.000000, -1.000000, 1.000000, 0)
};

static const float4 offset_array3[vnum] =
{
    float4(-1.000000, -1.000000, 1.000000, 0),
    float4(1.000000, -1.000000, 1.000000, 0),
    float4(-1.000000, -1.000000, -1.000000, 0),
    float4(1.000000, -1.000000, -1.000000, 0)
};

[maxvertexcount(4)]
void main(
	line VSOutput input[2] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	
    float4 offset;
    matrix mat = mul(view, projection);
    GSOutput element;
    //for (int i = 0; i < 4; i++)
    //{
    //    offset = offset_array[i] * 1;
    //    element.svpos = input[0].svpos + offset;
    //    element.svpos = mul(view, element.svpos);
    //    element.svpos = mul(projection, element.svpos);
    //    element.uv = input[0].uv;
    //    element.color = input[0].color;
    //    output.Append(element);
    //}
    
    for (int j = 0; j < 3; j++)
    {
        offset = offset_array1111[j] * 1;
        if (j < 2)
        {
            element.svpos = input[1].svpos + offset;
            element.svpos = mul(view, element.svpos);
            element.svpos = mul(projection, element.svpos);
            element.uv = input[1].uv;
            element.color = input[1].color;
        }
        else
        {
            element.svpos = input[0].svpos + offset;
            element.svpos = mul(view, element.svpos);
            element.svpos = mul(projection, element.svpos);
            element.uv = input[0].uv;
            element.color = input[0].color;
        }
        output.Append(element);
    }
    output.RestartStrip();
    
    for (int j = 0; j < 3; j++)
    {
        offset = offset_array2222[j] * 1;
        if (j < 2)
        {
            element.svpos = input[1].svpos + offset;
            element.svpos = mul(view, element.svpos);
            element.svpos = mul(projection, element.svpos);
            element.uv = input[1].uv;
            element.color = input[1].color;
        }
        else
        {
            element.svpos = input[0].svpos + offset;
            element.svpos = mul(view, element.svpos);
            element.svpos = mul(projection, element.svpos);
            element.uv = input[0].uv;
            element.color = input[0].color;
        }
        output.Append(element);
    }
    output.RestartStrip();
    
    
    
    //for (int j = 0; j < 4; j++)
    //{
    //    offset = offset_array3[j] * 1;
    //    element.svpos = input[0].svpos + offset;
    //    element.svpos = mul(view, element.svpos);
    //    element.svpos = mul(projection, element.svpos);
    //    element.uv = input[0].uv;
    //    element.color = input[0].color;
    //    output.Append(element);
    //}
    
}