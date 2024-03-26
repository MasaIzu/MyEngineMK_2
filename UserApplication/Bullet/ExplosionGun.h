#pragma once
#include "NormalBullet.h"
#include <array>
#include <stdint.h>
#include "WorldTransform.h"
#include <FbxModel.h>
#include <FBXObject3d.h>
#include "ExplosionBullet.h"
#include <AudioManager.h>

class ExplosionGun
{
public://基本関数
	ExplosionGun(const unsigned short Attribute);
	~ExplosionGun();

	//初期化
	void Initialize(const Vector3& Pos,Model* BulletModel,AudioManager* audioManager_,const float& soundVol);
	//更新
	void Update(const Vector3& Pos,const Vector3& rot);
	//描画
	void Draw(const ViewProjection& viewProjection_,const ViewProjection& LightViewProjection_);
	//Fbx描画
	void FbxDraw(const ViewProjection& viewProjection_,const ViewProjection& lightViewProjection_);
	void FbxShadowDraw(const ViewProjection& lightViewProjection_);
	//パーティクル
	void CSUpdate(ID3D12GraphicsCommandList* commandList);
	void ParticleDraw(const ViewProjection& viewProjection_);
public:
	//発射
	void ShotBullet(const Vector3& BulletVec);

private:
	//ポジションの更新
	void UpdatePosition();

	//タイムの更新
	void TimeUpdate();

public:
	//見てる方向のゲット
	WorldTarnsLook GetLook()const;
	bool GetIsReload();
	uint32_t GetBulletCount();
private://コンスト
	const static uint32_t BulletMaxCount = 2;


private://クラス関連
	//バレットの数
	std::array<std::unique_ptr<ExplosionBullet>,BulletMaxCount> explosionBullet;
	std::unique_ptr<Model> model_;
	WorldTransform GunTrans;

	std::unique_ptr<FBXModel> fbxModel_;
	std::unique_ptr<FBXObject3d> fbxObj3d_;

	AudioManager* audioManager = nullptr;
private:
	bool isReload = false;

	uint32_t CoolTime = 0;
	uint32_t MaxCoolTime = 15;
	uint32_t MaxReloadCoolTime = 200;
	uint32_t UseBulletCount = 0;

	float BulletSpeed = 4.5f;

	//爆発銃サウンド
	uint32_t ExNum = 0;
	uint32_t ExBaNum = 0;
	float ExSoundSize = 0.2f;
	float ExBaSoundSize = 0.2f;

};

