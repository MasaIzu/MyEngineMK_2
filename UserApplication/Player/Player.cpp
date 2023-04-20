#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>
#include"ImGuiManager.h"
#include <FbxLoader.h>


Player::Player()
{
}

Player::~Player()
{
	delete easing_;
}

void Player::Initialize(Model* model, float WindowWidth, float WindowHeight) {
	//NULLポインタチェック
	assert(model);
	playerModel_ = model;
	oldPlayerModel_.reset(Model::CreateFromOBJ("UFO", true));

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();

	Window_Width = WindowWidth;
	Window_Height = WindowHeight;

	easing_ = new Easing();

	// コリジョンマネージャに追加
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);

	playerAvoidance = 6.0f;

	for (int i = 0; i < SphereCount; i++) {
		// コリジョンマネージャに追加
		AttackCollider[i] = new SphereCollider(Vector4(0, radius, 0, 0), radius);
		CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);
	}

	//worldTransform_.translation_ = { 0,0,-100 };

	//ワールド変換の初期化
	worldTransform_.Initialize();
	oldWorldTransform_.Initialize();
	playerAttackTransform_.Initialize();

	collider->Update(worldTransform_.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	for (int i = 0; i < SphereCount; i++) {
		playerAttackTransformaaaa_[i].Initialize();
		playerAttackTransformaaaa_[i].TransferMatrix();
	}

	worldTransform_.scale_ = { 0.04f,0.04f,0.04f };

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();

	ParticleMan = std::make_unique<ParticleManager>();

	ParticleMan->Initialize();


	//体力の画像読み込み
	healthSprite = Sprite::Create(TextureManager::Load("mario.jpg"));
	healthSprite->SetAnchorPoint({ 0,0 });

	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("3dRowPoriAllMotion"));
	fbxmodel->Initialize();
	modelAnim = std::make_unique<FbxAnimation>();
	modelAnim->Load("3dRowPoriAllMotion");

}


void Player::Update(const ViewProjection& viewProjection) {

	Move();

	Attack();

	//if (isEnemyHit) {
	//	SetKnockBackCount();
	//	Collision();
	//}

	KnockBackUpdate();

	if (isAttackHit)
	{
		AttackCollision();
	}

	ParticleMan->Update();

	//worldTransform_.translation_ = { 5,0,20 };

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();

	collider->Update(worldTransform_.matWorld_);

	if (frem < MaxFrem) {
		frem += 0.013f;
	}
	else {
		frem = MinimumFrem;
	}

	if (input_->PushKey(DIK_P)) {
		frem = fremX;
	}
	if (input_->PushKey(DIK_J)) {
		fremX += 0.1;
	}
	if (input_->PushKey(DIK_K)) {
		fremX += 0.01;
	}
	if (input_->PushKey(DIK_N)) {
		fremX += -0.1;
	}
	if (input_->PushKey(DIK_M)) {
		fremX += -0.01;
	}

	fbxmodel->ModelAnimation(frem, modelAnim->GetAnimation(static_cast<int>(playerNowMotion)));



	ImGui::Begin("player");

	ImGui::Text("fremX:%f", fremX);
	ImGui::Text("frem:%f", frem);

	ImGui::Text("root:%f,%f,%f", root.x, root.y, root.z);

	ImGui::End();
}

