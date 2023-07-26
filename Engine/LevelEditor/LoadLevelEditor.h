#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include <Model.h>
#include "WorldTransform.h"
#include <map>
#include "TutorialEnemy.h"
#include "BulletShotEnemy.h"
#include "Ground.h"
#include "TouchableObject.h"

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

	std::list<TutorialEnemy*> GetTutorialEnemyList() { return tutorialEnemyList; }
	std::list<BulletShotEnemy*> GetBulletShotEnemyList() { return billetShotEnemyList; }
	std::vector<Vector3> GetFirstSpline() { return FirstSplineVec; }
	std::vector<Vector3> GetSpline() { return splineVec; }
	std::vector<Vector3> GetFinalSpline() { return FinalSplineVec; }
private:
	std::unique_ptr<LevelData> levelData;
	std::map<std::string, Model*> models;
	std::vector<ModelData> LoadedObjects;
	std::vector<ModelData> NewLoadObjects;
	std::list<TutorialEnemy*> tutorialEnemyList;
	std::list<BulletShotEnemy*> billetShotEnemyList;
	std::list<Ground*> ground;
	std::vector<Vector3> FirstSplineVec;
	std::vector<Vector3> splineVec;
	std::vector<Vector3> FinalSplineVec;

	//最初から読み込むやつ
	std::unique_ptr<Model> modelSrop;
	std::unique_ptr<Model> modelGround;
	std::unique_ptr<Model> modelNormalEnemy;
	std::unique_ptr<Model> modelBulletShotEnemy;
};