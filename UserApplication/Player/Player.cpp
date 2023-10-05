#include "Player.h"
#include "WinApp.h"
#include "MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
//#include <FbxLoader.h>
#include <SphereCollider.h>


Player::Player()
{
}

Player::~Player()
{

}

void Player::Initialize(const Vector3& Pos, ViewProjection* viewProjection)
{
	input_ = Input::GetInstance();
	model_.reset(Model::CreateFromOBJ("sphere", true));
	playerWorldTrans.Initialize();
	playerWorldTrans.translation_ = Pos;
	playerWorldTransHed.Initialize();
	playerWorldTransHed.translation_ = { 0,Radius * 2,0.0f };
	playerWorldTransHed.parent_ = &playerWorldTrans;
	playerWorldTransHed.TransferMatrix();

	viewProjection_ = viewProjection;

	playerWorldTransForBullet.Initialize();
	StartingPointOfGrapple.Initialize();

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

	// Attackフォント
	AttackFontSprite[0] = Sprite::Create(TextureManager::Load("sprite/Attack_off.png"));
	AttackFontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	AttackFontSprite[1] = Sprite::Create(TextureManager::Load("sprite/Attack_on.png"));
	AttackFontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });

	// Wフォント
	W_FontSprite[0] = Sprite::Create(TextureManager::Load("sprite/160x144_W_Font.png"));
	W_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	W_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	W_FontSprite[1] = Sprite::Create(TextureManager::Load("sprite/160x144_W_Font.png"));
	W_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	W_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Aフォント
	A_FontSprite[0] = Sprite::Create(TextureManager::Load("sprite/160x144_A_Font.png"));
	A_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	A_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	A_FontSprite[1] = Sprite::Create(TextureManager::Load("sprite/160x144_A_Font.png"));
	A_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	A_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Sフォント
	S_FontSprite[0] = Sprite::Create(TextureManager::Load("sprite/160x144_S_Font.png"));
	S_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	S_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	S_FontSprite[1] = Sprite::Create(TextureManager::Load("sprite/160x144_S_Font.png"));
	S_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	S_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	// Dフォント
	D_FontSprite[0] = Sprite::Create(TextureManager::Load("sprite/160x144_D_Font.png"));
	D_FontSprite[0]->SetTextureRect({ 0,0 }, { 160,144 });
	D_FontSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	D_FontSprite[1] = Sprite::Create(TextureManager::Load("sprite/160x144_D_Font.png"));
	D_FontSprite[1]->SetTextureRect({ 160,0 }, { 160,144 });
	D_FontSprite[1]->SetAnchorPoint({ 0.5f,0.5f });

	Vector2 AttackFontsize = { 188.0f,54.0f };
	Vector2 W_Fontsize = { 32.0f ,28.0f };
	Vector2 A_Fontsize = { 32.0f ,28.0f };
	Vector2 S_Fontsize = { 32.0f ,28.0f };
	Vector2 D_Fontsize = { 32.0f ,28.0f };

	for (int i = 0; i < 2; i++) {
		AttackFontSprite[i]->SetSize(AttackFontsize);
		W_FontSprite[i]->SetSize(W_Fontsize);
		A_FontSprite[i]->SetSize(A_Fontsize);
		S_FontSprite[i]->SetSize(S_Fontsize);
		D_FontSprite[i]->SetSize(D_Fontsize);
	}




}

