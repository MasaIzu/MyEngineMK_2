#include "TitleScene.h"
#include <imgui.h>
#include"PostEffect.h"
#include <Explosion.h>


void TitleScene::Initialize()
{
	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	loserTexture_ = TextureManager::Load("sprite/ParticleDemo.png");
	sprite_=  Sprite::Create(loserTexture_);
	sceneManager_ = SceneManager::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-200 };
	viewProjection_->UpdateMatrix();

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), Vector3(0, 0, 0));
	gameCamera->SetFreeCamera(false);
	gameCamera->SetCameraMode(false);

	ParticleMan = std::make_unique<MeshParticle>();
	ParticleMan->Initialize(MaxParticleCount);
	ParticleMan->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	model.reset(Model::CreateFromOBJ("ken", true));
	//model1.reset(Model::CreateFromOBJ("Ground", true));

	ParticleMan->SetMeshPos(model->GetMeshData());

}

void TitleScene::Update()
{
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

	//if (input_->TriggerKey(DIK_SPACE))
	//{
	//	sceneManager_->ChangeScene("STAGESELECT");
	//}

	CameraPos.z = 5.0f;

	gameCamera->SetPlayerPosition(CameraPos);
	gameCamera->Update();
}

void TitleScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	Model::PreDraw(commandList);


	Model::PostDraw();

	////パーティクル
	Explosion::PreDraw(commandList);

	Explosion::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();

	MeshParticle::PreDraw(commandList);
	ParticleMan->Draw(*viewProjection_.get());
	MeshParticle::PostDraw();

	PostEffect::PostDrawScene();
}

void TitleScene::Draw()
{
	
	sprite_->Draw({640,360},{1,1,1,1});

	//Hibana::PreDraw(commandList);
	//ParticleMan->Draw(*viewProjection_.get());
	//Hibana::PostDraw();

}

void TitleScene::Finalize()
{
}

void TitleScene::CopyData()
{
}

void TitleScene::CSUpdate()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	ParticleMan->CSUpdate(commandList, Vector4(0, 7, 0, 0));
}
