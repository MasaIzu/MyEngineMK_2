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
#include "SplinePosition.h"

class Player {

public://��{�֐�
	Player();
	~Player();

	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
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
	// �v���C���[�̓����蔻��
	void CheckPlayerCollider();
	//����
	void Fall();
	//�X�v���C���A�b�v�f�[�g
	void SplineUpdate();
	//���e�B�N���A�b�v�f�[�g
	void UpdateReticle();

public://Setter
	void SetCameraModeNotFree(const bool& mode) { isCameraModeNotFree = mode; }
	void SetCameraRot(const Vector2& CameraRot) { cameraRot = CameraRot; }
	void SetEyeToTagetVecDistance(const Vector3& cameradis) { Distance = cameradis; }
	void SetCameraDistance(const float& Distance) { PlayerToCameraDistance = Distance; }
	void SetCameraMaxDistance(const float& cameraMaxDistance) { this->cameraMaxDistance = cameraMaxDistance; }
	void SetFirstMoveSpline(const std::vector<Vector3>& points) { FirstMoveSpline->SetNotSplineVector(points); }
	void SetSpline(const std::vector<Vector3>& points) { playerMoveSpline->SetNotSplineVector(points); }
	void SetFinalSpline(const std::vector<Vector3>& points) { FinalMoveSpline->SetNotSplineVector(points); }
public://Getter
	bool GetIsPlayerSetUp()const { return isPlayerSetUp; }
	bool GetHitFirstRail()const { return isHitFirstRail; }
	bool GetHit2ndRail()const { return isHitRail; }
	bool GetHitFinalRail()const { return isHitFinalRail; }
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return FirstMoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetHowReturnSpline2ndRail(const uint32_t & HowIndex)const { return playerMoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetHowReturnFainalSpline(const uint32_t& HowIndex)const { return FinalMoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishFirstSpline()const { return FirstMoveSpline->GetFinishSpline(); }
	Vector3 GetPlayerPos()const { return MyMath::GetWorldTransform(playerWorldTrans.matWorld_); }
	WorldTarnsLook GetPlayerLook()const { return playerWorldTrans.LookVelocity; }

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform playerWorldTrans;
	WorldTransform playerWorldTransHed;
	WorldTransform playerWorldTransForBullet;
	WorldTransform StartingPointOfGrapple;
	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;
	std::unique_ptr<PlayerBullet> playerBullet;
	// �Ə��X�v���C�g
	std::unique_ptr<Sprite> AttackSprite;
	//WASD�X�v���C�g
	std::unique_ptr<Sprite> W_FontSprite[2];
	std::unique_ptr<Sprite> A_FontSprite[2];
	std::unique_ptr<Sprite> S_FontSprite[2];
	std::unique_ptr<Sprite> D_FontSprite[2];
	std::unique_ptr<Sprite> AttackFontSprite[2];
	// �R���C�_�[
	BaseCollider* PlayerCollider = nullptr;
	//�X�v���C��
	std::unique_ptr<SplinePosition> FirstMoveSpline;//�ŏ��̃J�����X�v���C��
	std::unique_ptr<SplinePosition> playerMoveSpline;//�r���̃X�v���C��
	std::unique_ptr<SplinePosition> FinalMoveSpline;//�Ō�̃J�����X�v���C��

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
	bool onGround = false;
	bool isHitRail = false;
	bool isHitFirstRail = false;
	bool isHitFinalRail = false;
	bool isCameraModeNotFree = false;
	bool isAttack = false;
	bool Stop = false;
	bool isPressing = false;
	bool isGrapple = false;

	uint32_t bulletNumber = 0;

	float Radius = 1.0f;
	float playerSpeed = 0.6f;
	float diagonalPlayerSpeed = 0.7f;
	float PlayerToCameraDistance = 0.0f;
	float PlayerToCameraTargetVecDistance = 0.0f;
	float cameraMaxDistance = 0.0f;
	float alpha = 0.0f;
	float PlayerToAimSaiteVecDistance = 0.0f;
	float GrappleSpeed = 0.0f;

	float pendulumLength = 0.0f;
	const float timeStep = 0.1f;

	Vector3 playerMoveMent;//�ړ���
	Vector3 Distance;
	Vector3 DistanceNolm;
	Vector3 ReticlePos;
	Vector3 ShootVec;
	Vector3 PlayerToAimSaiteVec;
	//�U��q�p�x
	Vector3 angle;
	Vector3 angularVelocity;

	Vector4 fallVec;
};