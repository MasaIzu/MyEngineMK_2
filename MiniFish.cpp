#include "MiniFish.h"
#include"ImGuiManager.h"
#include <CollisionManager.h>
#include <CollisionAttribute.h>

void MiniFish::LeaveGayser(Vector3 gayserPos)
{
	//�ړ��̃C�[�W���O�J�n
	easeMove.Start(120);
	easeMove.Update();
	//�x�W�G�Ȑ���ԗp�̍��W4��ݒ�
	positions[0] = world.translation_;
	positions[1] = { Random(-10.0f,10.0f),15.0f,Random(-10.0f,10.0f) };
	positions[2] = positions[1];
	positions[3] = gayserPos;
	positions[1] += positions[0];
	positions[2] += positions[3];
}

void MiniFish::Initialize(const Vector3& pos, unsigned short attribute)
{
	//���[���h�ϊ��̏�����
	world.Initialize();
	//���W�������Őݒ�
	positions[3] = pos;
	world.translation_ = pos;
	world.TransferMatrix();

	isAlive = true;

	// �R���W�����}�l�[�W���ɒǉ�
	collider = new SphereCollider(Vector4(0, fishRadius, 0, 0), fishRadius);
	CollisionManager::GetInstance()->AddCollider(collider);

	collider->Update(world.matWorld_);
	collider->SetAttributeWakeEnemy(COLLISION_ATTR_WEAKENEMYS);
	collider->SetAttribute(attribute);

	//���̃��f��������
	//bodyModel.reset(Model::CreateFromOBJ("FishBody", true));
	//eyeModel.reset(Model::CreateFromOBJ("FishMedama", true));
}

static int count = 0;
void MiniFish::Update(const Vector3& stagePos, float stageRadius)
{

	//�C�[�W���O�X�V
	easeMove.Update();

	Vector3 vecStageToPos = world.translation_ - stagePos;

	////�C�[�W���O���I����Ă���
	//if (!easeMove.GetActive() || vecStageToPos.length() >= stageRadius) {
	//	SetMovePos(stagePos, stageRadius);
	//}




	ImGui::Text("timeRate:%f", easeMove.GetTimeRate());

	//if (easeMove.GetActive()) {
	//	ImGui::Text("active!");
	//	for (int i = 0; i < 4; i++) {
	//		ImGui::Text("pos[%d]:%f,%f,%f", i, positions[i].x, positions[i].y, positions[i].z);
	//	}

	//}


	if (speedResetCount == 0) {
		moveRad += Random(-15, 15);

		move.x = sin(moveRad * PI / 180.0f) * spdtemp;
		move.z = cos(moveRad * PI / 180.0f) * spdtemp;

		speedResetCount = spdResetInterval;
	}

	Vector3 pos;

	//pos = world.translation_;
	Vector3 nextPos = world.translation_ + move;
	if (nextPos.length() >= stageRadius) {
	/*	if (fabs(nextPos.x) > fabs(nextPos.z)) {
			move.x = -move.x;
		}
		else {
			move.z = -move.z;
		}*/
		//move = -move;
		moveRad += 180;
		spdtemp = Random(0.5f, 2.0f);
		move.x = sin(moveRad * PI / 180.0f) * spdtemp;
		move.z = cos(moveRad * PI / 180.0f) * spdtemp;
		speedResetCount = spdResetInterval;

	}

	if (moveRad > 360) {
		moveRad -= 360;
	}

	Matrix4 matRot;

	if (easeMove.GetActive()) {
		pos = LerpBezireCubic(positions[0], positions[1], positions[2], positions[3], easeMove.GetTimeRate());
		matRot = CreateMatRot(world.translation_, pos);
		world.translation_ = pos;
	}
	else {
		matRot = CreateMatRot(world.translation_, world.translation_ + move);
		world.translation_ += move;

	}
	world.SetMatRot(matRot);
	world.TransferMatrix();
	speedResetCount--;

	collider->Update(world.matWorld_);

}

void MiniFish::Draw(ViewProjection viewPro)
{
	//bodyModel->Draw(world, viewPro);
	//eyeModel->Draw(world, viewPro);
}

void MiniFish::SetMovePos(const Vector3& stagePos, float stageRadius)
{
	//�v���C���[���Ō�ɂ����ꏊ���n�_�Ƃ���
	positions[0] = world.translation_;

	for (int i = 1; i < 4; i++) {

		//�x�W�G�Ȑ��̂��ߑ��x�𕪊�
		float spd = spdPerSec / 4.0f;

		//�X�e�[�W�̊O�ɍs���Ȃ����߂̍��W�ݒ�
		//	ImGui::Text("not active...");


		//i��1�̎��̂�(����_1��)�O���rad����O��90�̕��ɂ���
		if (i == 1) {
			moveRad += Random(-90, 90);
		}
		else {
			moveRad = Random(0, 360);
		}

		positions[i].x = sin(moveRad * PI / 180) * spd;
		positions[i].z = cos(moveRad * PI / 180) * spd;
		positions[i].y = stagePos.y;
		positions[i] += positions[i - 1];
		Vector3 vecPosToStage = stagePos - positions[i];
		if (stageRadius < vecPosToStage.length()) {
			//�X�e�[�W���S����ړ����W�ւ̋������X�e�[�W�̔��a��肨�������Ȃ�������for����
			i--;
		}
	}
	//�C�[�W���O�J�n
	easeMove.Start(120);
}

void MiniFish::SetAttribute(unsigned short attribute)
{
	collider->SetAttribute(attribute);
}

void MiniFish::OnCollision()
{
	isAlive = false;
}