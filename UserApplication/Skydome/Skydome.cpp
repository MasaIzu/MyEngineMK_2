#include "Skydome.h"

void Skydome::Initialize() {
	model_.reset(Model::CreateFromOBJ("skydome", true));
	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(Size,Size,Size);
	//行列の転送
	worldTransform_.TransferMatrix();

}

void Skydome::Update() {

}


void Skydome::Draw(ViewProjection& viewProjection_) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}