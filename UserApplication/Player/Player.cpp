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
	playerWorldTrans.translation_ = { 0,10.0f,-50.0f };

	playerWorldTransForBullet.Initialize();

	playerBullet = std::make_unique<PlayerBullet>();
	playerBullet->Initialize();

	DebugWorldTrans.Initialize();

	AttackSprite = Sprite::Create(TextureManager::Load("shoujuun.png"));
	AttackSprite->SetAnchorPoint({ 0.5f,0.5f });

	// コリジョンマネージャに追加
	PlayerCollider = new SphereCollider(Vector4(0, Radius, 0, 0), Radius);
	CollisionManager::GetInstance()->AddCollider(PlayerCollider);
	PlayerCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	PlayerCollider->Update(playerWorldTrans.matWorld_);

}

void Player::Update()
{

	//回転させる
	PlayerRot();
	if (playerBullet->GetExpandingBullet() == false) {
		//どう動くか
		Move();
	}

	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallVec.y = max(fallVec.y + fallAcc, fallVYMin);
		// 移動
		playerWorldTrans.translation_.x += fallVec.x;
		playerWorldTrans.translation_.y += fallVec.y;
		playerWorldTrans.translation_.z += fallVec.z;
	}
	//当たり判定チェック
	CheckPlayerCollider();

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

	ImGui::End();

	DebugWorldTrans.translation_ = Distance;
	DebugWorldTrans.TransferMatrix();

	///playerBullet->UpdateWhileExpanding(GetPlayerPos(), DistanceNolm);
	playerBullet->Update();

}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(playerWorldTrans, viewProjection_);
	playerBullet->Draw(viewProjection_);
	model_->Draw(DebugWorldTrans, viewProjection_);
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
		DistanceNolm = Distance - GetPlayerPos();
		PlayerToCameraDistance = DistanceNolm.length();
		DistanceNolm.normalize();
		bulletNumber = playerBullet->MakePlayerBullet(GetPlayerPos(), DistanceNolm, PlayerToCameraDistance);
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
}

void Player::CheckPlayerCollider()
{

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(PlayerCollider);
	assert(sphereCollider);

	sphereCollider->SetRadius(Radius);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const Vector4 up = { 0,1,0,0 };

			Vector4 rejectDir = info.reject;
			rejectDir.normalize();
			rejectDir.dot(up);
			float cos = rejectDir.y;

			// 地面判定しきい値
			const float threshold = cosf(DirectX::XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		Vector4 move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE, &playerWorldTrans.matWorld_);
	// 交差による排斥分動かす
	playerWorldTrans.translation_.x += callback.move.x;
	playerWorldTrans.translation_.y += callback.move.y;
	playerWorldTrans.translation_.z += callback.move.z;
	// ワールド行列更新
	playerWorldTrans.TransferMatrix();
	PlayerCollider->Update(playerWorldTrans.matWorld_);

	float RayPos = -1.0f;

	//地面メッシュコライダー
	{
		// 球の上端から球の下端までのレイキャスト
		Ray Groundray;
		Groundray.start = sphereCollider->center;
		Groundray.start.y += sphereCollider->GetRadius();
		Groundray.dir = { 0,-1.0f,0,0 };
		RaycastHit raycastHit;


		// 接地状態
		if (onGround) {
			// スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			// 接地を維持
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
				onGround = true;
				playerWorldTrans.translation_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			}
			// 地面がないので落下
			else {
				onGround = false;
				fallVec = {};
			}
		}
		// 落下状態
		else {
			if (CollisionManager::GetInstance()->Raycast(Groundray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
				// 着地
				onGround = true;
				playerWorldTrans.translation_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			}
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = sphereCollider->center;
		wall.start.y += sphereCollider->GetRadius();
		wall.dir = { 0,0,1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, sphereCollider->GetRadius())) {
			playerWorldTrans.translation_.z += (wallRaycastHit.distance - sphereCollider->GetRadius());
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = sphereCollider->center;
		wall.start.y += sphereCollider->GetRadius();
		wall.dir = { 0,0,-1,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, sphereCollider->GetRadius())) {
			playerWorldTrans.translation_.z -= (wallRaycastHit.distance - sphereCollider->GetRadius());
		}
	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = sphereCollider->center;
		wall.start.y += sphereCollider->GetRadius();
		wall.dir = { 1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, sphereCollider->GetRadius())) {
			playerWorldTrans.translation_.x += (wallRaycastHit.distance - sphereCollider->GetRadius());
		}

	}
	{
		//横メッシュコライダー
		Ray wall;
		wall.start = sphereCollider->center;
		wall.start.y += sphereCollider->GetRadius();
		wall.dir = { -1,0,0,0 };
		RaycastHit wallRaycastHit;
		// スムーズに坂を下る為の吸着距離

		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(wall, COLLISION_ATTR_LANDSHAPE, &wallRaycastHit, sphereCollider->GetRadius())) {
			playerWorldTrans.translation_.x -= (wallRaycastHit.distance - sphereCollider->GetRadius());
		}

	}
}

