#include "ParticleManager.h"
#include <ParticleCS.h>
#include <ParticleHandHanabi.h>
#include <Explosion.h>
#include <Hibana.h>

void ParticleManager::Initialize(ID3D12Device* device)
{
	ParticleCS::StaticInitialize(device);
	ParticleHandHanabi::StaticInitialize(device);
	Explosion::StaticInitialize(device);
	Hibana::StaticInitialize(device);
}

void ParticleManager::Finalize()
{
	Hibana::StaticFinalize();
	Explosion::StaticFinalize();
	ParticleHandHanabi::StaticFinalize();
	ParticleCS::StaticFinalize();
}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	ParticleCS::PreDraw(cmdList);
}

void ParticleManager::PostDraw()
{
	ParticleCS::PostDraw();
}
