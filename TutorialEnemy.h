#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>

class TutorialEnemy {

public://��{�֐�
	TutorialEnemy(const Vector3& BonePos_);
	~TutorialEnemy();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	//�G�����������̓����ƌ����ĂȂ����̓���
	void PlayerFoundMove();
	void PlayerNotFoundMove();
	//��̊֐��̃^�C�}�[
	void PlayerFoundMoveTimer();
	void PlayerNotFoundMoveTimer();

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
private:
	enum class NotFoundPhase {
		Walk,//����
		Stop,//�~�܂�
		Interruption,//���f
		ForcedWalking,//��������
	};
	//�����ĂȂ��Ƃ��̓����t�F�[�Y
	NotFoundPhase NotFoundPhase_ = NotFoundPhase::Walk;

	enum class FoundPhase {
		Walk,//����
		Stop,//�~�܂�
		Attack,//�U��
	};
	//���������̓����t�F�[�Y
	FoundPhase FoundPhase_ = FoundPhase::Walk;

private:
	bool isPlayerFound = 0;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;
	uint32_t AttackWalkTime = 0;
	uint32_t AttackStopTime = 0;

	float TerritoryRadius = 20.0f;
	float EnemySpeed = 0.12f;
	float Rot = 0.0f;
	float dist = 0.0f;
	float radius2 = 0.0f;

	Vector3 enemyMoveMent;
	Vector3 BonePos;
	Vector3 tmp;
	

};