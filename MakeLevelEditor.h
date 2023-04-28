﻿#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include <Model.h>
#include "WorldTransform.h"
#include <map>


// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		Vector3 translation;
		// 回転角
		Vector3 rotation;
		// スケーリング
		Vector3 scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

// レベルデータの生成
class MakeLevelEditor {

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数
	
	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	LevelData* LoadFile(const std::string& fileName);

	void Initialize(const std::string& fileName);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	std::unique_ptr<LevelData> levelData;


	std::map<std::string, Model*> models;
	std::vector<Model*> objects;
	std::vector<WorldTransform> ModelTrans;

};