void Player::Update()
{
	isAttack = false;
	isPressing = false;
	isPushW = false;
	isPushA = false;
	isPushS = false;
	isPushD = false;

	if (playerMoveSpline->GetFinishSpline()) {
		isHitRail = false;
	}
	if (FirstMoveSpline->GetFinishSpline()) {
		isHitFirstRail = false;
	}
	//回転させる
	PlayerRot();

	//どう動くか
	Move();

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
	if (FinalMoveSpline->GetHowReturnIndex(5)) {
		Stop = true;
	}

	if (input_->TriggerKey(DIK_LSHIFT)) {

		isSliding = true;
		SlidingTime = 60;
		SlidingSpeed = 1.5f;
		DownSlidingTimes = 90.0f;

		if (isPushW == 1 && isPushA == 0 && isPushD == 0) {
			SlidingNumber = 1;
		}
		else if (isPushW == 0 && isPushS == 0 && isPushA == 1) {
			SlidingNumber = 2;
		}
		else if (isPushS == 1 && isPushA == 0 && isPushD == 0) {
			SlidingNumber = 3;
		}
		else if (isPushW == 0 && isPushS == 0 && isPushD == 1) {
			SlidingNumber = 4;
		}
		else if (isPushW == 1 && isPushA == 1 && isPushD == 0) {
			SlidingNumber = 5;
		}
		else if (isPushW == 1 && isPushA == 0 && isPushD == 1) {
			SlidingNumber = 6;
		}
		else if (isPushS == 1 && isPushA == 1 && isPushD == 0) {
			SlidingNumber = 7;
		}
		else if (isPushS == 1 && isPushA == 0 && isPushD == 1) {
			SlidingNumber = 8;
		}
		else {
			SlidingNumber = 0;
		}
	}

	if (isSliding) {
		if (SlidingTime > 0) {
			SlidingTime--;
			if (SlidingNumber == 1) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.look * SlidingSpeed;
			}
			else if (SlidingNumber == 2) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookLeft * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.lookLeft * SlidingSpeed;
			}
			else if (SlidingNumber == 3) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.lookBack * SlidingSpeed;
			}
			else if (SlidingNumber == 4) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookRight * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.lookRight * SlidingSpeed;
			}
			else if (SlidingNumber == 5) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look_lookLeft * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.look_lookLeft * SlidingSpeed;
			}
			else if (SlidingNumber == 6) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look_lookRight * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.look_lookRight * SlidingSpeed;
			}
			else if (SlidingNumber == 7) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack_lookLeft * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.lookBack_lookLeft * SlidingSpeed;
			}
			else if (SlidingNumber == 8) {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.lookBack_lookRight * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.lookBack_lookRight * SlidingSpeed;
			}
			else {
				playerWorldTrans.translation_ += playerWorldTrans.LookVelocity.look * SlidingSpeed;
				SlidingVelocity = playerWorldTrans.LookVelocity.look * SlidingSpeed;
			}
		}
		else {
			isSliding = false;

			DownSlidingVelocity = SlidingVelocity / DownSlidingTimes;
			SlidingVelocity -= DownSlidingVelocity;
			playerWorldTrans.translation_ += SlidingVelocity;
		}
	}
	else {
		if (onGround) {
			SlidingVelocity = { 0,0,0 };
			DownSlidingVelocity = { 0,0,0 };
		}
		else {
			if (DownSlidingTimes > 0.0f) {
				DownSlidingTimes -= 1.0f;
				SlidingVelocity -= DownSlidingVelocity;
				playerWorldTrans.translation_ += SlidingVelocity;
			}
		}
	}

	//移動の値更新
	WorldTransUpdate();

	DistanceNolm = Distance - MyMath::GetWorldTransform(playerWorldTransHed.matWorld_);
	PlayerToCameraTargetVecDistance = DistanceNolm.length();
	DistanceNolm.normalize();

	UpdateReticle();

	isGrapple = false;
	if (input_->MouseInputing(1)) {
		isPressing = true;

		// 範囲レイキャスト
		Ray GroundRay;
		GroundRay.start = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(playerWorldTransHed.matWorld_));
		//LookRay.start.y += CameraRayCollisionRadius;
		PlayerToAimSaiteVec = ShootVec.norm();
		PlayerToAimSaiteVecDistance = PlayerToCameraTargetVecDistance * 1.5f;
		GroundRay.dir = MyMath::Vec3ToVec4(PlayerToAimSaiteVec.norm());
		RaycastHit raycastHit;

		//カメラとの間に地面があれば位置を変える
		if (CollisionManager::GetInstance()->Raycast(GroundRay, COLLISION_ATTR_LANDSHAPE, &raycastHit, PlayerToAimSaiteVecDistance)) {
			StartingPointOfGrapple.translation_ = MyMath::Vec4ToVec3(raycastHit.inter);
		}

		StartingPointOfGrapple.TransferMatrix();

	}

	if (input_->MouseInputing(0)) {
		isAttack = true;
	}


	if (input_->MouseInputTrigger(1)) {

	}


	ImGui::Begin("Player");


	ImGui::Text("angle:%f,%f,%f", angle.x, angle.y, angle.z);
	ImGui::End();

	//DebugWorldTrans.translation_ = Distance;
	//DebugWorldTrans.TransferMatrix();

	///playerBullet->UpdateWhileExpanding(GetPlayerPos(), DistanceNolm);
	playerBullet->Update();

}

void Player::Draw(ViewProjection& viewProjection)
{
	//model_->Draw(DebugWorldTrans, viewProjection_);
	if (isPressing) {
		model_->Draw(StartingPointOfGrapple, viewProjection);
	}
	playerBullet->Draw(viewProjection);
	model_->Draw(playerWorldTrans, viewProjection);
	model_->Draw(playerWorldTransHed, viewProjection);
}

