#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(GSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp,input.uv);
	////シェーディングによる色
	//float4 shadecolor;
	////光沢度
	//const float shininess = 4.0f;

	float4 lightv = float4(1, 1, 0, 0);

	//float3 lightcolor = float3(1, 1, 1);

	////頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	////ライトに向かうベクトルと法線の内積
	float3 dotlightnormal = dot(lightv, input.normal);
	//float lightIntensity = smoothstep(0, 0.01, dotlightnormal);
	////反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);

	//shadecolor.a = m_alpha;

	//input.normal = normalize(input.normal);
	//float3 lightDir = normalize(-lightv);

	//// Ambient
	//float3 ambient = ambientColor;

	//// Diffuse
	//float diffuseIntensity = max(dot(input.normal, lightDir), 0.0);
	//diffuseIntensity = smoothstep(0.0, 1.0, diffuseIntensity); // Apply smoothstep
	//float3 diffuse = diffuseIntensity * diffuseColor;

	//// Specular
	//float spec = pow(max(dot(eyedir, reflect), 0.0), m_specular);
	//float3 specular = spec * specularColor;

	//// Combine
	//return float4(ambient + diffuse + specular, 1.0);



	////環境反射光m_specular
	//float3 ambient = m_ambient;
	////拡散反射光
	//float3 diffuse = dotlightnormal * m_diffuse;
	////鏡面反射光 
	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	////全て加算する
	//shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	//shadecolor.a = m_alpha;

	



	float4 baseCol = tex.Sample(smp, input.uv);

	// Ambient
	float3 ambient = m_ambient * ambientColor;
	float3 lightDir = normalize(lightv);

	// Diffuse
	float intensity = saturate(dot(normalize(input.normal), lightDir));
	intensity = smoothstep(minThreshold, maxThreshold, intensity);
	float3 diffuse = intensity * diffuseColor;

	// Specular
	float specIntensity = pow(saturate(dot(reflect, eyedir)), 40);
	specIntensity = smoothstep(minThreshold, maxThreshold, specIntensity);
	float3 specular = specIntensity * specularColor;

	float3 ads = (1 - intensity) * ambient; +diffuse + (1 - specIntensity) * specular;
	ads += diffuse;
	ads = (1 - specIntensity) * ads + specular;

	// Rim light
	float rimIntensity = pow(1.0 - saturate(dot(input.normal, eyedir)), rimLightPow);
	rimIntensity = smoothstep(minThreshold, maxThreshold, rimIntensity);
	float3 rimColor = rimIntensity * rimLightColor;

	return float4(ads, baseCol.a) * texcolor;

}