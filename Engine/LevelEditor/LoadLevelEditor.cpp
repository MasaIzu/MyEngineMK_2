#include "LoadLevelEditor.h"
#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string LoadLevelEditor::kDefaultBaseDirectory = "Resources/levels/";
const std::string LoadLevelEditor::kExtension = ".json";

LoadLevelEditor::LoadLevelEditor()
{
}

LoadLevelEditor::~LoadLevelEditor()
{

	//for (TutorialEnemy* enemy : tutorialEnemyList) {
	//	delete enemy;
	//}
	for (auto& object : NewLoadObjects) {
		if (object.model) {
			delete object.model;
		}
	}
	for (Ground* ground : ground) {
		delete ground;
	}
}

LevelData* LoadLevelEditor::LoadFile(const std::string& fileName) {
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();
		//std::string addType = object["file_name"].get<std::string>();
		if (object["file_name"].get<std::string>().empty()) {
			// MESH
			if (type.compare("MESH") == 0) {
				// 要素追加
				levelData->objects.emplace_back(LevelData::ObjectData{});
				// 今追加した要素の参照を得る
				LevelData::ObjectData& objectData = levelData->objects.back();

				if (object.contains("name")) {
					// ファイル名
					objectData.fileName = object["name"];
				}

				// トランスフォームのパラメータ読み込み
				nlohmann::json& transform = object["transform"];
				// 平行移動
				objectData.translation.x = static_cast<float>(transform["translation"][0]);
				objectData.translation.y = static_cast<float>(transform["translation"][2]);
				objectData.translation.z = static_cast<float>(transform["translation"][1]);

				// 回転角
				objectData.rotation.x = -static_cast<float>(transform["rotation"][1]);
				objectData.rotation.y = -static_cast<float>(transform["rotation"][2]);
				objectData.rotation.z = static_cast<float>(transform["rotation"][0]);
				// スケーリング
				objectData.scaling.x = static_cast<float>(transform["scaling"][1]);
				objectData.scaling.y = static_cast<float>(transform["scaling"][2]);
				objectData.scaling.z = static_cast<float>(transform["scaling"][0]);

				// TODO: コライダーのパラメータ読み込み
			}
		}
		else if (object["file_name"].get<std::string>() == "TutorialSropVec") {
			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			Vector3 trans = { static_cast<float>(transform["translation"][0]),static_cast<float>(transform["translation"][2]),static_cast<float>(transform["translation"][1]) };
			trans.x *= -1.0f;
			FirstSplineVec.push_back(trans);
		}
		else if (object["file_name"].get<std::string>() == "FIRSTSPLINE") {
			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			Vector3 trans = { static_cast<float>(transform["translation"][0]),static_cast<float>(transform["translation"][2]),static_cast<float>(transform["translation"][1]) };
			trans.x *= -1.0f;
			FirstSplineVec.push_back(trans);
		}
		else if (object["file_name"].get<std::string>() == "SPLINE") {
			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			Vector3 trans = { static_cast<float>(transform["translation"][0]),static_cast<float>(transform["translation"][2]),static_cast<float>(transform["translation"][1]) };
			trans.x *= -1.0f;
			splineVec.push_back(trans);
		}
		else if (object["file_name"].get<std::string>() == "FINALSPLINE") {
			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			Vector3 trans = { static_cast<float>(transform["translation"][0]),static_cast<float>(transform["translation"][2]),static_cast<float>(transform["translation"][1]) };
			trans.x *= -1.0f;
			FinalSplineVec.push_back(trans);
		}

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}

	return levelData;
}


