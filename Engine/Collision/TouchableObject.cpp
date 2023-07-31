#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include <CollisionManager.h>

TouchableObject* TouchableObject::Create(Model* model, WorldTransform& worldTrans, unsigned short attribute)
{
	// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(model, worldTrans, attribute)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(Model* model, WorldTransform& worldTrans, unsigned short attribute)
{

	// �R���C�_�[�̒ǉ�
	MeshCollider* collider = new MeshCollider();
	// �R���W�����}�l�[�W���ɒǉ�
	collider->ConstructTriangles(model, worldTrans.matWorld_);
	collider->SetAttribute(attribute);
	CollisionManager::GetInstance()->AddCollider(collider);

	return true;
}