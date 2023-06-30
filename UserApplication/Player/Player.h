#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"

class Player {

public://��{�֐�
	Player();
	~Player();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);
	
private:
	//�v���[���[�̈ړ�
	void Move();
	//�v���C���[�̉�]
	void PlayerRot();
	//�v���[���[�̈ړ��̒l�X�V
	void WorldTransUpdate();

public://GetterSetter
	void SetCameraRot(const float& CameraRot) { cameraRot = CameraRot; }

public:
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	std::unique_ptr<PlayerBullet> playerBullet;

private://�v���C���[�N���X�ϐ�
	float playerSpeed = 1.0f;
	Vector3 playerMoveMent;//�ړ���


private://�ʃN���X����l�����炤
	float cameraRot;

};