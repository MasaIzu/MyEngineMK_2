#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <Sprite.h>

class Player {

public://��{�֐�
	Player();
	~Player();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void DrawSprite(ViewProjection& viewProjection_);

private:
	//�v���[���[�̈ړ�
	void Move();
	//�v���C���[�̉�]
	void PlayerRot();
	//�v���[���[�̈ړ��̒l�X�V
	void WorldTransUpdate();

public://GetterSetter
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }

public:
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransForBullet;
	std::unique_ptr<PlayerBullet> playerBullet;
	// �Ə��X�v���C�g
	std::unique_ptr<Sprite> AttackSprite;

private://�ʃN���X����l�����炤
	Vector2 cameraRot;

private://�v���C���[�N���X�ϐ�
	uint32_t bulletNumber = 0;
	float playerSpeed = 0.5f;
	float diagonalPlayerSpeed = 0.6f;
	Vector3 playerMoveMent;//�ړ���

};