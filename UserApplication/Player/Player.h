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

	//�p�[�e�B�N�����o���p
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

private:
	//�v���[���[�̈ړ�
	void Move();
	//�v���C���[�̉�]
	void PlayerRot();
	//�v���[���[�̍U��
	void PlayerAttack();
	//�v���[���[�̈ړ��̒l�X�V
	void WorldTransUpdate();

public://Setter
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }
	void SetCameraDistance(const Vector3& cameradis) { Distance = cameradis; }
public://Getter
	bool GetIsPlayerSetUp()const { return isPlayerSetUp; }
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }
	WorldTarnsLook GetPlayerLook()const { return playerWorldTrans.LookVelocity; }

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransForBullet;
	WorldTransform DebugWorldTrans;
	std::unique_ptr<PlayerBullet> playerBullet;
	// �Ə��X�v���C�g
	std::unique_ptr<Sprite> AttackSprite;

private://�C�[�i���N���X
	enum class AttackPhase {
		AttackCombo1,//
		AttackCombo2,//
		AttackCombo3,//
		AttackCombo4,//
		AttackUlt,//
		Nothing,//�������Ȃ�
	};
	//�����ĂȂ��Ƃ��̓����t�F�[�Y
	AttackPhase AttackPhase_ = AttackPhase::Nothing;

private://�ʃN���X����l�����炤
	Vector2 cameraRot;

private://�v���C���[�N���X�ϐ�
	bool isPlayerSetUp = false;

	uint32_t bulletNumber = 0;

	float playerSpeed = 0.3f;
	float diagonalPlayerSpeed = 0.4f;
	float PlayerToCameraDistance = 0.0f;

	Vector3 playerMoveMent;//�ړ���
	Vector3 ReticlePos;
	Vector3 Distance;
	Vector3 DistanceNolm;
};