void Player::DrawSprite()
{
	AttackSprite->Draw(Vector2(640, 360), Vector4(1, 1, 1, 1), 2);
	Vector2 W_Fontpos = { 270,530 };
	Vector2 A_Fontpos = { 240,560 };
	Vector2 S_Fontpos = { 270,560 };
	Vector2 D_Fontpos = { 300,560 };
	Vector2 AttackFontpos = { 150,480 };
	for (int i = 0; i < 2; i++) {
		if (input_->PushKey(DIK_W)) {
			W_FontSprite[1]->Draw(W_Fontpos, { 1,1,1,1 });
		}
		else {
			W_FontSprite[0]->Draw(W_Fontpos, { 1,1,1,1 });
		}
		if (input_->PushKey(DIK_A)) {
			A_FontSprite[1]->Draw(A_Fontpos, { 1,1,1,1 });
		}
		else {
			A_FontSprite[0]->Draw(A_Fontpos, { 1,1,1,1 });
		}
		if (input_->PushKey(DIK_S)) {
			S_FontSprite[1]->Draw(S_Fontpos, { 1,1,1,1 });
		}
		else {
			S_FontSprite[0]->Draw(S_Fontpos, { 1,1,1,1 });
		}
		if (input_->PushKey(DIK_D)) {
			D_FontSprite[1]->Draw(D_Fontpos, { 1,1,1,1 });
		}
		else {
			D_FontSprite[0]->Draw(D_Fontpos, { 1,1,1,1 });
		}
		if (isAttack) {
			AttackFontSprite[0]->Draw(AttackFontpos, { 1,1,1,1 });
		}
		else if (isAttack == false) {
			AttackFontSprite[1]->Draw(AttackFontpos, { 1,1,1,1 });
		}
	}
}

void Player::CopyParticle()
{
	playerBullet->CopyParticle();
}

void Player::AttackUpdate(const Vector3& EnemyPos, bool& LockOn)
{
	if (isAttack == true) {
		PlayerAttack(EnemyPos, LockOn);
	}
}

