#include "PlayerAnimation.h"

PlayerAnimation::PlayerAnimation()
{
}

PlayerAnimation::~PlayerAnimation()
{
}

void PlayerAnimation::Initialize()
{
	//fbx
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("3JamJiki",true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->PlayAnimation(0);
	fbxObj3d_->Update();

}

void PlayerAnimation::Update()
{
	if ( isStart )
	{
		if ( nowAnmFCount_ < maxFcount )
		{
			nowAnmFCount_++;
		}
		else
		{
			isStart = false;
			if ( isLoop )
			{
				nowAnmFCount_ = 0;
				isStart = true;
			}
		}
	}
	
	fbxObj3d_->Update(AnmNumNumber_,nowAnmFCount_,maxFcount);
}

void PlayerAnimation::FbxDraw(const WorldTransform& worldTransform,const ViewProjection& viewProjection_)
{
	fbxObj3d_->Draw(worldTransform,viewProjection_);
}

void PlayerAnimation::SetAnimation(const uint32_t& animNumber,const uint32_t& startCount,const uint32_t& maxCount,const bool& loop)
{
	isStart = true;
	isLoop = loop;
	AnmNumNumber_ = animNumber;
	nowAnmFCount_ = startCount;
	maxFcount = maxCount;
}
