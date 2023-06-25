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
}

void Player::Update()
{
	//回転させる
	PlayerRot();
	//どう動くか
	Move();
	//移動の値更新
	WorldTransUpdate();
}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(playerWorldTrans, viewProjection_);
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
		playerMoveMent += playerWorldTrans.LookVelocity.look_lookLeft * playerSpeed;
	}
	if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_D) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.look_lookRight * playerSpeed;
	}
	if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_A) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookLeft * playerSpeed;
	}
	if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_D) == 1) {
		playerMoveMent = { 0.0f,0.0f,0.0f };
		playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookRight * playerSpeed;
	}


	playerWorldTrans.translation_ += playerMoveMent;

}

void Player::PlayerRot()
{

	playerWorldTrans.SetRot(Vector3(0.0f, cameraRot, 0.0f));

	//値更新
	WorldTransUpdate();
}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
}

