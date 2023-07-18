#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include <Model.h>
#include "WorldTransform.h"
#include <map>
#include "TutorialEnemy.h"
#include "BulletShotEnemy.h"

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

struct ModelData {
	Model* model;
	WorldTransform worldTrans;
};

// レベルデータの生成
class LoadLevelEditor {

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数

	LoadLevelEditor();
	~LoadLevelEditor();

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	LevelData* LoadFile(const std::string& fileName);

	void Initialize(const std::string& fileName);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	std::list<TutorialEnemy*> GetEnemyList() { return tutorialEnemyList; }

private:
	std::unique_ptr<LevelData> levelData;
	std::map<std::string, Model*> models;
	std::vector<ModelData> LoadedObjects;
	std::vector<ModelData> NewLoadObjects;
	std::list<TutorialEnemy*> tutorialEnemyList;

	//最初から読み込むやつ
	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	std::unique_ptr<Model> modelSphere;
};