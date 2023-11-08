#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"
#include "NormalGun.h"
#include "Player.h"
#include "MissileGun.h"
#include <Sprite3D.h>
#include "EnemyHP3DUI.h"
#include "EnemyHP2DUI.h"

/// <summary>
/// 中ボス
/// </summary>
class MiddleBossEnemy {
private:
	enum class AttackType
	{
		Nomal,//4つバラマキ
		Missile,//ミサイル
		MoveingAttack,//移動しながら
		Move,//移動
		NotAttack,//攻撃しない
	};

public://基本関数
	MiddleBossEnemy();
	~MiddleBossEnemy();

	//初期化
	void Initialize(Player* player);
	//タイトル用初期化
	void TitleInitialize();
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection_);
	//スプライト描画
	void DrawSprite(const ViewProjection& viewProjection_);
	//ムービー用更新
	bool MovieUpdate(const Vector3& StartPos, Vector3& EndPos);
	//タイトル用アップデート
	void TitleUpdate(const Vector3& TrackingLocation);
	//弾を作る
	void MakeMissileBullet();
	//タイトルの弾を作る
	void MakeTitleMissileBullet();
	//タイトルリセット
	void ResetTitleMove();

private://関数
	//タイマー
	void Timer();
	//アタック
	void Attack();
	//考える時間
	void ThinkingTime();
	//移動の値更新
	void WorldTransUpdate();
	//アタックタイプ変更
	void CheckAttackType();
	//タイプのランダム
	uint32_t RandomType(uint32_t& NoUseType);

	//

public://Setter
	//スプラインセット
	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter
	//指定位置にいるかどうか
	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	//スプラインが終わったか
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }
	//死んだか
	bool GetIsDead()const { return isDead; }
	//ポジションリターン
	Vector3 GetPosition() const;

private://const関連
	static const uint32_t BossEnemyBodyCount = 20;

	static const uint32_t AttackedKeepCount = 5;

	const uint32_t MoveFirstTime = 50;
	const uint32_t MoveOneMoreTime = 30;
	const uint32_t MaxAttackCoolTime = 50;
	const uint32_t MaxNomalTime = 50;
	const uint32_t MaxNomalBulletCoolTime = 5;
	const uint32_t MaxMissileTime = 100;
	const uint32_t MaxMissileBulletCoolTime = 50;
	const uint32_t MaxMoveingAttackTime = 40;
	const uint32_t MaxMoveingAttackBulletTime = 1;
private://クラス関連
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans;

	WorldTransform DebugWorldTrans;

	//スプライン
	std::unique_ptr<SplinePosition> MoveSpline;//途中のスプライン

	//弾
	std::unique_ptr<NormalGun> normalGun;
	std::unique_ptr<MissileGun> missileGun;
	std::unique_ptr<EnemyHP2DUI> enemyHP2DUI;
	std::unique_ptr<EnemyHP3DUI> enemyHP3DUI;
	// コライダー
	BaseCollider* MiddleBossCollider = nullptr;


private://イーナムクラス


private://別クラスから値をもらう
	Player* player = nullptr;

private://EnemyBossクラス変数

	bool isStart = false;
	bool isTurn = false;
	bool isSporn = false;
	bool isDead = false;
	bool isAttack = false;
	bool isMoveing = false;
	bool isAngleGet = false;
	bool isOneMoreTime = false;
	bool isBackSponePos = false;
	bool isTitleShot = false;

	uint32_t BulletCoolTime = 0;
	uint32_t MoveingTimer = 0;
	uint32_t MaxMoveingTimer = 0;
	uint32_t MaxBulletCoolTime = 0;
	uint32_t MaxMiddleBossHp = 1000;
	uint32_t MiddleBossHp = MaxMiddleBossHp;
	uint32_t AttackCooltime = 0;
	uint32_t AttackTypeCount = static_cast<uint32_t>(AttackType::Move);
	uint32_t AllAttackTypeCount = static_cast<uint32_t>(AttackType::NotAttack) + 1;
	uint32_t KeepAttackingTime = 0;
	uint32_t RotTime = 0;
	uint32_t mveType = 0;
	uint32_t MoveTimes = 0;
	uint32_t MaxMoveTimes = 3;
	uint32_t BackPosCounter = 1;
	uint32_t BackMissileFirstCoolTime = 20;
	uint32_t BackMissileCoolTime = 6;
	uint32_t BackMissileTimes = 0;
	uint32_t BackMissileMaxTimes = 10;
	uint32_t BulletMake = 10;

	float Scale = 5.0f;
	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;
	float MovieUpdateTimes = 0.0f;
	float MaxMovieUpdateTimes = 120.0f;
	float Radius = 10.0f;
	float Angle = 0.0f;
	float AngleSize = 0.0f;
	float RotSpeed = 1.0f;
	float BulletSpeed = 6.0f;
	float MoveSafeRadius = 100.0f;
	float jampHeight = 20.0f;
	float BackStrength = 30.0f;
	float BackSpeed = 2.0f;
	float MovePower = 30.0f;
	float BackBosPower = 50.0f;

	Vector2 HpPosition = { 500,45 };

	Vector3 BonePos;
	Vector3 EndPos;
	Vector3 Velocity;
	Vector3 MovePos;
	Vector3 MoveStartPos;
	Vector3 BackLerpPos;

	AttackType attackType = AttackType::NotAttack;
	std::array<AttackType,AttackedKeepCount>oldAttackType;

	std::vector<Vector3> BackPoints{};
};