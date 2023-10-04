#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"
#include "MultiBullet.h"
#include "Player.h"
#include "MissileBullet.h"

class MiddleBossEnemy {
private:
	enum class AttackType
	{
		Nomal,//4�o���}�L
		Missile,//�~�T�C��
		MoveingAttack,//�ړ����Ȃ���
		Move,//�ړ�
		NotAttack,//�U�����Ȃ�
	};

public://��{�֐�
	MiddleBossEnemy();
	~MiddleBossEnemy();

	void Initialize(Player* player);
	void Update();
	void Draw(ViewProjection& viewProjection_);

	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);

private://�֐�
	void Timer();
	void Attack();
	void ThinkingTime();
	void WorldTransUpdate();//�ړ��̒l�X�V
	void CheckAttackType();
	uint32_t RandomType(uint32_t& NoUseType);

public://Setter

	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter

	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }

	bool GetIsDead()const { return isDead; }

	Vector3 GetPosition() const;

private://const�֘A
	static const uint32_t BossEnemyBodyCount = 20;

	static const uint32_t AttackedKeepCount = 8;

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans;

	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;

	//�X�v���C��
	std::unique_ptr<SplinePosition> MoveSpline;//�r���̃X�v���C��

	//�e
	std::unique_ptr<MultiBullet> multiBullet;
	std::unique_ptr<MissileBullet> missileBullet;

	// �R���C�_�[
	BaseCollider* MiddleBossCollider = nullptr;

private://�C�[�i���N���X


private://�ʃN���X����l�����炤
	Player* player = nullptr;

private://EnemyBoss�N���X�ϐ�

	bool isStart = false;
	bool isTurn = false;
	bool isSporn = false;
	bool isDead = false;
	bool isAttack = false;
	bool isMoveing = false;
	bool isAngleGet = false;
	bool isOneMoreTime = false;
	bool isBackSponePos = false;

	uint32_t BulletCoolTime = 0;
	uint32_t MoveingTimer = 0;
	uint32_t MaxMoveingTimer = 0;
	uint32_t MaxBulletCoolTime = 0;
	uint32_t MiddleBossHp = 20;
	uint32_t AttackCooltime = 0;
	uint32_t AttackTypeCount = static_cast<uint32_t>(AttackType::Move);
	uint32_t AllAttackTypeCount = static_cast<uint32_t>(AttackType::NotAttack) + 1;
	uint32_t KeepAttackingTime = 0;
	uint32_t RotTime = 0;
	uint32_t mveType = 0;
	uint32_t MoveTimes = 0;
	uint32_t MaxMoveTimes = 5;
	uint32_t BackLengthHalfUint32 = 0;
	uint32_t BackLengthUint32 = 0;

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;
	float Radius = 10.0f;
	float Angle = 0.0f;
	float AngleSize = 0.0f;
	float RotSpeed = 1.0f;
	float BulletSpeed = 6.0f;
	float MoveSafeRadius = 80.0f;
	float BackLength = 0.0f;
	float JumpMoveSpeed = 1.0f;
	float JumpHeight = 0.0f;
	float MaxJumpHeight = 15.0f;

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;
	Vector3 MovePos;
	Vector3 MoveStartPos;
	Vector3 JampBackVelocity;

	AttackType attackType = AttackType::NotAttack;
	AttackType oldAttackType[AttackedKeepCount];

};