void Player::Move()
{
	playerMoveMent = { 0.0f,0.0f,0.0f };

	if (isHitRail == false || playerMoveSpline->GetFinishSpline() == true) {
		if (input_->PushKey(DIK_W)) {
			isPushW = true;
			playerMoveMent += playerWorldTrans.LookVelocity.look.norm() * playerSpeed;
		}
		if (input_->PushKey(DIK_S)) {
			isPushS = true;
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack.norm() * playerSpeed;
			//playerMoveMent.y -= 0.02f;
		}
		if (input_->PushKey(DIK_A)) {
			isPushA = true;
			playerMoveMent += playerWorldTrans.LookVelocity.lookLeft.norm() * playerSpeed;
		}
		if (input_->PushKey(DIK_D)) {
			isPushD = true;
			playerMoveMent += playerWorldTrans.LookVelocity.lookRight.norm() * playerSpeed;
		}

		if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_A) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.look_lookLeft.norm() * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_W) == 1 && input_->PushKey(DIK_D) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.look_lookRight.norm() * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_A) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookLeft.norm() * diagonalPlayerSpeed;
		}
		if (input_->PushKey(DIK_S) == 1 && input_->PushKey(DIK_D) == 1) {
			playerMoveMent = { 0.0f,0.0f,0.0f };
			playerMoveMent += playerWorldTrans.LookVelocity.lookBack_lookRight.norm() * diagonalPlayerSpeed;
		}
		//if (onGround) {
		if (input_->TriggerKey(DIK_SPACE)) {
			playerMoveSpline->ResetNearSplineReset();
			FirstMoveSpline->ResetNearSplineReset();
			onGround = false;
			const float jumpVYFist = 0.6f;
			float SlidingJump = 0.0f;
			float ChangeSlidingSpeed = 0.7f;
			float ChangeJump = 1.1f;
			if (isSliding) {
				SlidingJump = 1.5f;

				if (isPushW == 1 && isPushA == 0 && isPushD == 0) {
					if (SlidingNumber != 0 && SlidingNumber != 1) {
						SlidingNumber = 1;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushW == 0 && isPushS == 0 && isPushA == 1) {
					if (SlidingNumber != 2) {
						SlidingNumber = 2;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushS == 1 && isPushA == 0 && isPushD == 0) {
					if (SlidingNumber != 3) {
						SlidingNumber = 3;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushW == 0 && isPushS == 0 && isPushD == 1) {
					if (SlidingNumber != 4) {
						SlidingNumber = 4;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushW == 1 && isPushA == 1 && isPushD == 0) {
					if (SlidingNumber != 5) {
						SlidingNumber = 5;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushW == 1 && isPushA == 0 && isPushD == 1) {
					if (SlidingNumber != 6) {
						SlidingNumber = 6;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushS == 1 && isPushA == 1 && isPushD == 0) {
					if (SlidingNumber != 7) {
						SlidingNumber = 7;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else if (isPushS == 1 && isPushA == 0 && isPushD == 1) {
					if (SlidingNumber != 8) {
						SlidingNumber = 8;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
				else {
					if (SlidingNumber != 0) {
						SlidingNumber = 0;
						SlidingSpeed = ChangeSlidingSpeed;
						SlidingJump = ChangeJump;
					}
				}
			}
			fallVec = { 0, jumpVYFist + SlidingJump, 0, 0 };

		}
		//}

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

void Player::PlayerAttack(const Vector3& EnemyPos, bool& LockOn)
{
	if (AttackPhase_ == AttackPhase::Nothing) {
		AttackPhase_ = AttackPhase::AttackCombo1;
	}
	switch (AttackPhase_)
	{
	case Player::AttackPhase::AttackCombo1:
		if (LockOn) {
			bulletNumber = playerBullet->MakePlayerBullet(MyMath::GetWorldTransform(playerWorldTransHed.matWorld_), EnemyPos.norm(), PlayerToCameraTargetVecDistance);
		}
		else {
			bulletNumber = playerBullet->MakePlayerBullet(MyMath::GetWorldTransform(playerWorldTransHed.matWorld_), ShootVec.norm(), PlayerToCameraTargetVecDistance);
		}
	
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
	//isHitRail = false;
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
	if (PlayerCollider->GetFirstSplineHit()) {
		PlayerCollider->FirstSplineHitReset();
		Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0, Radius, 0);
		FirstMoveSpline->ResetNearSpline(splinePos);
		isHitFirstRail = true;
	}
	//レールコライダー
	if (isHitRail == false) {
		if (PlayerCollider->GetSphereMeshHit()) {
			PlayerCollider->SphereMeshHitReset();
			Vector3 splinePos = playerWorldTrans.translation_ - Vector3(0, Radius, 0);
			playerMoveSpline->ResetNearSpline(splinePos);
			isHitRail = true;
		}
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
	if (isHitRail == false/* || isHitFirstRail == false*/) {
		// 落下処理
		if (!onGround) {
			// 下向き加速度
			const float fallAcc = -0.035f;
			const float fallVYMin = -1.3f;
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
	else if (playerMoveSpline->GetFinishSpline()) {
		isHitRail = false;
	}
	if (isHitFirstRail == true && FirstMoveSpline->GetFinishSpline() == false) {
		float speed = 0.02f;
		FirstMoveSpline->Update(speed);
		playerWorldTrans.translation_ = FirstMoveSpline->NowPos + Vector3(0, Radius, 0);
	}
	else if (FirstMoveSpline->GetFinishSpline()) {
		isHitFirstRail = false;
	}
	if (isHitFinalRail == true && FinalMoveSpline->GetFinishSpline() == false) {
		if (Stop == false) {
			float speed = 0.02f;
			FinalMoveSpline->Update(speed);
			playerWorldTrans.translation_ = FinalMoveSpline->NowPos + Vector3(0, Radius, 0);
		}
	}
	else if (FinalMoveSpline->GetFinishSpline()) {
		isHitFinalRail = false;
	}
}

void Player::UpdateReticle()
{
	Vector2 windowWH = Vector2(WinApp::GetInstance()->GetWindowSize().x, WinApp::GetInstance()->GetWindowSize().y);
	//ビューポート行列
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * Viewport;

	//バレットレイメッシュコライダー
	Ray wall;
	wall.start = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(playerWorldTransHed.matWorld_));
	wall.start.y -= 0.4f;
	wall.dir = MyMath::Vec3ToVec4(DistanceNolm);
	RaycastHit wallRaycastHit;

	if (input_->MouseInputing(2) == false) {
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, PlayerToCameraTargetVecDistance)) {
			//playerWorldTrans.translation_.x -= (wallRaycastHit.distance - Radius);
			ReticlePos = MyMath::Vec4ToVec3(wallRaycastHit.inter);
			ReticlePos.y += 0.4f;
			ShootVec = ReticlePos - MyMath::GetWorldTransform(playerWorldTransHed.matWorld_);
			//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
			//ReticlePos = MyMath::DivVecMat(ReticlePos, matViewProjectionViewport);
		}
		else {
			//ReticlePos = MyMath::DivVecMat(Distance, matViewProjectionViewport);
			ShootVec = DistanceNolm;
		}
	}

	DebugWorldTrans.translation_ = ReticlePos;
	DebugWorldTrans.TransferMatrix();
}

float Player::AngleSelect(float& angle_, float& selectAngle)
{
	//float sprt = std::sqrt(angle * angle);
	int intOverAngle = static_cast<int>(angle_ / selectAngle);

	float floatOverAngle = static_cast<float>(intOverAngle);

	float Angle = angle_ - (selectAngle * floatOverAngle);

	return Angle;
}

