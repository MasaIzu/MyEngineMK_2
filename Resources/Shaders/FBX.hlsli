cbuffer WorldTransform : register(b0) {
	matrix world; // ワールド行列
};

cbuffer ViewProjection : register(b1) {
	matrix view;       // ビュー変換行列
	matrix projection; // プロジェクション変換行列
	float3 cameraPos; // カメラ座標（ワールド座標）
};

cbuffer Material : register(b2) {
	float3 m_ambient  : packoffset(c0); // アンビエント係数
	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w);	// アルファ
}

//バーテックスバッファーの入力
struct VSInput
{
	float4 pos	: POSITION;//位置   
	float3 normal : NORMAL;//頂点法線
	float2 uv	: TEXCOORD;//テクスチャー座標
	uint4 boneIndices : BONEINDICES;//ボーンの番号
	float4 boneWeights : BONEWEIGHTS;//ボーンのスキンウェイト
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
};

//ボーンの最大数
static const int MAX_BONES = 128;

cbuffer skinning:register(b3)//ボーンのスキニング行列が入る
{
	matrix matSkinning[MAX_BONES];
}

cbuffer initialMatrix : register(b4)//ボーンのスキニング行列が入る
{
	matrix initialMatrix;
}