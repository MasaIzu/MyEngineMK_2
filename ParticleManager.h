#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mesh.h"
#include "Light.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	//// 頂点データ構造体
	//struct VertexPosNormalUv
	//{
	//	Vector3 pos; // xyz座標
	//	Vector3 normal; // 法線ベクトル
	//	vextor2 uv;  // uv座標
	//};

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
		Vector4 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		//Vector4 color;	// 色 (RGBA)
		Matrix4 mat;	// 3D変換行列
		Matrix4 matBillboard;//ビルボード行列
	};

	enum class Type
	{
		Normal,
		Out,
		In,
	};

	//パーティクル一粒
	struct Particle
	{
		Type type;
		//曲線を描くか？
		bool isBezier=false;
		//座標
		Vector3 position = {};

		Vector3 startPosition;

		Vector3 controlPosition;

		Vector3 endPosition;
		// 現在フレーム
		int frame = 0;
		//終了フレーム
		int numFrame = 0;

		float scale = 1.0f;

		float startScale = 1.0f;

		float endScale = 0.0f;

		Vector4 color = {};

		Vector4 startColor = {};

		Vector4 endColor = {};

	};


public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device);


	/// <summary>
	/// 静的終了処理
	/// </summary>
	static void StaticFinalize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

private:// 静的メンバ関数

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	void InitializeVerticeBuff();

public: // メンバ関数
	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection view);

	void SetTextureHandle(uint32_t textureHandle);

	size_t GetParticlesListSize() {return Particles.size() ; }

	/// <summary>
	/// イージングパーティクル（out)
	/// </summary>
	/// <param name="life">寿命(イージングなんでスピードにも関係する)</param>
	/// <param name="isBezier">ベジェ曲線にする？</param>
	/// <param name="startPosition">初期座標</param>
	/// <param name="controlPosition">制御点座標</param>
	/// <param name="endPosition">最終座標</param>
	/// <param name="startScale">初期サイズ</param>
	/// <param name="endScale">最終サイズ</param>
	/// <param name="startColor">初期色</param>
	/// <param name="endColor">最終色</param>
	void Add(Type type, int life,bool isBezier, Vector3 startPosition,Vector3 controlPosition,Vector3 endPosition, float startScale, float endScale, Vector4 startColor, Vector4 endColor);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	//// 色
	//Vector4 color = { 1,1,1,1 };
	//// ローカルスケール
	//Vector3 scale = { 1,1,1 };
	//// X,Y,Z軸回りのローカル回転角
	//Vector3 rotation = { 0,0,0 };
	//// ローカル座標
	//Vector3 position = { 0,0,0 };
	// ローカルワールド変換行列
	Matrix4 matWorld;
	//// 親オブジェクト
	//ParticleManager* parent = nullptr;

	//パーティクル配列
	std::list<Particle>Particles;

	UINT textureHandle_ = 0;
};
