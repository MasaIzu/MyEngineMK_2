#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
#include <FbxLoader.h>
#include <SphereCollider.h>


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	input_ = Input::GetInstance();
	model_.reset(Model::CreateFromOBJ("sphere", true));
	playerWorldTrans.Initialize();
	playerWorldTrans.translation_ = { 0,-155.0f,-283.0f };
	playerWorldTransHed.Initialize();
	playerWorldTransHed.translation_ = { 0,Radius * 2,0.0f };
	playerWorldTransHed.parent_ = &playerWorldTrans;
	playerWorldTransHed.TransferMatrix();

	playerWorldTransForBullet.Initialize();

	playerBullet = std::make_unique<PlayerBullet>();
	playerBullet->Initialize();

	DebugWorldTrans.Initialize();

	AttackSprite = Sprite::Create(TextureManager::Load("sprite/shoujuun.png"));
	AttackSprite->SetAnchorPoint({ 0.5f,0.5f });

	// コリジョンマネージャに追加
	PlayerCollider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider);
	PlayerCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	playerMoveSpline = std::make_unique<SplinePosition>();
	FirstMoveSpline = std::make_unique<SplinePosition>();
	FinalMoveSpline = std::make_unique<SplinePosition>();
}

void Player::Update()
{
	if (playerMoveSpline->GetFinishSpline()) {
		isHitRail = false;
	}
	if (FirstMoveSpline->GetFinishSpline()) {
		isHitFirstRail = false;
	}
	//回転させる
	PlayerRot();
	if (playerBullet->GetExpandingBullet() == false) {
		//どう動くか
		Move();
	}
	//落下
	Fall();
	//当たり判定チェック
	CheckPlayerCollider();

	//スプラインアップデート
	SplineUpdate();

	if (isCameraModeNotFree == true) {
		//カメラの位置でアルファが決まる
		alpha = 1.0f - (1.0f - PlayerToCameraDistance / cameraMaxDistance);
		playerWorldTrans.alpha = alpha;
		playerWorldTransHed.alpha = alpha;
	}
	
	//移動の値更新
	WorldTransUpdate();

	if (input_->MouseInputTrigger(0)) {
		//isPlayerSetUp = true;
		//PlayerAttack();
	}
	if (input_->MouseInputing(0)) {
		isPlayerSetUp = true;
		PlayerAttack();
	}
	if (input_->MouseInputTrigger(1)) {

	}


	ImGui::Begin("Player");

	ImGui::Text("Distance:%f,%f,%f", Distance.x, Distance.y, Distance.z);
	ImGui::Text("Distance:%f", alpha);
	ImGui::End();

	DebugWorldTrans.translation_ = Distance;
	DebugWorldTrans.TransferMatrix();

	///playerBullet->UpdateWhileExpanding(GetPlayerPos(), DistanceNolm);
	playerBullet->Update();

}

void Player::Draw(ViewProjection& viewProjection_)
{
	//model_->Draw(DebugWorldTrans, viewProjection_);
	model_->Draw(playerWorldTrans, viewProjection_);
	model_->Draw(playerWorldTransHed, viewProjection_);
	playerBullet->Draw(viewProjection_);
}

void Player::DrawSprite(ViewProjection& viewProjection_)
{
	AttackSprite->Draw(Vector2(640, 360), Vector4(1, 1, 1, 1), 2);
}

void Player::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	playerBullet->CSUpdate(cmdList);
}

void Player::ParticleDraw(ViewProjection& viewProjection_)
{
	playerBullet->ParticleDraw(viewProjection_);
}

void Player::CopyParticle()
{
	playerBullet->CopyParticle();
}

void Player::Move()
{
	playerMoveMent = { 0.0f,0.0f,0.0f };

	if (isHitRail == false || playerMoveSpline->GetFinishSpline() == true) {
		if (input_->PushKey(DIK_W)) {
			playerMoveMent += playerWorldTrans.LookVelocity.look * playerSpeed;
		}
		if (input_->PushKey(DIK_S)) {
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack * playerSpeed;
			//playerMoveMent.y -= 0.02f;
		}
		if (input_->PushKey(DIK_A)) {
			playerMoveMent += playerWorldTrans.LookVelocity.lookLeft * playerSpeed;
		}
		if (input_->PushKey(DIK_D)) {
			playerMoveMent += playerWorldTrans.LookVelocity.lookRight * playerSpeed;
		}

		if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_A) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.look_lookLeft * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_D) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.look_lookRight * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_A) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookLeft * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_D) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookRight * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_SPACE)) {
			playerMoveSpline->ResetNearSplineReset();
			FirstMoveSpline->ResetNearSplineReset();
			onGround = false;
			const float jumpVYFist = 0.4f;
			fallVec = { 0, jumpVYFist, 0, 0 };
		}
	}
	


	playerWorldTrans.translation_ += playerMoveMent;

}

void Player::PlayerRot()
{

	playerWorldTrans.SetRot(Vector3(0.0f, cameraRot.x, 0.0f));
	playerWorldTransForBullet.SetRot(Vector3(cameraRot.y, cameraRot.x, 0.0f));
	//値更新
	WorldTransUpdate();
}

