#include "Framework.h"
#include "MixPostEffect.h"
#include <PostEffectManager.h>
#include <ParticleManager.h>

void Framework::Initialize()
{

	// �Q�[���E�B���h�E�̍쐬
	winApp_ = WinApp::GetInstance();
	winApp_->MakeWindow(L"Shooting Star");

	// DirectX����������
	directXCore_ = DirectXCore::GetInstance();
	directXCore_->DirectXCoreInitialize(winApp_->Gethwnd(), winApp_->GetWindowSize().x, winApp_->GetWindowSize().y);

#pragma region �ėp�@�\������
	// ���͂̏�����
	input_ = Input::GetInstance();
	input_->Initialize();


	// �e�N�X�`���}�l�[�W���̏�����
	TextureManager_ = TextureManager::GetInstance();
	TextureManager_->Initialize(directXCore_->GetDevice());
	TextureManager::Load("white1x1.png");

	//// FBX�֘A�ÓI������
	//fbxLoader_ = FbxLoader::GetInstance();
	//fbxLoader_->Initialize(directXCore_->GetDevice());

	// �X�v���C�g�ÓI������
	Sprite::StaticInitialize(directXCore_->GetDevice());


	// 3D���f���ÓI������
	Model::StaticInitialize();
	/*FbxModel::StaticInitialize();*/

	//Imgui������
	imGui = std::make_unique <ImGuiManager>();
	imGui->Initialize(winApp_, directXCore_);

	fps = std::make_unique<FPS>();

	ParticleManager::Initialize(DirectXCore::GetInstance()->GetDevice());

	//�|�X�g�G�t�F�N�g�̏���������
	PostEffectManager::Initialize(DirectXCore::GetInstance(), winApp_->window_width, winApp_->window_height);

#pragma endregion

	//�V�[���}�l�[�W���[�̐���
	sceneManager_ = SceneManager::GetInstance();


}

void Framework::Update()
{

	//fps����
	fps->FpsControlBegin();

	// ���b�Z�[�W����
	if (winApp_->ProcessMessage()) {
		isEndRequst = true;
	}

	// ���͊֘A�̖��t���[������
	input_->Update();

	//Imgui�̍X�V
	imGui->Bigin();

	//�f���E�B���h�E�̕\���I��
	//ImGui::ShowDemoWindow();

	//�V�[���}�l�[�W���[�̍X�V
	sceneManager_->Update();

	//Imgui�̃R�}���h��t�I��
	imGui->End();
}

void Framework::Finalize()
{
	// �e����
	sceneManager_->Finalize();

	PostEffectManager::Finalize();

	ParticleManager::Finalize();

	imGui->Finalize();
	sceneFactory_.reset();

	/*FbxModel::StaticFainalize();*/
	Model::StaticFinalize();

	Sprite::StaticFinalize();

	/*fbxLoader_->Finalize();*/

	TextureManager_->Delete();

	input_->Destroy();

	directXCore_->Destroy();

	// �Q�[���E�B���h�E�̔j��
	winApp_->DeleteGameWindow();
	winApp_->Destroy();


}

bool Framework::isPlayMyGame()
{
	return isEndRequst;
}

void Framework::Run()
{

	//�Q�[���̏�����
	Initialize();

	while (true) {//�Q�[�����[�v

		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (isPlayMyGame()) {
			break;
		}

		CSUpdate();

		PostEffectDraw();

		PostEffectManager::PreDrawScene(directXCore_->GetCommandList());

		Draw();
		PostEffect::Draw(directXCore_->GetCommandList());

		PostEffectManager::PostDrawScene();

		// �`��J�n
		directXCore_->PreDraw();

		PostEffectManager::Draw(directXCore_->GetCommandList());

		//ImGui�`��
		imGui->Draw();

		

		// �`��I��
		directXCore_->PostDraw();

		CopyData();

		//FPS�Œ�
		fps->FpsControlEnd();

		if (input_->TriggerKey(DIK_ESCAPE)) {
			break;
		}

	}

	//�Q�[���̏I��
	Finalize();


}
