#include "Torch.h"

Torch::Torch(Model* model)
{
	torchModel = model;
}

void Torch::Initialze(const WorldTransform& trans)
{
	torchTrans = trans;
	torchTrans.TransferMatrix();

	TorchParticle = std::make_unique<ParticleEditor>();
	TorchParticle->Initialize("Torch");
	TorchParticle->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	//touchableObject.reset(TouchableObject::Create(groundModel,groundTrans,COLLISION_ATTR_LANDSHAPE));

}

void Torch::Update()
{

}

void Torch::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	TorchParticle->CSUpdate(cmdList,MyMath::Vec3ToVec4(torchTrans.translation_),true);
}

void Torch::Draw(const ViewProjection& view,const ViewProjection& LightViewProjection_)
{
	torchModel->Draw(torchTrans,view,LightViewProjection_);
}

void Torch::ShadowDraw(const ViewProjection& view)
{
	torchModel->ShadowDraw(torchTrans,view);
}

void Torch::ParticleDraw(const ViewProjection& view)
{
	TorchParticle->Draw(view);
}
