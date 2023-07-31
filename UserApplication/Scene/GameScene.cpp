#include "GameScene.h"
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

GameScene::GameScene() {}
GameScene::~GameScene() {
	collisionManager->AllClearCollider();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();


	sceneManager_ = SceneManager::GetInstance();

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 50,0,0 };
	worldTransform_.scale_ = { 10.0f,3.0f,4.0f };
	worldTransform_.TransferMatrix();

	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();
	ParticleMan->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, -210.0f, -283.0f), viewProjection_.get());

	gameCamera = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height);
	gameCamera->Initialize(viewProjection_.get(), MyMath::GetAngle(180.0f), player_->GetPlayerPos());

	model_.reset(Model::CreateFromOBJ("cube", true));

	ground = std::make_unique<Ground>(model_.get());
	ground->Initialze();

	//tutorialEnemy = std::make_unique<TutorialEnemy>(Vector3(0, 10, 0));
	//tutorialEnemy->Initialize();

	levelData = std::make_unique<LoadLevelEditor>();
	levelData->Initialize("tutorialStage");

	player_->SetFirstMoveSpline(levelData->GetFirstSpline());
	player_->SetSpline(levelData->GetSpline());
	player_->SetFinalSpline(levelData->GetFinalSpline());

	tutorialEnemyList = levelData->GetTutorialEnemyList();
	bulletShotEnemy = levelData->GetBulletShotEnemyList();

	for (TutorialEnemy* enemy : tutorialEnemyList) {
		enemy->Initialize();
	}
	for (BulletShotEnemy* enemy : bulletShotEnemy) {
		enemy->Initialize();
	}

	touchableObject.reset(TouchableObject::Create(model_.get(), worldTransform_, COLLISION_ATTR_LANDSHAPE));

	collisionManager = CollisionManager::GetInstance();

	gameCamera->SetFreeCamera(true);
	//Vector3 target = levelData->GetFirstSpline()[1] + Vector3(10, -5, 0);
	eye = levelData->GetFirstSpline()[1] + Vector3(-50, 0, 0);
	Vector3 target = levelData->GetFirstSpline()[1] + Vector3(0, 10, 0);
	gameCamera->SetCameraTargetAndPos(target, eye);
}

