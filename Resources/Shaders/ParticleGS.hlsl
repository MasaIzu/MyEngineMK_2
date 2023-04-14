#include "Particle.hlsli"

////センターからのオフセット
//static const float4 offset_array[20][3] =
//{
//	{
//		float4(1  ,0  ,0.5,0),//左下
//		float4(0.5,1  ,0  ,0),//左上
//		float4(0  ,0.5,1  ,0),//左下
//	},
//	{
//		float4(-1  ,0  ,0.5,0),//左下
//		float4(-0.5,1  ,0  ,0),//左上
//		float4(0  ,0.5  ,1,0)//右下
//	},
//	{
//		float4(1  , 0  ,0.5,0),//左下
//		float4(0.5,-1  ,0  ,0),//左上
//		float4(0  ,-0.5,1  ,0),//左下
//	},
//	{
//		float4(-1  , 0  ,0.5,0),//左下
//		float4(-0.5,-1  ,0  ,0),//左上
//		float4(0   ,-0.5,1  ,0),//左下
//	},
//
//	{
//		float4(0.5,1  ,0,0),//左下
//		float4(-0.5,1  ,0,0),//左上
//		float4(0  ,0.5,1,0),//左下
//	},
//	{
//		float4(0.5,1  , 0,0),//左下
//		float4(-0.5,1  , 0,0),//左上
//		float4(0   ,0.5,-1,0),//左下
//	},
//	{
//		float4(0.5,-1  ,0,0),//左下
//		float4(-0.5,-1  ,0,0),//左上
//		float4(0  ,-0.5,1,0),//左下
//	},
//	{
//		float4(0.5,-1  , 0,0),//左下
//		float4(-0.5,-1  , 0,0),//左上
//		float4(0  ,-0.5,-1,0)//右下
//	},
//
//	{
//		float4(1  ,0, 0.5,0),//左下
//		float4(1  ,0,-0.5,0),//左上
//		float4(0.5,1, 0  ,0)//右下
//	},
//	{
//		float4(1  , 0, 0.5,0),//左下
//		float4(1  , 0,-0.5,0),//左上
//		float4(0.5,-1, 0  ,0)//右下
//	},
//	{
//		float4(-1  ,0, 0.5,0),//左下
//		float4(-1  ,0,-0.5,0),//左上
//		float4(-0.5,1, 0  ,0)//右下
//	},
//	{
//		float4(-1  , 0, 0.5,0),//左下
//		float4(-1  , 0,-0.5,0),//左上
//		float4(-0.5,-1, 0  ,0)//右下
//	},
//
//	{
//		float4(0, 0.5,1  ,0),//左下
//		float4(0,-0.5,1  ,0),//左上
//		float4(1, 0  ,0.5,0),//左下
//	},
//	{
//		float4( 0, 0.5,1  ,0),//左下
//		float4( 0,-0.5,1  ,0),//左上
//		float4(-1, 0  ,0.5,0),//左下
//	},
//	{
//		float4(0, 0.5,-1  ,0),//左下
//		float4(0,-0.5,-1  ,0),//左上
//		float4(1, 0  ,-0.5,0)//右下
//	},
//	{
//		float4( 0, 0.5,-1  ,0),//左下
//		float4( 0,-0.5,-1  ,0),//左上
//		float4(-1, 0  ,-0.5,0)//右下
//	},
//
//	{
//		float4(1  , 0  ,-0.5,0),//左下
//		float4(0.5, 1  , 0  ,0),//左上
//		float4(0  ,0.5,-1  ,0),//左下
//	},
//	{
//		float4(-1  ,0  ,-0.5,0),//左下
//		float4(-0.5,1  , 0  ,0),//左上
//		float4( 0  ,0.5,-1  ,0),//左下
//	},
//	{
//		float4(1  , 0  ,-0.5,0),//左下
//		float4(0.5,-1  , 0  ,0),//左上
//		float4(0  ,-0.5,-1  ,0),//左下
//	},
//	{
//		float4(-1  , 0  ,-0.5,0),//左下
//		float4(-0.5,-1  , 0  ,0),//左上
//		float4( 0  ,-0.5,-1  ,0),//左下
//	},
//};
//
//static const float2 uv_array[3] =
//{
//	float2(0.0,1.0),
//	float2(0.5,0.0),
//	float2(1.0,1.0),
//};
//
//[maxvertexcount(20 * 3)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout TriangleStream<GSOutput>output
//)
//{
//	float4 offset;
//	GSOutput element;
//	for (uint j = 0; j < 20; j++)
//	{
//		for (uint i = 0; i < 3; i++)
//		{
//			//offset = mul(matBillboard, offset_array[i]);
//			offset = offset_array[j][i] * input[0].scale;
//			//offset = mul(matBillboard, offset);
//			element.svpos = input[0].pos + offset;
//			//element.svpos = input[0].pos + offset_array[i];
//			element.svpos =  element.svpos;
//			element.uv = uv_array[i]; 
//			element.color = input[0].color;
//			output.Append(element);
//		}
//		output.RestartStrip();
//	}
//}

static const int vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.3f,-0.3f,0,0),//左下
	float4(-0.3f,+0.3f,0,0),//左上
	float4(+0.3f,-0.3f,0,0),//右下
	float4(+0.3f,+0.3f,0,0)//右上
};

static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0),
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput>output
)
{
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos;
	//	element.normal = input[i].normal;
	//	element.uv = input[i].uv;
	//	output.Append(element);
	//}

	////現在のストリップを終了
	//output.RestartStrip();

	////2つ目の三角形
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos+float4(20.0f,0.0f,0.0f,0.0f);
	//	element.normal = input[i].normal;
	//	//UVを5倍に
	//	element.uv = input[i].uv * 5.0f;
	//	output.Append(element);
	//}
	float4 offset;
	GSOutput element;
	for (uint i = 0; i < vnum; i++)
	{
		//offset = mul(matBillboard, offset_array[i]);
		offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);
		element.svpos = input[0].pos + offset;
		//element.svpos = input[0].pos + offset_array[i];
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}