#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"

class MiddleBossEnemy {

public://��{�֐�
	MiddleBossEnemy();
	~MiddleBossEnemy();

	void Initialize(const Vector3& Pos);
	void Update();
	void Draw(ViewProjection& viewProjection_);

	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);

private://�֐�

	void WorldTransUpdate();//�ړ��̒l�X�V

public://Setter

	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter

	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }

private://const�֘A
	static const uint32_t BossEnemyBodyCount = 20;

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans;

	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;

	//�X�v���C��
	std::unique_ptr<SplinePosition> MoveSpline;//�r���̃X�v���C��

private://�C�[�i���N���X


private://�ʃN���X����l�����炤


private://EnemyBoss�N���X�ϐ�

	bool isStart = false;
	bool isSporn = false;

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;

};