void LoadLevelEditor::Initialize(const std::string& fileName)
{

	levelData.reset(LoadFile(fileName));

	// モデル読み込み
	modelSrop.reset(Model::CreateFromOBJ("srop1", true));
	modelGround.reset(Model::CreateFromOBJ("stageTest", true));
	modelGoal.reset(Model::CreateFromOBJ("goal", true));
	//modelFighter = Model::CreateFromOBJ("chr_sword", true);
	modelNormalEnemy.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	modelBulletShotEnemy.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));

	models.insert(std::make_pair("srop1", modelSrop.get()));
	models.insert(std::make_pair("stageTest", modelGround.get()));
	models.insert(std::make_pair("goal", modelGoal.get()));
	//models.insert(std::make_pair("chr_sword", modelFighter));
	models.insert(std::make_pair("sphereNormalEnemy", modelNormalEnemy.get()));
	models.insert(std::make_pair("sphereBulletEnemy", modelBulletShotEnemy.get()));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {

		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;

		std::string fileName;

		if (objectData.fileName.find(".") != std::string::npos) {
			for (uint32_t i = 0; i < objectData.fileName.length(); i++) {
				if (objectData.fileName.substr(i, 1) != ".") {
					fileName += objectData.fileName.substr(i, 1);
				}
				else {
					break;
				}
			}
		}
		else {
			fileName = objectData.fileName;
		}

		decltype(models)::iterator it = models.find(fileName);
		if (it != models.end()) {
			model = it->second;
			// 座標
			WorldTransform Trans;
			Trans.Initialize();
			Trans.translation_ = objectData.translation;// 座標
			Trans.scale_ = objectData.scaling;
			Trans.SetRot(objectData.rotation);// 回転角
			Trans.TransferMatrix();

			if (fileName == modelNormalEnemy->GetName()) {
				TutorialEnemy* tutorialEnemy = new TutorialEnemy(objectData.translation, model);
				tutorialEnemyList.push_back(tutorialEnemy);
			}
			else if (fileName == modelBulletShotEnemy->GetName()) {
				BulletShotEnemy* bulletShotEnemy = new BulletShotEnemy(objectData.translation, model);
				billetShotEnemyList.push_back(bulletShotEnemy);
			}
			else if (fileName == modelGround->GetName()) {
				Ground* tutorialEnemy = new Ground(modelGround.get());
				ground.push_back(tutorialEnemy);
			}
			else if (fileName == modelGoal->GetName()) {
				Ground* tutorialEnemy = new Ground(modelGoal.get());
				ground.push_back(tutorialEnemy);
			}
			else if (fileName == modelSrop->GetName()) {

				// 配列に登録
				ModelData Data;
				Data.model = Model::CreateFromOBJ("srop1", true);
				Data.worldTrans = Trans;
				NewLoadObjects.push_back(Data);
				TouchableObject::Create(modelSrop.get(), Trans, COLLISION_ATTR_RAIL);
			}
		}
		else {
			// モデルを指定して3Dオブジェクトを生成
			model = Model::CreateFromOBJ(fileName, true);
			// 座標
			WorldTransform Trans;
			Trans.Initialize();
			Trans.translation_ = objectData.translation;
			Trans.scale_ = objectData.scaling;// 座標
			Trans.SetRot(objectData.rotation);// 回転角

			// 配列に登録
			ModelData Data;
			Data.model = model;
			Data.worldTrans = Trans;
			Data.worldTrans.TransferMatrix();
			NewLoadObjects.push_back(Data);

			if (fileName == "cube") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "shahei") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "srop") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_FIRSTRAIL);
			}
			else if (fileName == "goalSrop") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_FINALRAIL);
			}
			else if (fileName == "foundationCube") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "stageSrop") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "tutorialSrop") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_FIRSTRAIL);
			}
		}

	}
	for (Ground* ground : ground) {
		ground->Initialze();
	}

	Update();
}

void LoadLevelEditor::Update()
{
	//for (auto& modelTrans : LoadedObjects) {
	//	modelTrans.worldTrans.TransferMatrix();
	//}
	for (auto& modelTrans : NewLoadObjects) {
		modelTrans.worldTrans.TransferMatrix();
	}
}

void LoadLevelEditor::Draw(const ViewProjection& viewProjection)
{
	for (Ground* ground : ground) {
		ground->Draw(viewProjection);
	}
	for (auto& object : NewLoadObjects) {
		object.model->Draw(object.worldTrans, viewProjection);
	}
}