void GameScene::Update() {

	CheckReticle();
	
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

	if (input_->PushKey(DIK_0)) {
		ParticleMan->Add(Pos, Verocty, MaxFream);
	}

	ParticleMan->Update();

	player_->SetCameraRot(gameCamera->GetCameraAngle());
	player_->SetEyeToTagetVecDistance(gameCamera->GetEyeToTagetVecDistance(120.0f));
	player_->SetCameraDistance(gameCamera->GetCameraDistanse());
	player_->SetCameraMaxDistance(gameCamera->GetMaxDistance());
	player_->Update();

	if (isSpline == false) {
		if (player_->GetHowReturnSpline(2)) {
			larpTime = 0.0f;
			isSpline = true;
		}
	}
	else {
		if (player_->GetHowReturnSpline(3)) {

			Vector2 Mous(0, -2);
			gameCamera->MousePositionReset(Mous, true);

		}
		if (player_->GetFinishFirstSpline()) {
			gameCamera->MousePositionReset();
			gameCamera->SetFreeCamera(false);
			player_->SetCameraModeNotFree(true);
			isSpline = false;
		}
		Vector3 larp;
		if (larpTime < 1.0f) {
			larpTime += 0.01f;
		}
		else {
			larpTime = 1.0f;
		}
		
		gameCamera->SetCameraTargetAndPos(larp.lerp(levelData->GetFirstSpline()[1] + Vector3(0, 10, 0), player_->GetPlayerPos() + Vector3(0,7,0), larpTime),
			larp.lerp(eye, gameCamera->GetPlayerDistanceEyePos(player_->GetPlayerPos()),larpTime));
	}
	if (player_->GetHowReturnFainalSpline(5)) {
		isFinish = true;
		sceneManager_->ChangeScene("STAGESELECT");
	}
	if (player_->GetPlayerPos().y < -250.0f) {
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	gameCamera->SetCameraMode(player_->GetHitFinalRail());
	gameCamera->SetPlayerPosition(player_->GetPlayerPos());
	gameCamera->Update();

	//tutorialEnemy->Update(player_->GetPlayerPos());
	//bulletShotEnemy->Update(player_->GetPlayerPos());

	for (TutorialEnemy* enemy : tutorialEnemyList) {
		enemy->Update(player_->GetPlayerPos());
	}
	for (BulletShotEnemy* enemy : bulletShotEnemy) {
		enemy->Update(player_->GetPlayerPos());
	}
	// 敵のデスフラグが立っていたらリストから消す
	tutorialEnemyList.remove_if([](TutorialEnemy* enemy) { return enemy->GetIsDead(); });
	bulletShotEnemy.remove_if([](BulletShotEnemy* enemy) { return enemy->GetIsDead(); });
	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void GameScene::PostEffectDraw()
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
	ParticleManager::PreDraw(commandList);
	ParticleMan->Draw(*viewProjection_.get());
	player_->ParticleDraw(*viewProjection_.get());
	ParticleManager::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();



	PostEffect::PostDrawScene();
}

void GameScene::CSUpdate()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	////パーティクル
	ParticleMan->CSUpdate(commandList);
	player_->CSUpdate(commandList);
	for (BulletShotEnemy* enemy : bulletShotEnemy) {
		enemy->CSUpdate(commandList);
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

#pragma region 3Dオブジェクト描画
	ParticleManager::PreDraw(commandList);



	ParticleManager::PostDraw();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	skydome->Draw(*viewProjection_.get());
	ground->Draw(*viewProjection_.get());
	model_->Draw(worldTransform_ ,*viewProjection_.get());
	levelData->Draw(*viewProjection_.get());
	//tutorialEnemy->Draw(*viewProjection_.get());
	for (TutorialEnemy* enemy : tutorialEnemyList) {
		enemy->Draw(*viewProjection_.get());
	}
	for (TutorialEnemy* enemy : tutorialEnemyList) {
		enemy->DebugDraw(*viewProjection_.get());
	}
	for (BulletShotEnemy* enemy : bulletShotEnemy) {
		enemy->Draw(*viewProjection_.get());
	}
	player_->Draw(*viewProjection_.get());
	//3Dオブジェクト描画後処理
	Model::PostDraw();

	player_->DrawSprite(*viewProjection_.get());

	//bulletShotEnemy->CSUpdate(commandList);

	ParticleManager::PreDraw(commandList);
	
	for (BulletShotEnemy* enemy : bulletShotEnemy) {
		enemy->ParticleDraw(*viewProjection_.get());
	}
	//bulletShotEnemy->ParticleDraw(*viewProjection_.get());
	
	//ParticleMan->Draw(*viewProjection_.get());
	//player_->ParticleDraw(*viewProjection_.get());

	ParticleManager::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void GameScene::Finalize()
{
}

void GameScene::CopyData()
{
	ParticleMan->CopyData();
	player_->CopyParticle();
	//bulletShotEnemy->CopyParticle();
	for (BulletShotEnemy* enemy : bulletShotEnemy) {
		enemy->CopyParticle();
	}
}

bool GameScene::CheckReticle()
{
	////Vector3 EnemyPos = tutorialEnemy->GetTutorialEnemyPos();

	//Vector2 windowWH = Vector2(WinApp::GetInstance()->GetWindowSize().x, WinApp::GetInstance()->GetWindowSize().y);

	////ビューポート行列
	//Matrix4 Viewport =
	//{ windowWH.x / 2,0,0,0,
	//0,-windowWH.y / 2,0,0,
	//0,0,1,0,
	//windowWH.x / 2, windowWH.y / 2,0,1 };

	////ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * Viewport;

	////ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	//this->EnemyPos = MyMath::DivVecMat(EnemyPos, matViewProjectionViewport);

	//dist = MyMath::Distance2Vec2(Vector2(this->EnemyPos.x, this->EnemyPos.y),Vector2(640, 360));
	//radius = 8.0f;
	//if (dist <= radius) {
	//	int a = 0;
	//}
	//else {
	//	
	//}

	return false;
}