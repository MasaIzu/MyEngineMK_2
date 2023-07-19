#include "Ground.h"
#include <CollisionManager.h>

void Ground::Initialze()
{
	groundModel.reset(Model::CreateFromOBJ("Ground", true));

	groundTrans.Initialize();
	groundTrans.scale_ = { 100,0,100 };
	groundTrans.TransferMatrix();

	touchableObject.reset(TouchableObject::Create(groundModel, groundTrans));

	//meshCollider = std::make_unique<MeshCollider>();
	//CollisionManager::GetInstance()->AddCollider(meshCollider.get());
	//meshCollider->Update(groundTrans.matWorld_);
	//meshCollider->ConstructTriangles(groundModel, groundTrans.matWorld_);
	//meshCollider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

}

void Ground::Update()
{

}

void Ground::Draw(const ViewProjection& view)
{
	groundModel->Draw(groundTrans, view);
}
