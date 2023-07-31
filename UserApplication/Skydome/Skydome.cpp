#include "Skydome.h"

void Skydome::Initialize() {
	model_.reset(Model::CreateFromOBJ("skydome", true));
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(600, 600, 600);
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection& viewProjection_) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection_);
}