#include"PostEffectGlow.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  	// 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{

	float offsetU = 1/ 1280.0f;
	float offsetV = 1 / 720.0f;


	/*float4 color = tex.Sample(smp, input.uv + float2(offsetU,0.0f));

	color += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));

	color += tex.Sample(smp, input.uv + float2(0.0f,offsetV));

	color += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));

	color += tex.Sample(smp, input.uv + float2(offsetU, offsetV));
	color += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));
	color += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));
	color += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	color /= 8.0f;

	color+= tex.Sample(smp, input.uv);*/

	//float4 texcolor = tex.Sample(smp, input.uv);

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	//掛ける強度
	int kernelSize = KernelSize;

	// 平均値を計算するための総和
	float4 sum = float4(0, 0, 0, 0);

	// カーネルサイズの範囲内でピクセルをイテレート
	for (int y = -kernelSize; y <= kernelSize; ++y)
	{
		for (int x = -kernelSize; x <= kernelSize; ++x)
		{
			float2 offset = float2(x, y) * float2(offsetU, offsetV);
			float4 colortex0 = tex0.Sample(smp, input.uv + offset);
			sum += colortex0;
		}
	}

	// 総和をカーネルサイズで割ることで平均値を計算
	float kernelArea = (2 * kernelSize + 1) * (2 * kernelSize + 1);
	float4 averageColor = sum / kernelArea;

	float4 color = averageColor;

	//fmodってなに？ってなったから出す
	//x - y * floor(x / y);floorは浮動小数点値の端数を切り捨てて整数値に変換
	//uvは Xpix/画面横幅,Ypix/画面縦
	if (fmod(input.uv.y, 0.1f) < 0.05f) {

		color = colortex1;
	}

	return float4(color.rgb, 1);
}

//Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
//smpState smp : register(s0);      	// 0番スロットに設定されたサンプラー
//
//float3 sampleMain(float2 uv) {
//    return tex.Sample(smp, uv).rgb;
//}
//
//float3 sampleBox(float2 uv, float delta) {
//    float4 _MainTex_TexelSize = float4(0, 1, 0, 0);
//
//    float4 offset = _MainTex_TexelSize.xyxy * float2(-delta, delta).xxyy;
//    float3 sum = sampleMain(uv + offset.xy) + sampleMain(uv + offset.zy) + sampleMain(uv + offset.xw) + sampleMain(uv + offset.zw);
//    return sum * 0.25;
//}
//
//float4 main(VSOutput input) : SV_TARGET
//{
//    float _Intensity = 1;
//
//       float4 col = tex.Sample(smp, input.uv);
//                col.rgb += sampleBox(input.uv, 0.5) * _Intensity;
//                return col;
//}