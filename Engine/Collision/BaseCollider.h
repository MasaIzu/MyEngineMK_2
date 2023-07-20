#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"

#include "Matrix4.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;


	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(const Matrix4& worldPos) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const float& speed, const Vector3& look) = 0;
	virtual void Update(const Matrix4& worldPos, const uint32_t& Cooltime, const bool& isCoolTime) = 0;
	virtual void Update(const Matrix4& worldPos, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime, const bool& isCoolTime) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime) = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }
	inline unsigned short GetAttribute() { return attribute; }
	inline bool GetHit() { return isHitPlayerAttack; }
	inline bool GetHitEnemyEachOtherHit() { return isEnemyHittingEachOther; }
	inline bool GetBulletHit() { return isBulletMeshHit; }
	inline Vector3 GetRejectVec() { return rejectVec; }
	inline Matrix4 GetWorldPos() { return worldPos_; }


	/// <summary>
	/// 当たり判定属性をセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SetAttribute(unsigned short attribute) {
		this->attribute = attribute;
	}

	/// <summary>
	/// 雑魚敵当たり判定属性をセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SetAttributeWakeEnemy(unsigned short attribute) {
		this->attributeWakeEnemy = attribute;
	}

	/// <summary>
	/// 当たり判定属性を追加
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void AddAttribute(unsigned short attribute) {
		this->attribute |= attribute;
	}

	/// <summary>
	/// 当たり判定属性を削除
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void RemoveAttribute(unsigned short attribute) {
		this->attribute &= !attribute;
	}

	/// <summary>
	/// ヒットしたのをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void Reset() {
		isHitPlayerAttack = false;
	}

	/// <summary>
	/// 排斥ベクトルのヒットをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void EnemyHittingEachOtherReset() {
		isEnemyHittingEachOther = false;
	}

	/// <summary>
	/// 排斥ベクトルのヒットをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void BulletMeshHitReset() {
		isBulletMeshHit = false;
	}

protected:
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	// 当たり判定属性
	unsigned short attribute = 0b1111111111111111;
	// 雑魚的当たり判定属性
	unsigned short attributeWakeEnemy = 0b1111111111111111;

	bool isHitPlayerAttack = false;//プレーヤーの弾が当たっている場合
	bool isEnemyHittingEachOther = false;//敵同士が当たっている場合
	bool isBulletMeshHit = false;

	Vector3 rejectVec;//排斥ベクトル

	//位置
	Matrix4 worldPos_;
};

