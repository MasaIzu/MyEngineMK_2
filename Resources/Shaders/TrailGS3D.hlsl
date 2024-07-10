#include "Trail3D.hlsli"

static const int vnum = 4;
static const int vnum111 = 3;

static const float4 offset_array_front[vnum] =
{
    float4(-1.000000, 1.000000, -1.000000, 0),
    float4(-1.000000, -1.000000, -1.000000, 0),
    float4(1.000000, 1.000000, -1.000000, 0),
    float4(1.000000, -1.000000, -1.000000, 0)
};

static const float4 offset_array_top[vnum] =
{
    float4(1.000000, 1.000000, -1.000000, 0),
    float4(-1.000000, 1.000000, -1.000000, 0),
    float4(1.000000, 1.000000, 1.000000, 0),
    float4(-1.000000, 1.000000, 1.000000, 0)
};

static const float4 offset_array_down[vnum] =
{
    float4(1.000000, -1.000000, -1.000000, 0),
    float4(-1.000000, -1.000000, -1.000000, 0),
    float4(1.000000, -1.000000, 1.000000, 0),
    float4(-1.000000, -1.000000, 1.000000, 0)
};

static const float4 offset_array3333[vnum111] =
{
    float4(1.000000, 1.000000, 1.000000, 0),
    float4(1.000000, -1.000000, 1.000000, 0),
    float4(-1.000000, 1.000000, -1.000000, 0),
};


[maxvertexcount(36)]
void main(
	line VSOutput input[2] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	
    float4 offset;
        
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            GSOutput element;
                        
            offset = offset_array_front[i + j];
            float3 angle3 = angle;
            float4 rotoff = mul(Rot(angle3), offset);
            element.svpos = input[0].svpos + rotoff;
            element.svpos = mul(view, element.svpos);
            element.svpos = mul(projection, element.svpos);
            element.uv = input[0].uv;
            element.color = input[0].color;
           
            output.Append(element);
        }
        output.RestartStrip();
    }
    
    //for (int i = 0; i < 2; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //    {
    //        GSOutput element;
                        
    //        offset = offset_array_top[i + j];
            
    //        if (j < 2 - i)
    //        {
    //            element.svpos = input[1].svpos + offset;
    //            element.svpos = mul(view, element.svpos);
    //            element.svpos = mul(projection, element.svpos);
    //            element.uv = input[1].uv;
    //            element.color = input[1].color;
    //        }
    //        else
    //        {
    //            element.svpos = input[0].svpos + offset;
    //            element.svpos = mul(view, element.svpos);
    //            element.svpos = mul(projection, element.svpos);
    //            element.uv = input[0].uv;
    //            element.color = input[0].color;
    //        }
    //        output.Append(element);
    //    }
    //    output.RestartStrip();
    //}
    
    //for (int i = 0; i < 2; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //    {
    //        GSOutput element;
                        
    //        offset = offset_array_down[i + j];
            
    //        if (j < 2 - i)
    //        {
    //            element.svpos = input[1].svpos + offset;
    //            element.svpos = mul(view, element.svpos);
    //            element.svpos = mul(projection, element.svpos);
    //            element.uv = input[1].uv;
    //            element.color = input[1].color;
    //        }
    //        else
    //        {
    //            element.svpos = input[0].svpos + offset;
    //            element.svpos = mul(view, element.svpos);
    //            element.svpos = mul(projection, element.svpos);
    //            element.uv = input[0].uv;
    //            element.color = input[0].color;
    //        }
    //        output.Append(element);
    //    }
    //    output.RestartStrip();
    //}
    

}