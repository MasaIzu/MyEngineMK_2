#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Initialize()
{
	AnmNumNumber_ = 0;
	//fbx
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("3JamJiki",true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->PlayAnimation(AnmNumNumber_);
	fbxObj3d_->Update();

}

void Animation::Update()
{
	if ( isStart )
	{
		if ( nowAnmFCount_ < maxFcount )
		{
			isMotionFinish = false;
			nowAnmFCount_++;
		}
		else
		{
			isStart = false;
			isMotionFinish = true;
			if ( isLoop )
			{
				nowAnmFCount_ = 0;
				isStart = true;
			}
		}
	}
	
	fbxObj3d_->Update(AnmNumNumber_,nowAnmFCount_,maxFcount);
}

void Animation::FbxDraw(const WorldTransform& worldTransform,const ViewProjection& viewProjection_)
{
	fbxObj3d_->Draw(worldTransform,viewProjection_);
}

void Animation::SetAnimation(const uint32_t& animNumber,const uint32_t& startCount,const uint32_t& maxCount,const bool& loop)
{
	if ( isStart == false )
	{
		nowAnmFCount_ = startCount;
	}
	isStart = true;
	isMotionFinish = false;
	isLoop = loop;
	AnmNumNumber_ = animNumber;
	maxFcount = maxCount;
}

bool Animation::GetNowAnimFinish()
{
	return isMotionFinish;
}

Matrix4 Animation::GetBonePos(const uint32_t& boneNum)
{
	return fbxObj3d_->GetBonesMatPtr(boneNum);
}
