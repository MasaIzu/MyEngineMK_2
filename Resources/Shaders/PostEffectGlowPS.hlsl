#include"PostEffectGlow.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);  	// 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

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

	//�|���鋭�x
	int kernelSize = KernelSize;

	// ���ϒl���v�Z���邽�߂̑��a
	float4 sum = float4(0, 0, 0, 0);

	// �J�[�l���T�C�Y�͈͓̔��Ńs�N�Z�����C�e���[�g
	for (int y = -kernelSize; y <= kernelSize; ++y)
	{
		for (int x = -kernelSize; x <= kernelSize; ++x)
		{
			float2 offset = float2(x, y) * float2(offsetU, offsetV);
			float4 colortex0 = tex0.Sample(smp, input.uv + offset);
			sum += colortex0;
		}
	}

	// ���a���J�[�l���T�C�Y�Ŋ��邱�Ƃŕ��ϒl���v�Z
	float kernelArea = (2 * kernelSize + 1) * (2 * kernelSize + 1);
	float4 averageColor = sum / kernelArea;

	float4 color = averageColor;

	//fmod���ĂȂɁH���ĂȂ�������o��
	//x - y * floor(x / y);floor�͕��������_�l�̒[����؂�̂ĂĐ����l�ɕϊ�
	//uv�� Xpix/��ʉ���,Ypix/��ʏc
	if (fmod(input.uv.y, 0.1f) < 0.05f) {

		color = colortex1;
	}

	return float4(color.rgb, 1);
}

//Texture2D<float4> tex : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
//smpState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
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