void Player::PlayerAttack()
{
	if (AttackPhase_ == AttackPhase::Nothing) {
		AttackPhase_ = AttackPhase::AttackCombo1;
	}
	switch (AttackPhase_)
	{
	case Player::AttackPhase::AttackCombo1:
		DistanceNolm = Distance - MyMath::GetWorldTransform(playerWorldTransHed.matWorld_);
		PlayerToCameraTargetVecDistance = DistanceNolm.length();
		DistanceNolm.normalize();
		bulletNumber = playerBullet->MakePlayerBullet(MyMath::GetWorldTransform(playerWorldTransHed.matWorld_), DistanceNolm, PlayerToCameraTargetVecDistance);
		break;
	case Player::AttackPhase::AttackCombo2:

		break;
	case Player::AttackPhase::AttackCombo3:

		break;
	case Player::AttackPhase::AttackCombo4:

		break;
	case Player::AttackPhase::AttackUlt:

		break;
	case Player::AttackPhase::Nothing:

		break;
	default:
		break;
	}
}

void Player::WorldTransUpdate()
{
	playerWorldTrans.TransferMatrix();
	playerWorldTransForBullet.TransferMatrix();

	playerWorldTransHed.parent_ = &playerWorldTrans;
	playerWorldTransHed.TransferMatrix();

}

void Player::CheckPlayerCollider()
{
	isHitRail = false;
	//isHitFirstRail = false;
	// ワールド行列更新
	playerWorldTrans.TransferMatrix();
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	//地面メッシュコライダー
	{
		// 球の上端から球の下端までのレイキャスト
		Ray Groundray;
		Groundray.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		Groundray.start.y += Radius;
		Groundray.dir = { 0,-1.0f,0,0 };
		RaycastHit raycastHit;


		// 接地状態
		if (onGround) {
			// スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			// 接地を維持
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, Radius * 2.0f + adsDistance)) {
				onGround = true;
				playerWorldTrans.translation_.y -= (raycastHit.distance - Radius * 2.0f);
			}
			// 地面がないので落下
			else {
				onGround = false;
				fallVec = {};
			}
		}
		// 落下状態
		else {
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, Radius * 2.0f)) {
				// 着地
				onGround = true;
				playerWorldTrans.translation_.y -= (raycastHit.distance - Radius * 2.0f);
			}
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { 0,0,1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius)) {
			playerWorldTrans.translation_.z += (wallRaycastHit.distance - Radius);
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { 0,0,-1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius)) {
			playerWorldTrans.translation_.z -= (wallRaycastHit.distance - Radius);
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { 1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius)) {
			playerWorldTrans.translation_.x += (wallRaycastHit.distance - Radius);
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = MyMath::Vec3ToVec4(playerWorldTrans.translation_);
		wall.start.y += Radius;
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, Radius)) {
			playerWorldTrans.translation_.x -= (wallRaycastHit.distance - Radius);
		}

	}

	//レールコライダー
	if (PlayerCollider->GetSphereMeshHit()) {
		PlayerCollider->SphereMeshHitReset();
		Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0, Radius, 0);
		playerMoveSpline->ResetNearSpline(splinePos);
		isHitRail = true;
	}
	//レールコライダー
	if (PlayerCollider->GetFirstSplineHit()) {
		PlayerCollider->FirstSplineHitReset();
		Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0, Radius, 0);
		FirstMoveSpline->ResetNearSpline(splinePos);
		isHitFirstRail = true;
	}
	//レールコライダー
	if (PlayerCollider->GetFinalSplineHit()) {
		PlayerCollider->FinalSplineHitReset();
		Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0, Radius, 0);
		FirstMoveSpline->ResetNearSpline(splinePos);
		isHitFinalRail = true;
	}

}

void Player::Fall()
{
	if (isHitRail == false || isHitFirstRail == false) {
		// 落下処理
		if (!onGround) {
			// 下向き加速度
			const float fallAcc = -0.03f;
			const float fallVYMin = -0.7f;
			// 加速
			fallVec.y = max(fallVec.y + fallAcc, fallVYMin);
			// 移動
			playerWorldTrans.translation_.x += fallVec.x;
			playerWorldTrans.translation_.y += fallVec.y;
			playerWorldTrans.translation_.z += fallVec.z;
		}
	}
}

void Player::SplineUpdate()
{
	if (isHitRail == true && playerMoveSpline->GetFinishSpline() == false) {
		float speed = 0.02f;
		playerMoveSpline->Update(speed);
		playerWorldTrans.translation_ = playerMoveSpline->NowPos + Vector3(0, Radius, 0);
	}
	if (isHitFirstRail == true && FirstMoveSpline->GetFinishSpline() == false) {
		float speed = 0.02f;
		FirstMoveSpline->Update(speed);
		playerWorldTrans.translation_ = FirstMoveSpline->NowPos + Vector3(0, Radius, 0);
	}
	if (isHitFinalRail == true && FinalMoveSpline->GetFinishSpline() == false) {
		float speed = 0.02f;
		FinalMoveSpline->Update(speed);
		playerWorldTrans.translation_ = FinalMoveSpline->NowPos + Vector3(0, Radius, 0);
	}

}

