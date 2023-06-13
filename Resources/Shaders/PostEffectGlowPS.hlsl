#include"PostEffectGlow.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  	// 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

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

	if (shadeNumber == 0) {
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
	else if (shadeNumber == 1) {
		float2 uv = input.uv;
		float2 direction = uv - center;
		float2 step = direction / float(samples);

		float4 result = tex0.Sample(smp, uv);
		float totalWeight = 1.0;
		for (int i = 1; i < samples; ++i)
		{
			float weight = (float(samples) - float(i)) / float(samples);
			float2 sampleUV = uv - step * float(i) * intensity;
			result += tex0.Sample(smp, sampleUV) * weight;
			totalWeight += weight;
		}

		result /= totalWeight;
		return result;
	}
    else if (shadeNumber == 2)
    {
		
        float4 colortex0 = tex0.Sample(smp, input.uv);
        float grayScale = colortex0.r * 0.299 + colortex0.g * 0.587 + colortex0.b * 0.114;
        float extract = smoothstep(0.6, 0.9, grayScale);
        float4 HighLumi = colortex0 * extract;
		
        float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;//Bloomはブラーを大げさに
        float4 col = float4(0, 0, 0, 0);
		
        for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)//xyで2の幅で色を取得
        {
            for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
            {
                float2 pickUV = input.uv + float2(px, py);
                float weight = Gaussian(input.uv, pickUV, _Sigma);
                col += tex0.Sample(smp, pickUV) * weight;
				
                //float4 colortex0 = tex0.Sample(smp, input.uv);
                float grayScale = col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
                float extract = smoothstep(0.6, 0.9, grayScale);
                float4 HighLumi = col * extract;
				
                totalWeight += weight;
            }
        }
        HighLumi.rgb = HighLumi.rgb / totalWeight;
		
        return col;
    }
	
	
	return float4(1,1,1,1);
}