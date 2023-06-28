#include "Ground.h"

void Ground::Initialze()
{
	groundModel.reset(Model::CreateFromOBJ("Ground", true));

	groundTrans.Initialize();
	groundTrans.scale_ = { 100,0,100 };
	groundTrans.TransferMatrix();

}

void Ground::Update()
{

}

void Ground::Draw(const ViewProjection& view)
{
	groundModel->Draw(groundTrans, view);
}
