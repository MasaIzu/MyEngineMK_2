#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

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
private:
	std::unique_ptr<Model> model_;
	WorldTransform enemyWorldTrans;
private:
	enum class NotFoundPhase {
		Walk,//����
		Stop,//�~�܂�
	};

	//�t�F�[�Y
	NotFoundPhase NotFoundPhase_ = NotFoundPhase::Walk;
private:
	bool isPlayerFound = 0;

	uint32_t WalkTime = 0;
	uint32_t StopTime = 0;

	float TerritoryRadius = 5.0f;

	Vector3 enemyMoveMent;
	Vector3 BonePos;

	

};