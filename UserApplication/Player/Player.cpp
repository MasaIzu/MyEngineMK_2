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
	//‚Ç‚¤“®‚­‚©
	Move();
	//ˆÚ“®‚Ì’lXV
	WorldTransUpdate();
}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(playerWorldTrans, viewProjection_);
}

void Player::Move()
{

	if (input_->PushKey(DIK_W)) {
		playerWorldTrans.translation_ += Vector3(0.0f, 0.0f, 0.1f);
	}
	if (input_->PushKey(DIK_S)) {
		playerWorldTrans.translation_ -= Vector3(0.0f, 0.0f, 0.1f);
	}
	if (input_->PushKey(DIK_A)) {
		playerWorldTrans.translation_ -= Vector3(0.1f, 0.0f, 0.0f);
	}
	if (input_->PushKey(DIK_D)) {
		playerWorldTrans.translation_ += Vector3(0.1f, 0.0f, 0.0f);
	}

}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
}

