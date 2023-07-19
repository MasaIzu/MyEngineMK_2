#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include <CollisionManager.h>

TouchableObject* TouchableObject::Create(std::unique_ptr<Model>& model, WorldTransform& worldTrans)
{
	// オブジェクトのインスタンスを生成
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model, worldTrans)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(std::unique_ptr<Model>& model, WorldTransform& worldTrans)
{

	// コライダーの追加
	MeshCollider* collider = new MeshCollider();
	// コリジョンマネージャに追加
	collider->ConstructTriangles(model, worldTrans.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	CollisionManager::GetInstance()->AddCollider(collider);

	return true;
}