#include "Trail3D.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD, float4 color : TEXCOLOR)
{
	VSOutput output; 
	
    output.svpos = mul(view, pos);
    output.svpos = mul(projection, output.svpos);
	
    //output.svpos = pos;
	
	output.uv = uv;
    output.color = color;
	return output;
}