#include "Ground.h"

using namespace DirectX;

Ground::Ground()
{
}

Ground::~Ground()
{
	//delete objGround;
	//delete modelGround;
	
}

void Ground::Initialize() {
	////NULLポインタチェック
	//assert(model);
	//model_.reset(model);
	////ワールド変換の初期化
	//worldTransform_.Initialize();

	//worldTransform_.translation_ = Vector3(0, -20.0f, 0);
	//worldTransform_.scale_ = Vector3(600, 100, 600);
	//worldTransform_.matWorld_ *= AffinTrans::Translation(worldTransform_.translation_);
	//worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	////行列の転送
	//worldTransform_.TransferMatrix();

	groundModel_ = std::make_unique<Model>();
	blockModel_ = std::make_unique<Model>();
	blockModelV2_ = std::make_unique<Model>();
	blockModelV3_ = std::make_unique<Model>();

	//地面の描画
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));
	blockModel_.reset(Model::CreateFromOBJ("Block", true));
	blockModelV2_.reset(Model::CreateFromOBJ("BlockV2", true));
	blockModelV3_.reset(Model::CreateFromOBJ("BlockV3", true));

	for (auto newGround : groundObjects)
	{
		Ground::CreateGround();
	}

	for (auto newblock : blockObjects)
	{
		Ground::CreateBlock();
	}

}

void Ground::Update() {

}

void Ground::Draw(ViewProjection viewProjection_) {
	//3Dモデル描画
	//model_->Draw(worldTransform_, viewProjection_);

	for (int i = 0; i < 64; i++)
	{
		groundModel_->Draw(groundObjects[i].pos, viewProjection_);

	}

	for (int  i = 0; i < 42; i++)
	{
		blockModel_->Draw(blockObjects[i].pos, viewProjection_);
	}
	
}

void Ground::CreateGround()
{
	//ground newGround;

	Vector3 SCALE = { XtZ,Y,XtZ};
	//newGround.pos.Initialize();
	//newGround.pos.TransferMatrix();

	//z
	for (int i = 0; i < MaxCount; i++)
	{ //x
		for (int j = 0; j < MaxCount; j++)
		{
			ground newGround;
			newGround.pos.Initialize();
			newGround.pos.TransferMatrix();

			float x = (j * SCALE.x) + (SCALE.x * j) - SCALE.x * (MaxCount);
			float y = SCALE.y;
			float z = (i * SCALE.z) + (SCALE.z * i) - SCALE.z * (MaxCount);

			//int modelIndex = rand() % 10;
			//newGround = Model::Create(modeltable[i]);

			newGround.pos.translation_ = { x,y,z };
			newGround.pos.scale_ = SCALE;
			newGround.pos.TransferMatrix();


			groundObjects.push_back(newGround);

		}
	}
}

void Ground::CreateBlock()
{
	 Vector3 SCALE= { XtZ,20,XtZ };
	//z
	for (int i = 0; i < MaxCount; i++)
	{ //x
		for (int j = 0; j < MaxCount; j++)
		{
			Vector3 POS = 
			{
				(j * SCALE.x) + (SCALE.x * j) - SCALE.x * (MaxCount),
				SCALE.y,
				(i * SCALE.z) + (SCALE.z * i) - SCALE.z * (MaxCount)
			};
			int select = Random(1, 3);
			if (i == 0 || j == 0 || i == MaxCount || j == MaxCount)
			{
				if (select == 1)
				{
					CreateBlockV1(POS);
				}
				else if (select == 2)
				{
					CreateBlockV2(POS);
				}
				else
				{
					CreateBlockV3(POS);
				}

			}

		}
	}
}

void Ground::CreateBlockV1(Vector3 pos)
{
	
	newblock.pos.Initialize();
	newblock.pos.translation_ = pos;

	newblock.pos.scale_.x = Random(40.0f, 60.0f);
	newblock.pos.scale_.y = Random(40.0f, 60.0f);
	newblock.pos.scale_.z = Random(40.0f, 60.0f);


	newblock.pos.TransferMatrix();

	blockObjects.push_back(newblock);
}

void Ground::CreateBlockV2(Vector3 pos)
{
	block newblockV2;
	newblockV2.pos.Initialize();
	newblockV2.pos.translation_ = pos;

	newblockV2.pos.scale_.x = Random(40.0f, 60.0f);
	newblockV2.pos.scale_.y = Random(40.0f, 60.0f);
	newblockV2.pos.scale_.z = Random(40.0f, 60.0f);

	newblockV2.pos.TransferMatrix();
	blockObjects.push_back(newblockV2);
}

void Ground::CreateBlockV3(Vector3 pos)
{
	block newblockV3;
	newblockV3.pos.Initialize();
	newblockV3.pos.translation_ = pos;

	newblockV3.pos.scale_.x = Random(4.0f, 60.0f);
	newblockV3.pos.scale_.y = Random(4.0f, 60.0f);
	newblockV3.pos.scale_.z = Random(4.0f, 60.0f);

	newblockV3.pos.TransferMatrix();
	blockObjects.push_back(newblockV3);
}

float Ground::Random(float num1, float num2)
{
	
		//引数から小さい方とおおきい方を分ける
		float min, max;
		min = num2;
		max = num1;
		if (num1 < num2) {
			min = num1;
			max = num2;
		}
		//乱数シード生成器
		std::random_device seedGem;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seedGem());
		//乱数範囲の指定
		std::uniform_real_distribution<float> dist(min, max);

		//ランダムな値を生成して返す
		return dist(engine);
	
}
