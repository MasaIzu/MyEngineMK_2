#include "FbxAnimation.h"
#include <cassert>
#include<DirectXMath.h>

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const std::string FbxAnimation::baseDirectory = "Resources/";
const std::string FbxAnimation::defaultTextureFileName = "white1x1.png";

FbxAnimation::FbxAnimation()
{
}

FbxAnimation::~FbxAnimation()
{
	if (mScene == nullptr) {

	}
	else {
		// FBXシーン解放
		aiReleaseImport(mScene);

	}
}

void FbxAnimation::Load(const std::string& failPath)
{

	// モデルと同じ名前のフォルダから読み込む
	const std::string directoryPath = baseDirectory + failPath + "/";
	// 拡張子.fbxを付加
	const std::string fileName = failPath + ".fbx";
	// 連結してフルパスを得る
	const std::string fullpath = directoryPath + fileName;

	UINT flag = 0;
	flag |= aiProcess_Triangulate;//三角面化
	flag |= aiProcess_CalcTangentSpace;//接線ベクトル生成
	flag |= aiProcess_GenSmoothNormals;//スムージングベクトル生成
	flag |= aiProcess_GenUVCoords;//非マッピングを適切なUV座標に変換
	flag |= aiProcess_RemoveRedundantMaterials;//冗長なマテリアルを削除
	flag |= aiProcess_OptimizeMeshes;//メッシュ数を最適化
	flag |= aiProcess_MakeLeftHanded;//ノードを左手座標系に

	mScene = aiImportFile(fullpath.c_str(), flag);

	// ファイル名を指定してFBXファイルを読み込む
	if (!mScene) {
		assert(0);
	}

}

aiAnimation* FbxAnimation::GetAnimation(int AnimationNumber)
{
	return mScene->mAnimations[AnimationNumber];
}
