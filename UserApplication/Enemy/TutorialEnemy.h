#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include <BaseCollider.h>
#include <CollisionManager.h>
#include "SplinePosition.h"

class TutorialEnemy {

public://��{�֐�
	TutorialEnemy(const Vector3& BonePos_);
	~TutorialEnemy();

	void Initialize();
	void Update(const Vector3& PlayerPos);
	void Draw(ViewProjection& viewProjection_);

public://getter

	Vector3 GetTutorialEnemyPos() const { return MyMath::GetWorldTransform(enemyWorldTrans.matWorld_); };
private:
	//�����ɂ��Ă̊֐�
	void PlayerBulletHit();//�v���C���[�̒e�������������̏���
	void PlayerNotSpottedMove();//�G�������ĂȂ����̓���
	void PlayerSpottedMove();//�G�����������̓���
	void Attack();

	//�G�̍s���ɂ��Ă̊֐��^�C�}�[
	void PlayerNotSpottedMoveTimer();//�G�������ĂȂ���
	void PlayerSpottedMoveTimer();//�G����������

	//�G���n�֐�
	bool GetIsAttackArea();//�U���ł���͈͂��ǂ���
	void SearchingPlayer();//�v���C���[�̃T�[�`�֐�
	void GetPlayerForEnemyAngle();//�p�x���Ƃ�
	void WorldTransUpdate();//Enemy�����̈ړ��̒l�X�V
	uint32_t Random(const uint32_t& low, const uint32_t& high);//�����_��


private://const�֘A
	static const uint32_t AttackSphereCount = 3;

private://�N���X�ϐ�
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> modelDebug_;
	WorldTransform enemyWorldTrans;
	WorldTransform AttackWorldTrans[AttackSphereCount];
	WorldTransform DebugWorldTrans;

	//�����蔻��
	BaseCollider* TutorialEnemyCollider = nullptr;
	BaseCollider* TutorialEnemyAttackSpereCollider[AttackSphereCount];
	CollisionManager* collisionManager = nullptr;

	//�X�v���C���Ȑ�
	std::unique_ptr<SplinePosition> splinePosition[AttackSphereCount];

private://�C�[�i��
	enum class NotSpottedPhase {
		Walk,//����
		Stop,//�~�܂�
		Interruption,//���f
		ForcedWalking,//��������
		SpottedPlayer,//�v���C���[��������
		Nothing,//�������Ȃ�
	};
	//�����ĂȂ��Ƃ��̓����t�F�[�Y
	NotSpottedPhase NotSpottedPhase_ = NotSpottedPhase::Walk;

	enum class SpottedPhase {
		Intimidation,//�Њd
		Walk,//����
		Stop,//�~�܂�
		Attack,//�U��
		Wait,//�҂�
		LoseSightofPlayer,//�v���C���[�������Ȃ�����
		Turn,//�v���C���[�̕�������
		Nothing,//�������Ȃ�
	};
	//���������̓����t�F�[�Y
	SpottedPhase SpottedPhase_ = SpottedPhase::Nothing;

	enum class AttackPhase {
		NormalAttack,//���ʂ̍U��
		RunAttack,//�����čU��
		Nothing,//�������Ȃ�
	};
	//���������̓����t�F�[�Y
	AttackPhase AttackPhase_ = AttackPhase::Nothing;

private://�O���玝���Ă������̂̊i�[
	Vector3 playerPos;

private:
	bool IsPlayerSpotted = false;
	bool IsAlive = false;
	bool IsAttack = false;
	bool IsEnemyHasADestination = false;
	bool IsNeverSpotted = true;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;
	uint32_t AttackWalkTime = 0;
	uint32_t AttackStopTime = 0;
	uint32_t TurnTimes = 60;
	uint32_t RunAttackTime = 60;
	uint32_t AttackDelayTime = 0;
	uint32_t MaxAttackDelayTime = 120;

	float TerritoryRadius = 20.0f;
	float AttackAreaRadius = 5.0f;
	float SearchingAreaRadius = 25.0f;
	float SpottedLookingPlayerRadius = 60.0f;
	float EnemySpeed = 0.09f;
	float SpottedEnemySpeed = 0.1f;
	float EnemyRadius = 1.0f;
	float EnemyToPlayerAngle = 0;
	float BackAngle = 180.0f;
	float HowAboutFarAway = 0.0f;
	float RunAttackSpeed = 0.4f;
	float LittleFar = 5.0f;
	float Distance = 0.0f;
	//���������悤
	float Rot = 0.0f;
	float dist = 0.0f;
	float radius = 0.0f;
	float NormalAttackSpeed = 0.02f;

	Vector3 enemyMoveMent;
	Vector3 BonePos;
	Vector3 tmp;
	Vector3 EnemyToPlayerVec;
	Vector3 BackBonePos;
	Vector3 DestinationPos;

	//�X�v���C���Ȑ��ɕK�v�ȃ����o�ϐ�
	Vector3 start = { 0.0f, 0.0f, 0.0f };	//�X�^�[�g�n�_
	Vector3 p1 = { 0.0f, 0.0f, 0.0f };		//����_����1
	Vector3 p2 = { 0.0f, 0.0f, 0.0f };		//����_����2
	Vector3 p3 = { 0.0f, 0.0f, 0.0f };		//����_����3
	Vector3 end = { 0.0f, 0.0f, 0.0f };		//�S�[���n�_
	//std::vector<Vector3> points{ start, start, p1, p2, end, end };���{

};