void Player::Move() {

	PlayerMoveMent = { 0,0,0 };
	Avoidance = { 0,0,0 };
	isPushLeft = false;
	isPushRight = false;
	isPushBack = false;
	spaceInput = false;
	isWalk = false;

	if (timer > 0) {
		timer--;
		alpha -= 0.02f;
	}
	else {
		spaceInput = false;
		collider->SetAttribute(COLLISION_ATTR_ALLIES);
	}

	cameraLookmat = MyMath::Rotation(Vector3(0, 90, 0), 2);

	Vector3 moveRot = cameraLook;
	cameraLook.normalize();

	root = (worldTransform_.lookLeft - worldTransform_.translation_);

	if (input_->PushKey(DIK_W)) {
		PlayerMoveMent += cameraLook * playerSpeed;
		isWalk = true;
	}
	if (input_->PushKey(DIK_A)) {
		PlayerMoveMent += root.normalize() * playerSpeed;
		isPushLeft = true;
		isWalk = true;
	}
	if (input_->PushKey(DIK_S)) {
		PlayerMoveMent -= cameraLook * playerSpeed;
		isPushBack = true;
		isWalk = true;
	}
	if (input_->PushKey(DIK_D)) {
		PlayerMoveMent -= root.normalize() * playerSpeed;
		isPushRight = true;
		isWalk = true;
	}


	/*if (input_->PushKey(DIK_W)) {
		PlayerMoveMent += cameraLook * playerSpeed;
		isWalk = true;
	}
	if (input_->PushKey(DIK_A)) {
		moveRot = MyMath::MatVector(cameraLookmat, moveRot);
		moveRot.y = 0;
		moveRot.norm();
		PlayerMoveMent -= moveRot * playerSpeed;
		isPushLeft = true;
		isWalk = true;
	}
	if (input_->PushKey(DIK_S)) {
		PlayerMoveMent -= cameraLook * playerSpeed;
		isPushBack = true;
		isWalk = true;
	}
	if (input_->PushKey(DIK_D)) {
		moveRot = MyMath::MatVector(cameraLookmat, moveRot);
		moveRot.y = 0;
		moveRot.norm();
		PlayerMoveMent += moveRot * playerSpeed;
		isPushRight = true;
		isWalk = true;
	}*/

	//worldTransform_.translation_ += PlayerMoveMent;

	if (spaceInput == false) {
		if (input_->TriggerKey(DIK_SPACE)) {
			spaceInput = true;
			timer = 20;
			alpha = 0.3f;
			collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
			oldWorldTransform_.translation_ = worldTransform_.translation_;

			if (isPushLeft == true) {
				Avoidance.x = playerAvoidance;
			}
			else if (isPushRight == true) {
				Avoidance.x = -playerAvoidance;
			}
			else if (isPushBack == true) {
				Avoidance.z = playerAvoidance;
			}
			else {
				Avoidance.z = -playerAvoidance;
			}
		}
	}

	worldTransform_.SetRot({ 0,-MyMath::GetAngle(angle),0 });

	CameraRot = MyMath::Rotation(Vector3(Rot.x, Rot.y, Rot.z), 6);

	Avoidance = MyMath::MatVector(CameraRot, Avoidance);
	Avoidance.y = 0;
	Avoidance.normalize();
	Avoidance *= playerAvoidance;

	//worldTransform_.translation_ += playerMovement;
	//worldTransform_.translation_ += Avoidance;
	AvoidanceMove += Avoidance;

	Vector3 allMove = PlayerMoveMent + AvoidanceMove;

	AvoidanceMove = { 0,0,0 };
	//ステージからでないようにする処理
	{
		float AR;
		float BR;

		AR = pow((worldTransform_.translation_.x + allMove.x) - 0, 2) + pow((0 + worldTransform_.translation_.z + allMove.z) - 0, 2);
		BR = pow((satgeSize - worldTransform_.scale_.x * 2), 2);

		if (AR <= BR)
		{
			worldTransform_.translation_ += allMove;
		}
	}
	

	if (input_->MouseInputing(2)) {
		oldWorldTransform_.translation_ = worldTransform_.look;
	}

	if (isWalk == true) {
		if (isWalking == false) {
			isWalking = true;
			MaxFrem = 1.3f;
			MinimumFrem = 0.45f;
			frem = 0;
			playerNowMotion = PlayerMotion::hasirihajimeTOowari;
		}

	}
	else {

		if (isWalking == true) {
			frem = 1.63f;
		}

		isWalking = false;

		MaxFrem = 1.8f;
		MinimumFrem = 1.8f;

	}


}

