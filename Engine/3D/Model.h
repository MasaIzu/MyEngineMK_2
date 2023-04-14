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
/// モデルデータ
/// </summary>
class Model {
private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector4 color;	// 色 (RGBA)
		Matrix4 mat;	// ３Ｄ変換行列
	};

private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultModelName;

private: // 静的メンバ変数
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;
	// ライト
	static std::unique_ptr<LightGroup> lightGroup;

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize();

	// 静的初期化
	static void StaticFinalize();

	// グラフィックスパイプラインの初期化
	static void InitializeGraphicsPipeline();

	// 3Dモデル生成
	static Model* Create();

	// OBJファイルからメッシュ生成
	static Model* CreateFromOBJ(const std::string& modelname, bool smoothing = false);

	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	// 描画後処理
	static void PostDraw();

public: // メンバ関数
	// デストラクタ
	~Model();

	// 初期化
	void Initialize(const std::string& modelname, bool smoothing = false);

	// 描画
	void Draw(
		const WorldTransform& worldTransform, const ViewProjection& viewProjection);
	void Draw(
		const WorldTransform& worldTransform, const ViewProjection& viewProjection,
		uint32_t textureHadle);


	// メッシュコンテナを取得
	inline const std::vector<Mesh*>& GetMeshes() { return meshes_; }

	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<Mesh::VertexPosNormalUv>& GetVertices() { return vertices; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

	void SetTextureHandle(uint32_t textureHandle) { modelTextureHandle = textureHandle; }


private: // メンバ変数
	// 名前
	std::string name_;
	// メッシュコンテナ
	std::vector<Mesh*> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;

	//頂点データ
	std::vector<Mesh::VertexPosNormalUv>vertices;
	//頂点インデックス
	std::vector<unsigned short> indices;

	uint32_t modelTextureHandle = 0;

private: // メンバ関数

	// モデル読み込み
	void LoadModel(const std::string& modelname, bool smoothing);

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// マテリアル登録
	void AddMaterial(Material* material);

	// テクスチャ読み込み
	void LoadTextures();
};
