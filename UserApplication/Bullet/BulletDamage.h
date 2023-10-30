#pragma once
#include <stdint.h>

/// <summary>
/// バレットダメージクラス
/// </summary>
class BulletDamage
{
public:
	static BulletDamage* GetInstance();

public://ゲッター
	uint32_t GetEnemyMissileBulletDamage() const {
		return EnemyMissileBulletDamage;
	}

private://シングルトン
	BulletDamage() = default;
	BulletDamage(const BulletDamage&) = delete;
	~BulletDamage() = default;
	BulletDamage& operator = (const BulletDamage&) = delete;

private://メンバ変数

	uint32_t EnemyMissileBulletDamage = 111;
	uint32_t EnemyNormalBulletDamage = 91;



};