void Player::Attack() {

	Vector3 moveRot = cameraLook;

	if (input_->MouseInputing(0)) {
		//実行前にカウント値を取得
		//計測開始時間の初期化
		isAttack = true;
		startCount = 0;
		nowCount = 0;
		timeRate = 0;
		startIndex = 1;
	}

	if (isAttack == true) {
		if (nowCount < maxTime * 4) {
			nowCount++;
		}
		else {
			isAttack = false;
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
		}

		//補間で使うデータ
		//start → end を知らん秒で完了させる
		Vector3 p0(worldTransform_.lookRight);									//スタート地点
		Vector3 p1((worldTransform_.look + worldTransform_.lookRight) / 2);		//制御点その1
		Vector3 p2(worldTransform_.look);										//制御点その2
		Vector3 p3((worldTransform_.look + worldTransform_.lookLeft) / 2);		//制御点その3
		Vector3 p4(worldTransform_.lookLeft);									//ゴール地点

		p0 = p0 + ((p0 - GetWorldPosition()) * attackDistanceX);
		p1 = p1 + ((p1 - GetWorldPosition()) * attackDistanceZ);
		p2 = p2 + ((p2 - GetWorldPosition()) * attackDistanceZ);
		p3 = p3 + ((p3 - GetWorldPosition()) * attackDistanceZ);
		p4 = p4 + ((p4 - GetWorldPosition()) * attackDistanceX);


		points = { p0,p0,p1,p2,p3,p4,p4 };

		// 落下
		// スタート地点        ：start
		// エンド地点        　：end
		// 経過時間            ：elapsedTime [s]
		// 移動完了の率(経過時間/全体時間) : timeRate (%)
		elapsedTime = nowCount - startCount;
		timeRate = elapsedTime / maxTime;

		if (timeRate >= 1.0f)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex += 1.0f;
				timeRate -= 1.0f;
				startCount = nowCount;
			}
			else
			{
				timeRate = 1.0f;
			}
		}

		position = splinePosition(points, startIndex, timeRate);

		playerAttackTransform_.translation_ = position;

		float sphereX = position.x - GetWorldPosition().x;
		float sphereY = position.y - GetWorldPosition().y;
		float sphereZ = position.z - GetWorldPosition().z;

		Vector3 sphere(sphereX / SphereCount, sphereY / SphereCount, sphereZ / SphereCount);

		for (int i = 0; i < SphereCount; i++) {
			colliderPos[i] = GetWorldPosition() + sphere * i;
			worldSpherePos[i] = MyMath::Translation(colliderPos[i]);
			AttackCollider[i]->Update(worldSpherePos[i]);
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
			playerAttackTransformaaaa_[i].translation_ = colliderPos[i];
			playerAttackTransformaaaa_[i].TransferMatrix();
		}

	}

}

void Player::SetKnockBackCount()
{
	KnockBack = MyMath::GetWorldTransform(worldTransform_.matWorld_) - MyMath::GetWorldTransform(EnemyPos);
	KnockBack.normalize();
	KnockBack.y = 0;
	KnockBack = KnockBack * KnockBackDistance;

	moveTime = 0;
	KnockBack = MyMath::GetWorldTransform(worldTransform_.matWorld_) + KnockBack;

	isKnockBack = true;
}

void Player::KnockBackUpdate()
{
	if (moveTime < MaxMoveTime) {
		moveTime++;
		KnockBack += PlayerMoveMent;
		Vector3 KnockBackMove = easing_->InOutVec3(MyMath::GetWorldTransform(worldTransform_.matWorld_), KnockBack, moveTime, MaxMoveTime);

		float AR;
		float BR;

		AR = pow((KnockBackMove.x) - 0, 2) + pow((KnockBackMove.z) - 0, 2);
		BR = pow((satgeSize - worldTransform_.scale_.x * 2), 2);

		if (AR <= BR)
		{
			worldTransform_.translation_ = KnockBackMove;
		}
	}
	else
	{
		isKnockBack = false;
	}

}

