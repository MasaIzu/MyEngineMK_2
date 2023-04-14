#include "wing.h"
#include <cassert>
#include "Affin.h"

void Wing::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//初期座標をセット
	worldTransform_.translation_ = Vector3{ 0,0,0 };

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3{ 5,5,10 };

	//変数初期化
	make = false;
	time = 0;
}

void Wing::Update(Vector3 pos) {
	//プレイヤーの座標を合わせる
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.z += 10;

	//タイマーカウント
	if (time > 0) {
		time--;
	}
	else {
		make = false;
	}

	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

}

void Wing::Draw(ViewProjection viewProjection) {
	if (make == true) {
		//3Dモデルを描画
		model_->Draw(worldTransform_, viewProjection);
	}
}

void Wing::Distance() {
	make = true;
	time = 1;
}