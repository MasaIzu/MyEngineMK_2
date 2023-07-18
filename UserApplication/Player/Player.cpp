#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
#include <FbxLoader.h>


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	input_ = Input::GetInstance();
	model_.reset(Model::CreateFromOBJ("sphere", true));
	playerWorldTrans.Initialize();
	playerWorldTrans.translation_ = { 0,10.0f,-50.0f };

	playerWorldTransForBullet.Initialize();

	playerBullet = std::make_unique<PlayerBullet>();
	playerBullet->Initialize();

	DebugWorldTrans.Initialize();

	AttackSprite = Sprite::Create(TextureManager::Load("shoujuun.png"));
	AttackSprite->SetAnchorPoint({ 0.5f,0.5f });

}

void Player::Update()
{

	//回転させる
	PlayerRot();
	if (playerBullet->GetExpandingBullet() == false) {
		//どう動くか
		Move();
	}
	//移動の値更新
	WorldTransUpdate();

	if (input_->MouseInputTrigger(0)) {
		//isPlayerSetUp = true;
		//PlayerAttack();
	}
	if (input_->MouseInputing(0)) {
		isPlayerSetUp = true;
		PlayerAttack();
	}
	if (input_->MouseInputTrigger(1)) {
		
	}


	ImGui::Begin("Player");

	ImGui::Text("Distance:%f,%f,%f", Distance.x, Distance.y, Distance.z);

	ImGui::End();


	DebugWorldTrans.translation_ = Distance;
	DebugWorldTrans.TransferMatrix();

	///playerBullet->UpdateWhileExpanding(GetPlayerPos(), DistanceNolm);
	playerBullet->Update();
}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(playerWorldTrans, viewProjection_);
	playerBullet->Draw(viewProjection_);
	model_->Draw(DebugWorldTrans, viewProjection_);
}

void Player::DrawSprite(ViewProjection& viewProjection_)
{
	AttackSprite->Draw(Vector2(640, 360), Vector4(1, 1, 1, 1), 2);
}

void Player::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	playerBullet->CSUpdate(cmdList);
}

void Player::ParticleDraw(ViewProjection& viewProjection_)
{
	playerBullet->ParticleDraw(viewProjection_);
}

void Player::CopyParticle()
{
	playerBullet->CopyParticle();
}

void Player::Move()
{
	playerMoveMent = { 0.0f,0.0f,0.0f };



	if (input_->PushKey(DIK_W)) {
		playerMoveMent += playerWorldTrans.LookVelocity.look * playerSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack * playerSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		playerMoveMent += playerWorldTrans.LookVelocity.lookLeft * playerSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		playerMoveMent += playerWorldTrans.LookVelocity.lookRight * playerSpeed;
	}

	if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_A) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.look_lookLeft * diagonalPlayerSpeed;
	}
	if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_D) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.look_lookRight * diagonalPlayerSpeed;
	}
	if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_A) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookLeft * diagonalPlayerSpeed;
	}
	if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_D) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookRight * diagonalPlayerSpeed;
	}

	playerWorldTrans.translation_ += playerMoveMent;

}

void Player::PlayerRot()
{

	playerWorldTrans.SetRot(Vector3(0.0f, cameraRot.x, 0.0f));
	playerWorldTransForBullet.SetRot(Vector3(cameraRot.y, cameraRot.x, 0.0f));
	//値更新
	WorldTransUpdate();
}

void Player::PlayerAttack()
{
	if (AttackPhase_ == AttackPhase::Nothing) {
		AttackPhase_ = AttackPhase::AttackCombo1;
	}
	switch (AttackPhase_)
	{
	case Player::AttackPhase::AttackCombo1:
		DistanceNolm = Distance - GetPlayerPos();
		PlayerToCameraDistance = DistanceNolm.length();
		DistanceNolm.normalize();
		bulletNumber = playerBullet->MakePlayerBullet(GetPlayerPos(), DistanceNolm, PlayerToCameraDistance);
		break;
	case Player::AttackPhase::AttackCombo2:

		break;
	case Player::AttackPhase::AttackCombo3:

		break;
	case Player::AttackPhase::AttackCombo4:

		break;
	case Player::AttackPhase::AttackUlt:

		break;
	case Player::AttackPhase::Nothing:

		break;
	default:
		break;
	}
}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
	playerWorldTransForBullet.TransferMatrix();
}

