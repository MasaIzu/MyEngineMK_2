#include "Skydome.h"

void Skydome::Initialize() {
	model_.reset(Model::CreateFromOBJ("skydome", true));
	//ƒ[ƒ‹ƒh•ÏŠ·‚Ì‰Šú‰»
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(600, 600, 600);
	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection& viewProjection_) {
	//3Dƒ‚ƒfƒ‹•`‰æ
	model_->Draw(worldTransform_, viewProjection_);
}