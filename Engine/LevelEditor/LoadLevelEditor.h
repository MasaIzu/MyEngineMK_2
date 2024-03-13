#pragma once


#include "Vector3.h"

#include "WorldTransform.h"

#include "Ground.h"
#include "TouchableObject.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <map>
#include <string>
#include <vector>
#include <Model.h>
#include <Torch.h>
MY_SUPPRESS_WARNINGS_END

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
	std::string ModelName = "";
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
	LevelData* LoadFile(const std::string& fileName,const Vector3& vec3);

	//初期化
	void Initialize(const std::string& fileName ,const Vector3& vec3);
	//更新
	void Update();
	//更新
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	//描画
	void Draw(const ViewProjection& viewProjection,const ViewProjection& LightViewProjection_);
	//描画
	void ShadowDraw(const ViewProjection& viewProjection);
	//描画
	void ParticleDraw(const ViewProjection& viewProjection);

	Vector3 GetBillPosition(const uint32_t& BillCount){return BillPos[ BillCount ]; }
		//最初のスプライン
	std::vector<Vector3> GetFirstSpline() { return FirstSplineVec; }
	//二つ目のスプライン
	std::vector<Vector3> GetSpline() { return SecondsplineVec; }
	//最後のスプライン
	std::vector<Vector3> GetFinalSpline() { return FinalSplineVec; }
	//ボスのスプライン
	std::vector<Vector3> GetBossSpline() { return BossEnemySplineVec; }
	//スロープスプライン
	std::vector<Vector3> GetStage2SropSpline() { return stage2SropSplineVec; }
	//カメラのスプライン
	std::vector<Vector3> GetCameraSpline() { return CameraSplineVec; }
	//ボスの生まれる場所
	Vector3 GetBossBonePos(){ return BossBonePos; }
private:
	std::unique_ptr<LevelData> levelData;
	std::map<std::string, Model*> models;
	std::vector<ModelData> LoadedObjects;
	std::vector<ModelData> NewLoadObjects;

	std::list<Ground*> ground;
	std::vector<std::unique_ptr<Torch>> torch;
	std::vector<Vector3> FirstSplineVec;
	std::vector<Vector3> SecondsplineVec;
	std::vector<Vector3> FinalSplineVec;
	std::vector<Vector3> BossEnemySplineVec;
	std::vector<Vector3> stage2SropSplineVec;
	std::vector<Vector3> CameraSplineVec;
	Vector3 BossBonePos;
	Vector3 JimenScale = { 200,200,200 };

	//最初から読み込むやつ
	std::unique_ptr<Model> modelSrop;
	std::unique_ptr<Model> modelGround;
	std::unique_ptr<Model> modelGoal;
	std::unique_ptr<Model> modelNormalEnemy;
	std::unique_ptr<Model> modelBulletShotEnemy;
	std::unique_ptr<Model> torchModel;

	std::vector<Vector3> BillPos;


};