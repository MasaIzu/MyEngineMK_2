#include "DebugScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
//#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"

DebugScene::DebugScene() {}
DebugScene::~DebugScene() {

}

void DebugScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-1000 };
	viewProjection_->UpdateMatrix();

	int a = 1000000;

	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(100, 100, 100);

	model.reset(Model::CreateFromOBJ("sphere", true));
	//model1.reset(Model::CreateFromOBJ("Ground", true));



	// �����ʒu�͎��̐^�����獶������45�x�X�����ʒu
	x = (100.0 * 2.0 * PI) / 4.0;

	// �����x�͂O
	speed = 0.0;

}

void DebugScene::Update() {

	// �����ɏ]���đ��x�����Z
	// MASS�̒l������������Ƃ�����蓮��
	speed += -MASS * G * sin(x / 100.0);

	// ���x�ɏ]���ĉ~��̍��W��ύX
	x += speed;

	// �������_�Ƃ����ꍇ�̂Ԃ牺�����Ă��镨�̍��W���Z�o
	// ���̂܂܂��Ɓ|45�`45�x�̐U��q�ɂȂ�̂�
	// ���v����90�x�iPI/2.0�j��]
	Angle = x / 200.0 + PI / 2.0;

	// ���߂��p�x���玲�����_�Ƃ����~����̍��W���擾
	nx = static_cast<float>(cos(Angle) * 100.0);
	ny = static_cast<float>(sin(Angle) * 100.0);


	worldTransform_.translation_ = Vector3(jiku_x + nx, -(jiku_y + ny), jiku_x + nx);

	worldTransform_.TransferMatrix();

	viewProjection_->target.y = -250.0f;
	viewProjection_->UpdateMatrix();


	ImGui::Begin("XXXXX");
	ImGui::Text("x = %lf", x);

	ImGui::End();

	if (shadeNumber == 0) {
		ImGui::Begin("Not");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::End();
	}
	else if (shadeNumber == 1) {
		ImGui::Begin("averageBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SliderInt("range", &range, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 2) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SliderFloat("centerX", &center.x, 0, 1);
		ImGui::SliderFloat("centerY", &center.y, 0, 1);
		ImGui::SliderFloat("intensity", &intensity, 0, 1);
		ImGui::SliderInt("samples", &samples, 0, 20);
		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 3) {
		ImGui::Begin("RadialBlurBlur");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}
	else if (shadeNumber == 4) {
		ImGui::Begin("CloseFilta");
		ImGui::SliderInt("shadeNumber", &shadeNumber, 0, 4);
		ImGui::SliderFloat("angle", &angle, 0.0f, 180.0f);
		ImGui::SliderFloat("angle2", &angle2, 0.0f, 180.0f);

		ImGui::SetCursorPos(ImVec2(0, 20));
		ImGui::End();
	}

	if (input_->PushKey(DIK_W)) {
		CameraPos += {0, 0, 0.5f};
	}
	if (input_->PushKey(DIK_S)) {
		CameraPos -= {0, 0, 0.5f};
	}
	if (input_->PushKey(DIK_D)) {
		CameraPos += {0.5f, 0, 0};
	}
	if (input_->PushKey(DIK_A)) {
		CameraPos -= {0.5f, 0, 0};
	}

}

void DebugScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	Model::PreDraw(commandList);


	Model::PostDraw();

	////�p�[�e�B�N��
	Explosion::PreDraw(commandList);

	Explosion::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();

	Hibana::PreDraw(commandList);
	
	Hibana::PostDraw();

	PostEffect::PostDrawScene();
}

void DebugScene::CSUpdate()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	

}

void DebugScene::Draw() {

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �w�i�X�v���C�g�`��

	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();


#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	ParticleCS::PreDraw(commandList);



	ParticleCS::PostDraw();

	//// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);
	//ground->Draw(*viewProjection_);
	model->Draw(worldTransform_, *viewProjection_.get());
	
	//3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();


	ParticleHandHanabi::PreDraw(commandList);

	ParticleHandHanabi::PostDraw();

	Explosion::PreDraw(commandList);
	//ParticleMan->Draw(*viewProjection_.get());
	Explosion::PostDraw();

	Hibana::PreDraw(commandList);

	Hibana::PostDraw();

#pragma endregion

#pragma region �O�i�X�v���C�g�`��



#pragma endregion
}

void DebugScene::Finalize()
{
}

void DebugScene::CopyData()
{

}
