#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include <CollisionManager.h>

TouchableObject* TouchableObject::Create(std::unique_ptr<Model>& model, WorldTransform& worldTrans)
{
	// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(model, worldTrans)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(std::unique_ptr<Model>& model, WorldTransform& worldTrans)
{

	// �R���C�_�[�̒ǉ�
	MeshCollider* collider = new MeshCollider();
	// �R���W�����}�l�[�W���ɒǉ�
	collider->ConstructTriangles(model, worldTrans.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	CollisionManager::GetInstance()->AddCollider(collider);

	return true;
}