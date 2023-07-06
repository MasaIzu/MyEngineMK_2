﻿﻿#include "LoadLevelEditor.h"

#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string MakeLevelEditor::kDefaultBaseDirectory = "Resources/levels/";
const std::string MakeLevelEditor::kExtension = ".json";

LevelData* MakeLevelEditor::LoadFile(const std::string& fileName) {
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

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation.x = static_cast<float>(transform["translation"][1]);
			objectData.translation.y = static_cast<float>(transform["translation"][2]);
			objectData.translation.z = -static_cast<float>(transform["translation"][0]);

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

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}

	return levelData;
}


void MakeLevelEditor::Initialize(const std::string& fileName)
{

	levelData.reset(LoadFile(fileName));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		Model* newObject = Model::CreateFromOBJ("UFO", true);

		// 座標
		WorldTransform Trans;
		Trans.Initialize();
		Trans.translation_ = objectData.translation;

		// 回転角
		Trans.SetRot(objectData.rotation);

		// 座標
		Trans.scale_ = objectData.scaling;

		// 配列に登録
		ModelTrans.push_back(Trans);
		objects.push_back(newObject);
	}

}

void MakeLevelEditor::Update()
{

	for (auto& modelTrans : ModelTrans) {
		modelTrans.TransferMatrix();
	}

}

void MakeLevelEditor::Draw(const ViewProjection& viewProjection)
{
	int i = 0;
	for (auto& object : objects) {
		object->Draw(ModelTrans[i], viewProjection);

		i++;
	}
}