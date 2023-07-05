#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include <BaseCollider.h>
#include <CollisionManager.h>

class TutorialEnemy {

public://��{�֐�
	TutorialEnemy(const Vector3& BonePos_);
	~TutorialEnemy();

	void Initialize();
	void Update(const Vector3& PlayerPos);
	void Draw(ViewProjection& viewProjection_);

private:
	//�v���C���[�̒e�������������̏���
	void PlayerBulletHit();

	//�G�����������̓����ƌ����ĂȂ����̓���
	void PlayerNotFoundMove();
	void PlayerFoundMove();
	//��̊֐��̃^�C�}�[
	void PlayerNotFoundMoveTimer();
	void PlayerFoundMoveTimer();
	//�v���C���[�̃T�[�`�֐�
	void SearchingPlayer();
	//�p�x���Ƃ�
	void GetPlayerForEnemyAngle();
	//�v���[���[�̈ړ��̒l�X�V
	void WorldTransUpdate();
	//�����_��
	uint32_t Random(const uint32_t& low, const uint32_t& high);

private://�N���X�ϐ�
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> modelDebug_;
	WorldTransform enemyWorldTrans;
	WorldTransform DebugWorldTrans;

	//�����蔻��
	BaseCollider* TutorialEnemyCollider = nullptr;
	CollisionManager* collisionManager = nullptr;

private:
	enum class NotFoundPhase {
		Walk,//����
		Stop,//�~�܂�
		Interruption,//���f
		ForcedWalking,//��������
		FoundPlayer,//�v���C���[��������
		Nothing,//�������Ȃ�
	};
	//�����ĂȂ��Ƃ��̓����t�F�[�Y
	NotFoundPhase NotFoundPhase_ = NotFoundPhase::Walk;

	enum class FoundPhase {
		Intimidation,//�Њd
		Walk,//����
		Stop,//�~�܂�
		Attack,//�U��
		Turn,//�v���C���[�̕�������
		Nothing,//�������Ȃ�
	};
	//���������̓����t�F�[�Y
	FoundPhase FoundPhase_ = FoundPhase::Nothing;

private://�O���玝���Ă������̂̊i�[
	Vector3 playerPos;

private:
	bool isPlayerFound = false;
	bool isAlive = false;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;
	uint32_t AttackWalkTime = 0;
	uint32_t AttackStopTime = 0;
	uint32_t TurnTimes = 60;

	float TerritoryRadius = 20.0f;
	float SearchingAreaRadius = 25.0f;
	float EnemySpeed = 0.12f;
	float Rot = 0.0f;
	float dist = 0.0f;
	float radius = 0.0f;
	float EnemyRadius = 1.0f;
	float dotPlayerAndEnemy = 0;
	float BackAngle = 180.0f;
	

	Vector3 enemyMoveMent;
	Vector3 BonePos;
	Vector3 tmp;
	

};