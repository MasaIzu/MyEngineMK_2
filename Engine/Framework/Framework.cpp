#include "Framework.h"
#include"ParticleManager.h"
#include"PostEffect.h"

void Framework::Initialize()
{

	// ゲームウィンドウの作成
	winApp_ = WinApp::GetInstance();
	winApp_->MakeWindow(L"Maverick");

	// DirectX初期化処理
	directXCore_ = DirectXCore::GetInstance();
	directXCore_->DirectXCoreInitialize(winApp_->Gethwnd(), winApp_->window_width, winApp_->window_height);

#pragma region 汎用機能初期化
	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize();


	// テクスチャマネージャの初期化
	TextureManager_ = TextureManager::GetInstance();
	TextureManager_->Initialize(directXCore_->GetDevice());
	TextureManager::Load("white1x1.png");

	// FBX関連静的初期化
	fbxLoader_ = FbxLoader::GetInstance();
	fbxLoader_->Initialize(directXCore_->GetDevice());

	// スプライト静的初期化
	Sprite::StaticInitialize(directXCore_->GetDevice());


	// 3Dモデル静的初期化
	Model::StaticInitialize();
	FbxModel::StaticInitialize();

	//Imgui初期化
	imGui = std::make_unique <ImGuiManager>();
	imGui->Initialize(winApp_, directXCore_);

	fps = std::make_unique<FPS>();

	ParticleManager::StaticInitialize(DirectXCore::GetInstance()->GetDevice());

	PostEffect::Initialize(DirectXCore::GetInstance());

#pragma endregion

	//シーンマネージャーの生成
	sceneManager_ = SceneManager::GetInstance();


}

void Framework::Update()
{

	//fps制限
	fps->FpsControlBegin();

	// メッセージ処理
	if (winApp_->ProcessMessage()) {
		isEndRequst = true;
	}

	// 入力関連の毎フレーム処理
	input_->Update();

	//Imguiの更新
	imGui->Bigin();

	//デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();

	//シーンマネージャーの更新
	sceneManager_->Update();

	//Imguiのコマンド受付終了
	imGui->End();
}



void Framework::Finalize()
{
	// 各種解放
	sceneManager_->Finalize();

	PostEffect::Finalize();

	ParticleManager::StaticFinalize();

	imGui->Finalize();
	sceneFactory_.reset();

	FbxModel::StaticFainalize();
	Model::StaticFinalize();

	Sprite::StaticFinalize();

	fbxLoader_->Finalize();

	TextureManager_->Delete();

	input_->Destroy();

	directXCore_->Destroy();

	// ゲームウィンドウの破棄
	winApp_->DeleteGameWindow();
	winApp_->Destroy();


}

bool Framework::isPlayMyGame()
{
	return isEndRequst;
}

void Framework::Run()
{

	//ゲームの初期化
	Initialize();

	while (true) {//ゲームループ

		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (isPlayMyGame()) {
			break;
		}

		PostEffectDraw();
		// 描画開始
		directXCore_->PreDraw();

		Draw();
		PostEffect::Draw(directXCore_->GetCommandList());

		//ImGui描画
		imGui->Draw();

		

		// 描画終了
		directXCore_->PostDraw();

		//FPS固定
		fps->FpsControlEnd();

		if (input_->TriggerKey(DIK_ESCAPE)) {
			break;
		}

	}

	//ゲームの終了
	Finalize();


}
