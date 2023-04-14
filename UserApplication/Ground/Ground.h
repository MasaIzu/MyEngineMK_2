#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"
#include<random>

class TouchableObject;

struct ground
{
	WorldTransform pos;

};

struct block
{
	WorldTransform pos;//world座標

};
//add

class Ground {

public:

	Ground();
	~Ground();

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	void CreateGround();

	void CreateBlock();

	void CreateBlockV1(Vector3 pos);
	void CreateBlockV2(Vector3 pos);
	void CreateBlockV3(Vector3 pos);
	float Random(float num1, float num2);

	block newblock;

	static const int MaxCount = 5;
	//地面作成
	std::unique_ptr<Model>groundModel_ = nullptr;
	std::vector<ground> groundObjects;
	//壁のブロック１
	std::unique_ptr<Model>blockModel_ = nullptr;
	std::vector<block>blockObjects;
	//壁のブロック２
	std::unique_ptr<Model>blockModelV2_ = nullptr;
	
	//壁のブロック３
	std::unique_ptr<Model>blockModelV3_ = nullptr;
	
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	//std::unique_ptr<Model> model_ = nullptr;

	/*std::unique_ptr<Model> groundModel = nullptr;
	TouchableObject* objGround = nullptr;*/

	//ステージサイズの変更
	CONST FLOAT XtZ = 40;
	//高さの変更
	CONST FLOAT Y = -1.25f;


	

	

	//std::unique_ptr<Model> modeltable[groundMaxCount];

};