void Player::Draw(ViewProjection viewProjection_) {

	if (timer == 0) {
		playerModel_->Draw(worldTransform_, viewProjection_);
	}
	if (timer > 0) {
		oldWorldTransform_.alpha = alpha;
		oldPlayerModel_->Draw(oldWorldTransform_, viewProjection_);
	}
	if (isAttack) {
		for (int i = 0; i < SphereCount; i++) {
			playerModel_->Draw(playerAttackTransformaaaa_[i], viewProjection_);
		}
	}

}

void Player::PlayerFbxDraw(ViewProjection viewProjection_) {

	fbxmodel->Draw(worldTransform_, viewProjection_);

}


void Player::DrawHealth() {

	Vector2 size = { 48.0f * HP,48.0f };
	Vector2 pos = { WinApp::window_width / 2 - (48 * 10),50 };
	healthSprite->SetSize(size);
	healthSprite->Draw(pos, { 1,1,1,1 });
}

void Player::ParticleDraw(ViewProjection view)
{
	ParticleMan->Draw(view);
}


Vector3 Player::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	return result;
}

void Player::Collision(int damage)
{
	if (isKnockBack == false)
	{
		SetKnockBackCount();
		//スペースキーを押していたら
		for (int i = 0; i < 4; i++)
		{
			//消えるまでの時間
			const float rnd_life = 70.0f;
			//最低限のライフ
			const float constlife = 10;
			float life = (float)rand() / RAND_MAX * rnd_life + constlife;

			//XYZの広がる距離
			const float rnd_pos = 30.0f;
			//Y方向には最低限の飛ぶ距離
			const float constPosY = 15;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = abs((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 2;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			//追加
			ParticleMan->Add(ParticleManager::Type::Out, life, true, MyMath::GetWorldTransform(worldTransform_.matWorld_), { MyMath::GetWorldTransform(worldTransform_.matWorld_).x,MyMath::GetWorldTransform(worldTransform_.matWorld_).y + pos.y,MyMath::GetWorldTransform(worldTransform_.matWorld_).z }, MyMath::GetWorldTransform(worldTransform_.matWorld_) + pos, 0.2, 0.2, { 0.5,1,1,0.7 }, { 0.5,1,1,0.3 });
		}
		HP -= damage;
	}
}

void Player::AttackCollision()
{
	//スペースキーを押していたら
	for (int i = 0; i < 10; i++)
	{
		//消えるまでの時間
		const float rnd_life = 50.0f;
		//最低限のライフ
		const float constlife = 10;
		float life = (float)rand() / RAND_MAX * rnd_life - rnd_life / 2.0f + constlife;

		//XYZの広がる距離
		const float rnd_pos = 200.0f;
		//Y方向には最低限の飛ぶ距離
		const float constPosY = 15;
		Vector3 pos;
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//追加
		ParticleMan->Add(ParticleManager::Type::In, life, false, MyMath::GetWorldTransform(worldTransform_.matWorld_), { MyMath::GetWorldTransform(worldTransform_.matWorld_).x,MyMath::GetWorldTransform(worldTransform_.matWorld_).y + pos.y,MyMath::GetWorldTransform(worldTransform_.matWorld_).z }, MyMath::GetWorldTransform(worldTransform_.matWorld_) + pos, 0.3, 0.1, { 1,1,0.95,1 }, { 1,1,0.95,0 });
	}
}

Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	// 補完すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n]; // Pnの値を返す
	if (startIndex < 1)return points[1]; // P1の値を返す

	// p0～p3の制御点を取得する　※　p1～p2を補完する
	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	// Catmull - Romの式による補間
	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t
		+ (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t
		+ (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);

	return position;
}
