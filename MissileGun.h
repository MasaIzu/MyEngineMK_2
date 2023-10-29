#pragma once
#include "MissileBullet.h"
#include <array>
#include <stdint.h>
#include "WorldTransform.h"

class MissileGun
{
public://基本関数
	MissileGun(const unsigned short Attribute);
	~MissileGun();

	//初期化
	void Initialize(const Vector3& Pos,Model* GunModel,Model* BulletModel);
	//更新
	void Update(const Vector3& GunPos,const Vector3& inductionPos);
	//描画
	void Draw(const ViewProjection& viewProjection_);

public:
	//発射
	void ShotBullet(const Vector3& BulletVec);

private:
	//ポジションの更新
	void UpdatePosition();

	//タイムの更新
	void TimeUpdate();

private://コンスト
	const static uint32_t BulletMaxCount = 30;


private://クラス関連
	//バレットの数
	std::array<std::unique_ptr<MissileBullet>,BulletMaxCount> normalBullet;
	Model* model_ = nullptr;
	WorldTransform GunTrans;

private:

	uint32_t CoolTime = 0;
	uint32_t MaxCoolTime = 15;

	float BulletSpeed = 1.0f;

};

