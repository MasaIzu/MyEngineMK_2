#include "ParticleManager.h"
#include "ParticleCS.h"
#include "ParticleHandHanabi.h"
#include "Explosion.h"
#include "MeshParticle.h"
#include "ParticleBoost.h"

void ParticleManager::Initialize(ID3D12Device* device)
{
	//ParticleCS::StaticInitialize(device);
	ParticleHandHanabi::StaticInitialize(device);
	ParticleBoost::StaticInitialize(device);
	//Explosion::StaticInitialize(device);
	//MeshParticle::StaticInitialize(device);
}

void ParticleManager::Finalize()
{
	//MeshParticle::StaticFinalize();
	//Explosion::StaticFinalize();
	ParticleBoost::StaticFinalize();
	ParticleHandHanabi::StaticFinalize();
	//ParticleCS::StaticFinalize();
}
