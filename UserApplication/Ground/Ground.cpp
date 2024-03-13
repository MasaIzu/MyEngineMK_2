#include "Ground.h"
#include <CollisionManager.h>

Ground::Ground(Model* model)
{
	groundModel = model;
}

void Ground::Initialze()
{

	groundTrans.Initialize();
	groundTrans.TransferMatrix();

	touchableObject.reset(TouchableObject::Create(groundModel, groundTrans, COLLISION_ATTR_LANDSHAPE));

}

void Ground::Update()
{

}

void Ground::Draw(const ViewProjection& view,const ViewProjection& LightViewProjection_)
{
	groundModel->Draw(groundTrans, view,LightViewProjection_);
}

void Ground::ShadowDraw(const ViewProjection& view)
{
	groundModel->ShadowDraw(groundTrans,view);
}
