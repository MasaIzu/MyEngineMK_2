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
		Vector3 position;
		float scale = 1;
		Vector4 color = { 1,1,1,1 };
		Vector3 velocity;
		uint32_t Frame = 0; // このパーティクルが生まれたフレーム
		uint32_t MaxFrame = 0;//このパーティクルの寿命
		bool alive = 0; // このパーティクルが生きているかどうか
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

	//// パーティクルの定義
	//struct Particle {
	//	Vector3 position;
	//	Vector3 velocity;
	//	int Frame; // このパーティクルが生まれたフレーム
	//	int MaxFrame;//このパーティクルの寿命
	//	bool alive; // このパーティクルが生きているかどうか
	//	float scale = 1;
	//	int a = 0;
	//	Vector4 color = { 1,1,1,1 };
	//};


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
	// コンピュートシェーダー用ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// コンピュートシェーダー用パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertPaticleStatus;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// アップロードバッファの作成
	ComPtr<ID3D12Resource> uploadBuffer;
	ComPtr<ID3D12Resource> m_cacheVertexBuffer;

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

	void CSUpdate(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& view);

	void SetTextureHandle(uint32_t textureHandle);

	size_t GetParticlesListSize() { return Particles.size(); }

	
	void Add(Vector3& position,Vector3& velocity, uint32_t& MaxFrame);

	//コンピュートシェーダー掛けた後のコピー処理
	void CopyData();

	


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
	std::vector<VertexPos>Particles;

	UINT textureHandle_ = 0;

	uint32_t numParticles = 1000000;
	// パーティクルデータをアップロードバッファにコピー
	void* mappedData = nullptr;

	// Create a fence.
	ComPtr<ID3D12Fence> fence;
	uint64_t fenceValue = 0;
};
