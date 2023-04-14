#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"
#include"ImGuiManager.h"
#include"Easing.h"
#include "CollisionManager.h"
#include <SphereCollider.h>
#include <CollisionAttribute.h>
#include"Sprite.h"

Boss::~Boss()
{
	delete collider;
}

void Boss::Initialize()
{
	fishParent.pos.Initialize();
	fishParent.radius = 20.0f;




	if (!fishes.empty()) {
		fishes.clear();
	}

	randSpdParam = 3.75f;

	//体力の画像読み込み
	healthPicture = TextureManager::Load("mario.jpg");
	healthSprite = Sprite::Create(healthPicture);
	healthSprite->SetAnchorPoint({ 0,0 });
	//剣のモデル初期化
	swordModel.reset(Model::CreateFromOBJ("dammySword", true));

	//魚のモデル初期化
	fishBodyModel.reset(Model::CreateFromOBJ("FishBody", true));
	fishEyeModel.reset(Model::CreateFromOBJ("FishMedama", true));

	phase1 = BossFirstPhase::Idle;
	nextPhaseInterval = attackCooltime;

	radius = 30.0f;
	// コリジョンマネージャに追加
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);

	collider->Update(fishParent.pos.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	swordTransform.Initialize();
	swordTransform.TransferMatrix();

	testTrans.Initialize();
	testTrans2.Initialize();

}

void Boss::Update(const Vector3& targetPos)
{
	//第1形態の魚群の更新
	//ImGui::Begin("sword");

	//狙う敵の座標更新
	this->targetPos = targetPos;

	ImGui::Text("health %d", bossHealth);

	//ダメージのタイマーをセット
	if (damageTimer > 0) {
		damageTimer--;
	}
	else {
		collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	}

	//魚が一匹も存在していないか、HPが0なら判定を無敵にして処理を終わる
	if (fishes.empty() || bossHealth <= 0) {
		collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
		return;
	}

	switch (phase1) {
	case BossFirstPhase::Idle:
		UpdateIdle();
		break;
	case BossFirstPhase::Atk_Sword:
		UpdateAtkSword();
		break;
	case BossFirstPhase::Atk_Rush:
		UpdateAtkRush();
		break;
	case BossFirstPhase::BeginMotion:
		UpdateBeginMotion();
		break;
	default:
		break;

	}


	//	ImGui::End();

	collider->Update(fishParent.pos.matWorld_);
}

void Boss::CreateFish(Vector3 spawnPos)
{

	float posY = Random(-fishParent.radius, fishParent.radius);

	//y座標が親の半径を超えないようにする
	if (fabs(posY) > fishParent.radius) {
		if (posY > 0)posY = fishParent.radius;
		else posY = -fishParent.radius;
	}

	//新しい魚を作成
	fish newFish;
	//角度をランダムに指定
	newFish.radian = Random(0.0f, 360.0f);
	//与えられた引数から半径を設定
	newFish.radius = sqrt(fishParent.radius * fishParent.radius - posY * posY);
	//ワールド行列初期化
	newFish.pos.Initialize();
	newFish.pos.scale_ = { 0.5f,0.5f,0.5f };
	//Y座標は引数で
	newFish.pos.translation_.y = posY;
	//newFish.pos.rotation_.y = 0.78f;
	newFish.pos.TransferMatrix();

	//速度をランダムに決定
	newFish.spd = Random(0.0f, randSpdParam);

	//親のポインタ貰う
	newFish.pos.parent_ = &fishParent.pos;

	//微妙なずれの大きさは自分の半径の1/10からランダムに
	float displacementParam = newFish.radius / 10.0f * 1.5f;
	newFish.displacement = Vector3(Random(0.0f, displacementParam), Random(0.0f, displacementParam), Random(0.0f, displacementParam));

	Vector3 pos;

	//	pos.y = fishParent.radius - fishes[i].radius;
	pos.x = sin(PI / 180.0f * newFish.radian) * newFish.radius;
	pos.z = cos(PI / 180.0f * newFish.radian) * newFish.radius;

	float plus = Random(-1.0f, 1.0f);
	float num = 1;
	if (plus < 0) {
		num = -1;
	}

	pos.y = (sqrt(fishParent.radius * fishParent.radius - newFish.radius * newFish.radius) * num);

	pos += newFish.displacement;

	newFish.afterPos = pos;
	newFish.afterPos = newFish.pos.matWorld_.transform(newFish.afterPos, fishParent.pos.matWorld_);
	newFish.beforePos = spawnPos;
	Vector3 randParam;
	randParam = { Random(-5, 5), Random(-5, 5), Random(-5, 5) };
	newFish.controll1 = newFish.beforePos + randParam;
	randParam = { Random(-5, 5), 30.0f, Random(-5, 5) };
	newFish.controll2 = newFish.afterPos + randParam;
	newFish.easeMove.Start(45);

	newFish.pos.translation_ = pos;
	newFish.pos.TransferMatrix();
	//配列にい列
	fishes.push_back(newFish);
}

void Boss::Draw(ViewProjection viewProMat)
{
	if (bossHealth <= 0) {
		return;
	}


	if (phase1 == BossFirstPhase::Atk_Sword) {
		swordModel->Draw(swordTransform, viewProMat);

		testTrans.scale_ = { 3,3,3 };
		testTrans.translation_ = posSwordColCube1;
		testTrans.TransferMatrix();
		fishEyeModel->Draw(testTrans, viewProMat);
		testTrans2.scale_ = { 3,3,3 };
		testTrans2.translation_ = posSwordColCube2;
		testTrans2.TransferMatrix();
		fishEyeModel->Draw(testTrans2, viewProMat);

	}

	if (fishes.empty()) {
		return;
	}

	for (int i = 0; i < fishes.size(); i++) {
		fishBodyModel->Draw(fishes[i].pos, viewProMat);
		fishEyeModel->Draw(fishes[i].pos, viewProMat);
	}

	swordModel->Draw(fishParent.pos, viewProMat);

}

void Boss::DrawHealth() {

	Vector2 size = { 48.0f * bossHealth,48.0f };
	Vector2 pos = { WinApp::window_width / 2 - (48 * 10),WinApp::window_height * 8.0f / 10.0f };
	healthSprite->SetSize(size);
	healthSprite->Draw(pos, {1,1,1,1});


}

void Boss::UpdateIdle()
{
	//魚群の中心(真ん中)の座標更新
	FishDirectionUpdate();
	fishParent.pos.TransferMatrix();

	//魚1匹ずつの更新

	for (int i = 0; i < fishes.size(); i++) {

		Vector3 pos;

		fishes[i].easeMove.Update();

		if (fishes[i].easeMove.GetActive()) {
			ImGui::Text("fish create active");
			fishes[i].pos.parent_ = nullptr;
			pos = LerpBezireCubic(fishes[i].beforePos, fishes[i].controll1, fishes[i].controll2, fishes[i].afterPos, fishes[i].easeMove.GetTimeRate());
			Matrix4 matrot;
			matrot = CreateMatRot(fishes[i].pos.translation_, pos);
			fishes[i].pos.SetMatRot(matrot);
		}
		else {
			ImGui::Text("atk interval:%d", nextPhaseInterval);
			if (fishes[i].pos.parent_ == nullptr) {
				fishes[i].pos.parent_ = &fishParent.pos;
			}

			//魚のラジアン(球の周回軌道)を加算
			fishes[i].radian += fishes[i].spd;
			if (fishes[i].radian > 360.0f) {
				fishes[i].radian -= 360.0f;
				fishes[i].spd = Random(0.0f, randSpdParam);
			}

			//座標を計算


			pos = fishes[i].pos.translation_ - fishes[i].displacement - fishParent.pos.translation_;
			pos.x = 100;
			pos.z = 100;

			Vector3 rotaVec;
			Vector3 vec = { 0, 1, 0 };
			vec.normalize();

			Quaternion rotaQuaternion = { vec,fishes[i].radian * PI / 180.0f };
			Quaternion posQ = { pos.x,pos.y,pos.z,0 };
			rotaVec = rotaQuaternion.multiply(posQ.GetAxis());

			rotaVec *= fishes[i].radius;

			//	pos.y = fishParent.radius - fishes[i].radius;
			pos.x = sin(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
			pos.z = cos(PI / 180.0f * fishes[i].radian) * fishes[i].radius;

			pos.x = rotaVec.x;
			pos.z = rotaVec.z;
			//pos = rotaVec;

			float plus = Random(-1.0f, 1.0f);
			float num = 1;
			if (plus < 0) {
				num = -1;
			}

			//	pos.y = (sqrt(fishParent.radius * fishParent.radius - fishes[i].radius * fishes[i].radius) *num);

			pos += fishes[i].displacement;

			//回転用の移動ベクトルを作成
			Vector3 dirvec = pos - fishes[i].pos.translation_;
			dirvec.normalize();
			//	Quaternion dirQ = { dirvec.x,dirvec.y,dirvec.z,0 };
			Matrix4 matrot;
			matrot = CreateMatRot(fishes[i].pos.translation_, pos);

			fishes[i].pos.SetMatRot(matrot);


			//攻撃のクールタイムを減らす

		}


		fishes[i].pos.translation_ = pos;

		//fishes[i].pos.rotation_.y =PI / fishes[i].radian * 180.0f;
		fishes[i].pos.TransferMatrix();
	}

	if (!fishes[fishes.size() - 1].easeMove.GetActive() && !fishes[0].easeMove.GetActive()) {
		nextPhaseInterval--;
	}

	if (nextPhaseInterval == 0) {

		//50%で突進、残りで剣撃
		//if (IsPercent(50)) {
		if (moveFlag == 0) {
			//突進攻撃の回数を初期化
			rushCount = rushMaxCount;
			//フェーズ移行
			phase1 = BossFirstPhase::Atk_Rush;

			moveFlag = 1;
		}
		else {
			moveFlag = 0;

			//0になったらクールタイムを攻撃開始モーションの時間に設定
			nextPhaseInterval = beginAttackDelay;
			//フェーズを移行
			phase1 = BossFirstPhase::BeginMotion;
			bossSwordPhase = BossSwordPhase::Start;

			//魚群の乱回転のためのランダムなベクトルを作成
			for (int i = 0; i < fishes.size(); i++) {
				fishes[i].randomVec = Vector3(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), Random(-1.0f, 1.0f));
				fishes[i].randomVec.normalize();
				fishes[i].spd = Random(randSpdParam, randSpdParam * 2);

				float displacement = fishParent.radius / 10.0f;
				fishes[i].radius = Random(fishParent.radius - displacement, fishParent.radius + displacement);
				fishes[i].radius = fishParent.radius;
			}
		}

	}
}

void Boss::UpdateAtkSword()
{
	//行動時間のフレームまとめ
	const int swordCreateTime = 120;	//剣の生成時間
	const int swordMoveTime = 120;		//剣の移動時間
	const int swordAtkTime = 300;		//剣の攻撃時間
	const int swordBreakTime = 120;		//剣の崩壊時間
	const int moveDelay = 60;			//補間のためのディレイ
	float distancePtoSword = 90.0f;		//標的と剣の距離(スカラー)

	ImGui::Text("SwordColCube1 : %f,%f,%f", posSwordColCube1.x, posSwordColCube1.y, posSwordColCube1.z);
	ImGui::Text("SwordColCube2 : %f,%f,%f", posSwordColCube2.x, posSwordColCube2.y, posSwordColCube2.z);

	//モーション開始の瞬間
	if (nextPhaseInterval <= 0) {
		//生成の瞬間
		if (bossSwordPhase == BossSwordPhase::Start) {
			bossSwordPhase = BossSwordPhase::Create;
			nextPhaseInterval = swordCreateTime;
			swordTransform.scale_ = { 0,0,0 };
			easeSwordScale.Start(swordCreateTime);
			//剣の座標
			swordPos.x = -30;
			swordPos.z = 30;
			//剣の座標を親のmatrixと掛け算
			Matrix4 mat;
			swordPos = mat.transform(swordPos, fishParent.pos.matWorld_);

			//swordPos.x = fishParent.pos.translation_.x;
			beforeScale = { 0.5f,0.5f,0.5f };
			afterScale = { 0,0,0 };

			if (!choiceFishIndex.empty()) {
				choiceFishIndex.clear();
			}


		}//移動開始の瞬間
		else if (bossSwordPhase == BossSwordPhase::Create) {
			bossSwordPhase = BossSwordPhase::Cooltime_Create;
			nextPhaseInterval = moveDelay;
		}
		else if (bossSwordPhase == BossSwordPhase::Cooltime_Create) {
			bossSwordPhase = BossSwordPhase::Move;
			nextPhaseInterval = swordMoveTime;
			easeSwordPos.Start(swordMoveTime);
			swordTransform.translation_ = swordPos;
			//		swordTransform.SetRot({ 0,0,0 });

		}//攻撃開始の瞬間
		else if (bossSwordPhase == BossSwordPhase::Move) {
			bossSwordPhase = BossSwordPhase::Attack;
			nextPhaseInterval = swordAtkTime;
			easeSwordPos.Start(swordAtkTime);
		}//霧散の瞬間
		else if (bossSwordPhase == BossSwordPhase::Attack) {
			bossSwordPhase = BossSwordPhase::Destroy;
			nextPhaseInterval = swordBreakTime;
			easeSwordScale.Start(swordBreakTime);
			afterScale = { 0.5f,0.5f,0.5f };
			beforeScale = { 0,0,0 };
		}
		else if (bossSwordPhase == BossSwordPhase::Destroy) {
			bossSwordPhase = BossSwordPhase::Cooltime_Destroy;
			nextPhaseInterval = moveDelay;
		}
		else if (bossSwordPhase == BossSwordPhase::Cooltime_Destroy) {
			//親子関係を戻す
			for (int i = 0; i < fishes.size(); i++) {
				if (fishes[i].pos.parent_ == nullptr) {
					fishes[i].pos.parent_ = &fishParent.pos;
				}
			}

			//モーション終了
			nextPhaseInterval = attackCooltime;
			//フェーズを移行
			phase1 = BossFirstPhase::Idle;
			return;
		}

	}
	else {

		if (bossSwordPhase == BossSwordPhase::Create) {
			//敵中心から剣の位置の中心まで移動する(120f)
			//毎フレームランダムに魚群から魚を選び、選ばれた魚は10fで剣の中心まで移動する

			ImGui::Text("now create!");

			//最初にどの魚を剣まで移動させるか決める
			int goFishToSwordIndex = 0;
			goFishToSwordIndex = static_cast<int>(Random(0, fishes.size()));
			//重複対策で配列内を探索
			if (!choiceFishIndex.empty()) {
				for (int i = 0; i < choiceFishIndex.size(); i++) {
					//重複していたらもう一度ランダムに振り分ける
					if (choiceFishIndex[i] == goFishToSwordIndex) {
						goFishToSwordIndex = static_cast<int>(Random(0, fishes.size()));
						//for文最初からやり直し
						i = -1;
					}
				}
			}
			//配列に挿入
			choiceFishIndex.push_back(goFishToSwordIndex);
			//動的配列末尾の要素のイージングを時間10fで開始させる(順番に移動してもらうため)

			size_t indexSize = choiceFishIndex.size();
			size_t indexParam = choiceFishIndex[indexSize - 1];

			Vector3 pos;
			float randomParam = 10.0f;
			//移動を開始する魚の元の座標を取っておき、制御点もイイ感じに決める
			fishesBeforePos[indexSize - 1] = fishes[indexParam].pos.translation_;

			//選択された魚は親子関係をいったん切る
			fishes[indexParam].pos.parent_ = nullptr;
			fishesBeforePos[indexSize - 1] = fishParent.pos.matWorld_.transform(fishesBeforePos[indexSize - 1], fishParent.pos.matWorld_);

			//制御点1は始点から誤差x(今は5)のランダムな地点。制御点2は終点から誤差xにランダム
			pos = fishesBeforePos[indexSize - 1];
			fishesControllP1[indexSize - 1] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
			pos = swordPos;
			fishesAfterPos[indexSize - 1] = pos;
			fishesControllP2[indexSize - 1] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
			easePFishToSword[indexSize - 1].Start(60);

			//剣を徐々におおきくする
			Vector3 swordScale;
			easeSwordScale.Update();
			swordScale = Lerp({ 0,0,0 }, { 4,4,4 }, easeSwordScale.GetTimeRate());

			swordTransform.scale_ = swordScale;
			swordTransform.translation_ = swordPos;
			swordTransform.SetMatRot(CreateMatRot(fishParent.pos.translation_, targetPos));
		}
		//アニメーション時間が移動した(魚の数+最後に移動した魚の移動時間)より小さくなったなら次のモーション(攻撃開始座標への移動)を開始
		else if (bossSwordPhase == BossSwordPhase::Move) {

			ImGui::Text("now move!");
			Vector3 rotaVec;
			rotaVec.x = sin(PI / 3.0f);
			rotaVec.z = cos(PI / 3.0f);
			rotaVec.normalize();
			rotaVec *= distancePtoSword;

			Vector3 matRotX, matRotY, matRotZ;
			Vector3 up{ 0,1,0 };
			matRotZ = fishParent.pos.translation_ - targetPos;
			matRotZ.normalize();
			matRotX = up.cross(matRotZ);
			matRotX.normalize();
			matRotY = matRotZ.cross(matRotX);
			matRotY.normalize();

			Matrix4 matRot{
				matRotX.x,matRotX.y,matRotX.z,0,
				matRotY.x,matRotY.y,matRotY.z,0,
				matRotZ.x,matRotZ.y,matRotZ.z,0,
				0,0,0,1
			};


			//標的の座標と掛け算
			Vector3 aftetVec;
			aftetVec = matRot.transform(rotaVec, matRot) + targetPos;

			easeSwordPos.Update();
			Vector3 pos;
			pos = Lerp(swordPos, aftetVec, LerpConbertOut(easeSwordPos.GetTimeRate()));
			/*swordTransform.rotation_.z = easeSwordMove.GetTimeRate() * -(PI / 2.0f);
			swordTransform.rotation_.x = easeSwordMove.GetTimeRate() * -(PI / 2.0f);*/

			//魚親座標と標的のなす角
			float angleTargetoFish;
			angleTargetoFish = acos(targetPos.dot(fishParent.pos.translation_) / (targetPos.length() * fishParent.pos.translation_.length()));

			ImGui::Text("nasukaku:%f", angleTargetoFish / PI * 180);
			swordRotAngle = easeSwordPos.GetTimeRate() * (/* - (PI / 2.0f) +*/ angleTargetoFish);
			ImGui::Text("rotAngle:%f", swordRotAngle / PI * 180);

			Matrix4 fishMatRot = CreateMatRot(fishParent.pos.translation_, targetPos);

			Vector3 rot = { easeSwordPos.GetTimeRate() * -(PI / 2.0f),0,easeSwordPos.GetTimeRate() * -(PI / 2.0f) };
			rot = fishMatRot.transform({ PI / 2, 0, PI / 2 }, fishMatRot);
			//rot = swordTransform.matWorld_.transform( rot, CreateMatRot(fishParent.pos.translation_, targetPos));
			swordTransform.SetRot(rot);

			swordTransform.translation_ = pos;

		}

		else if (bossSwordPhase == BossSwordPhase::Attack) {

			ImGui::Text("now attack!");

			Vector3 beforePos, afterPos;

			//攻撃前と攻撃後ベクトルの基準となる軸
			Vector3 axis(targetPos);


			Vector3 vecBossToTarget = fishParent.pos.translation_ - targetPos;
			vecBossToTarget = { 1,1,1 };

			ImGui::Text("vecBoss2Target:%f,%f,%f", vecBossToTarget.x, vecBossToTarget.y, vecBossToTarget.z);
			axis = vecBossToTarget.cross({ 1,0,0 });
			axis.normalize();
			ImGui::Text("axis:%f,%f,%f", axis.x, axis.y, axis.z);
			//回転軸のクォータニオン作成
			Quaternion axisQ = { axis,PI / 3 };
			Quaternion posQ = (vecBossToTarget.x, vecBossToTarget.y, vecBossToTarget.z, 0);
			beforePos = axisQ.multiply(posQ.GetAxis());
			axisQ = { axis, -PI / 3 };
			afterPos = axisQ.multiply(posQ.GetAxis());

			//beforePos.normalize();
			beforePos *= distancePtoSword;
			//afterPos.normalize();
			afterPos *= distancePtoSword;


			ImGui::Text("before:%f,%f,%f", beforePos.x, beforePos.y, beforePos.z);
			ImGui::Text("after:%f,%f,%f", afterPos.x, afterPos.y, afterPos.z);

			Vector3 rotaVec, rotaVecAfter;
			rotaVec.x = sin(PI / 3.0f);
			rotaVec.z = cos(PI / 3.0f);
			rotaVec.normalize();
			rotaVec *= distancePtoSword;
			rotaVecAfter.x = -sin(PI / 3.0f);
			rotaVecAfter.x = -cos(PI / 3.0f);
			rotaVecAfter.normalize();
			rotaVecAfter *= distancePtoSword;

			Vector3 matRotX, matRotY, matRotZ;
			Vector3 up{ 0,1,0 };
			matRotZ = fishParent.pos.translation_ - targetPos;
			matRotZ.normalize();
			matRotX = up.cross(matRotZ);
			matRotX.normalize();
			matRotY = matRotZ.cross(matRotX);
			matRotY.normalize();

			Matrix4 matRot{
				matRotX.x,matRotX.y,matRotX.z,0,
				matRotY.x,matRotY.y,matRotY.z,0,
				matRotZ.x,matRotZ.y,matRotZ.z,0,
				0,0,0,1
			};

			beforePos = matRot.transform(rotaVec, matRot) + targetPos;
			afterPos = matRot.transform(rotaVecAfter, matRot) + targetPos;


			//イージング更新
			easeSwordPos.Update();
			Vector3 pos, rot;
			//ベジエ曲線で座標を補完
			pos = LerpBezireQuadratic(beforePos, targetPos, afterPos, LerpConbertInback(easeSwordPos.GetTimeRate()));

			ImGui::Text("timeRate:%f", easeSwordPos.GetTimeRate());

			//ワールド行列から回転を借りてくる
			rot = swordTransform.rotation_;
			//回転
			rot.x = swordRotAngle - (LerpConbertInback(easeSwordPos.GetTimeRate()) * PI / 3.0f);
			//剣の回転にボスの向きの回転行列を掛ける
		/*	rot = swordTransform.matWorld_.transform(rot, CreateMatRot(fishParent.pos.translation_,targetPos));
			rot.x += -(PI / 2.0f) - (LerpConbertInback(easeSwordPos.GetTimeRate()) * PI / 3.0f);


			swordTransform.SetRot(rot);*/
			swordTransform.translation_ = pos;
		}
		//崩壊モーション
		else if (bossSwordPhase == BossSwordPhase::Destroy) {

			//毎フレーム1匹ずつ動かす
			int fishIndex = swordBreakTime - nextPhaseInterval;
			if (fishIndex >= moveFishMax)fishIndex = moveFishMax - 1;
			ImGui::Text("fishIndex:%d", fishIndex);

			//毎フレーム別の魚の移動を開始する
			Vector3 pos;
			float randomParam = 10.0f;
			//前座標は自座標
			fishesBeforePos[fishIndex] = fishes[choiceFishIndex[fishIndex]].pos.translation_;
			fishesBeforePos[fishIndex] = swordTransform.translation_;//制御点1は自座標から誤差x

			pos = fishesBeforePos[fishIndex];
			fishesControllP1[fishIndex] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
			//移動後の座標は魚群の球につくようにする(魚の生成関数と同じようなもんを使う)
			float posY = Random(-fishParent.radius, fishParent.radius);
			fishes[choiceFishIndex[fishIndex]].radian = Random(0.0f, 360.0f);
			fishes[choiceFishIndex[fishIndex]].radius = sqrt(fishParent.radius * fishParent.radius - posY * posY);
			fish newFish = fishes[choiceFishIndex[fishIndex]];
			pos.x = sin(PI / 180.0f * fishes[choiceFishIndex[fishIndex]].radian) * fishes[choiceFishIndex[fishIndex]].radius;
			pos.z = cos(PI / 180.0f * fishes[choiceFishIndex[fishIndex]].radian) * fishes[choiceFishIndex[fishIndex]].radius;


			float plus = Random(-1.0f, 1.0f);
			float num = 1;
			if (plus < 0) {
				num = -1;
			}

			pos.y = (sqrt(fishParent.radius * fishParent.radius - newFish.radius * newFish.radius) * num);

			pos += newFish.displacement;
			fishesAfterPos[fishIndex] = pos;

			fishesAfterPos[fishIndex] = fishParent.pos.matWorld_.transform(fishesAfterPos[fishIndex], fishParent.pos.matWorld_);
			fishesControllP2[fishIndex] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
			easePFishToSword[fishIndex].Start(60);

			//剣を徐々におおきくする
			Vector3 swordScale;
			swordScale = Lerp({ 4,4,4 }, { 0,0,0 }, easeSwordScale.GetTimeRate());
			easeSwordScale.Update();
			swordTransform.scale_ = swordScale;

		}

		//タイマー制御
		nextPhaseInterval--;

	}

	//イージングによるスケールと座標の制御
	for (int i = 0; i < choiceFishIndex.size(); i++) {
		easePFishToSword[i].Update();

		fishes[choiceFishIndex[i]].pos.translation_ = LerpBezireCubic(fishesBeforePos[i], fishesControllP1[i], fishesControllP2[i], fishesAfterPos[i], easePFishToSword[i].GetTimeRate());

		if (easePFishToSword[i].GetActive()) {
			fishes[choiceFishIndex[i]].pos.scale_ = Lerp(beforeScale, afterScale, easePFishToSword[i].GetTimeRate());
		}
		fishes[choiceFishIndex[i]].pos.TransferMatrix();
	}

	//剣の当たり判定座標の更新
	SwordColCubeUpdate();

	ImGui::SliderFloat("rotaX", &swordTransform.rotation_.x, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaY", &swordTransform.rotation_.y, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaZ", &swordTransform.rotation_.z, 0.0f, 360.0f);
	ImGui::Text("ParentPosZ:%1.5f", fishParent.pos.translation_.z);
	ImGui::Text("swordPosX:%1.5f", swordTransform.translation_.x);
	ImGui::Text("swordPosY:%1.5f", swordTransform.translation_.y);
	ImGui::Text("swordPosZ:%1.5f", swordTransform.translation_.z);

	//剣のワールド行列
	swordTransform.TransferMatrix();


}

void Boss::UpdateAtkRush()
{
	const int rushMaxTime = 60;		//突進攻撃の始点から終点までかかる時間
	const int rushCoolTime = 30;	//次の突進攻撃までのクールタイム
	const int fishesDispersionRate = 15;

	easeParentPos.Update();

	ImGui::Text("fish[0].translation:%f,%f,%f", fishes[0].pos.translation_.x, fishes[0].pos.translation_.x, fishes[0].pos.translation_.x);



	if (easeParentPos.GetActive()) {
		//突進中は始点と終点でイージング
		Vector3 pos = Lerp(parentBeforePos, parentAfterPos, easeParentPos.GetTimeRate());
		Vector3 pBeforePos = fishesBeforePos[0];
		Vector3 pAfterPos = fishesAfterPos[0];
		ImGui::Text("before:%1.3f,%1.3f,%1.3f", pBeforePos.x, pBeforePos.y, pBeforePos.z);
		ImGui::Text("after:%1.3f,%1.3f,%1.3f", pAfterPos.x, pAfterPos.y, pAfterPos.z);

		fishParent.pos.translation_ = pos;
		fishParent.pos.TransferMatrix();

	}
	else {
		//突進のクールタイムが終わっている
		if (nextPhaseInterval <= 0) {
			//突進の回数が残っている
			if (rushCount > 0) {

				//突進回数を減らし、クールタイムを設定しておき、挙動を開始
				rushCount--;
				nextPhaseInterval = rushCoolTime;

				//���̐e����W�I�܂ł̃x�N�g��
				Vector3 vecfishTotarget = fishParent.pos.translation_ - targetPos;

				//親座標の始点と終点を決める
				parentBeforePos = fishParent.pos.translation_;
				parentAfterPos = parentBeforePos - (vecfishTotarget * 2);
				parentAfterPos.y = parentBeforePos.y;

				float len = vecfishTotarget.length();
				//挙動に使う座標を設定
				for (int i = 0; i < fishes.size(); i++) {

					if (i % fishesDispersionRate == 0) {
						int easeParam = i / fishesDispersionRate;
						easePFishToSword[easeParam].Start(rushMaxTime - (i / fishesDispersionRate) /*+ (Random(-(rushMaxTime / 10.0f),rushMaxTime / 10.0f))*/);
					}

					//移動を親依存にはするけど、親に追従じゃ動かないので親子関係を切る
					fishes[i].pos.parent_ = nullptr;

					if (rushCount == rushMaxCount - 1) {
						fishesBeforePos[i] = parentBeforePos + fishes[i].pos.translation_;
						fishesAfterPos[i] = parentAfterPos + fishes[i].pos.translation_;
					}
					else {
						fishesBeforePos[i] = fishesAfterPos[i];
						fishesAfterPos[i] -= (vecfishTotarget * 2);
					}


					if (i == 9) {
						easeParentPos.Start(rushMaxTime);
					}

					/*	fishesAfterPos[i].x = fishesBeforePos[i].x - (vecfishTotarget.x * 2);
						fishesAfterPos[i].z = fishesBeforePos[i].z - (vecfishTotarget.z * 2);
						fishesAfterPos[i].y = fishesBeforePos[i].y;*/
				}
			}
			else {
				//突進の回数が残っていないならモーション終わり
				nextPhaseInterval = attackCooltime;
				phase1 = BossFirstPhase::Idle;
				for (int i = 0; i < fishes.size(); i++) {
					fishes[i].pos.parent_ = &fishParent.pos;
				}

			}
		}
		else {
			//突進のクールタイムが終わっていないならタイム減らす
			nextPhaseInterval--;
		}
	}



	//親座標の移動処理
	if (easeParentPos.GetActive()) {
		//突進中は始点と終点でイージング
		Vector3 pos = Lerp(parentBeforePos, parentAfterPos, easeParentPos.GetTimeRate());
		Vector3 pBeforePos = fishesBeforePos[0];
		Vector3 pAfterPos = fishesAfterPos[0];
		ImGui::Text("before:%1.3f,%1.3f,%1.3f", pBeforePos.x, pBeforePos.y, pBeforePos.z);
		ImGui::Text("after:%1.3f,%1.3f,%1.3f", pAfterPos.x, pAfterPos.y, pAfterPos.z);
		fishParent.pos.translation_ = pos;
		fishParent.pos.TransferMatrix();
	}


	//���Q�̈ړ�����
	for (int i = 0; i < fishes.size(); i++) {
		//��ԍŏ��̋��̋������n�܂�Ƃ��ɋ��ƕW�I�̋������狛�̔z�񏇔Ԃ�ύX����
		if (easePFishToSword[0].GetTimeRate() == 0) {
			SortFishMin();
		}




		easePFishToSword[i / fishesDispersionRate].Update();
		if (easePFishToSword[i / fishesDispersionRate].GetActive()) {

			Vector3 fishPos = Lerp(fishesBeforePos[i], fishesAfterPos[i], easePFishToSword[i / fishesDispersionRate].GetTimeRate());
			fishes[i].pos.translation_ = fishPos;
			fishes[i].pos.TransferMatrix();
		}
	}

}

void Boss::UpdateBeginMotion()
{
	//魚群の中心(真ん中)の座標更新
	fishParent.pos.TransferMatrix();

	//魚1匹ずつの更新
	for (int i = 0; i < fishes.size(); i++) {

		//魚のラジアン(球の周回軌道)を加算
		fishes[i].radian += fishes[i].spd;
		if (fishes[i].radian > 360.0f) {
			fishes[i].radian -= 360.0f;
			fishes[i].spd = Random(0.0f, randSpdParam);
			fishes[i].spd = Random(randSpdParam / 2.0f, randSpdParam);
			//fishes[i].spd = Random(randSpdParam, randSpdParam * 2);
		}


		//回転で動かす座標は回転の中心である親座標
		Vector3 vec = fishParent.pos.translation_;
		//回転軸用のベクトルを生成
		Vector3 baseVec = fishes[i].randomVec * fishes[i].radius;
		baseVec.normalize();

		//回転軸、座標を表すクォータニオンの生成
		Quaternion baseQ = { baseVec,fishes[i].radian * PI / 180.0f };
		Quaternion posQ = { vec.x,vec.y,vec.z,0 };

		//回転
		Vector3 pos = baseQ.multiply(posQ.GetAxis());

		//座標に半径をかけて整える(回転後の座標は正規化されている)
		pos *= fishes[i].radius;

		//ワールド行列に送る
		fishes[i].pos.translation_ = pos;
		fishes[i].pos.TransferMatrix();

	}

	//攻撃のクールタイムを減らす
	nextPhaseInterval--;
	if (nextPhaseInterval == 0) {
		//フェーズの変更とクールタイム再設定
		phase1 = BossFirstPhase::Atk_Sword;
		//nextPhaseInterval = atkSwordMotionTime;

		//剣の大きさ、回転を初期化
		swordTransform.scale_ = { 0,0,0 };
		swordTransform.SetRot({ 0,0,0 });
		swordTransform.TransferMatrix();
	}
}

void Boss::FishLookFront(Vector3 pos, Vector3 dirVec, int fishNum)
{
	Vector3 up{ 0,1,0 };
	Vector3 z = dirVec - pos;
	z.normalize();
	Vector3 x = up.cross(z);
	x.normalize();
	Vector3 y = z.cross(x);
	y.normalize();
	Matrix4 rotaMat{
		x.x, x.y, x.z,0,
		y.x, y.y, y.z,0,
		z.x, z.y, z.z,0,
		0,0,0,1
	};

	fishes[fishNum].pos.matWorld_ *= rotaMat;

}


void Boss::SwordColCubeUpdate()
{

	//剣の回転情報で座標を移動
	Matrix4 matRot;
	matRot.identity();
	matRot *= swordTransform.quaternion.Rotate();

	posSwordColCube1 = { swordSizeX1,swordSizeY1,swordSizeZ1 };

	posSwordColCube1 = matRot.transform(posSwordColCube1, swordTransform.matWorld_);
	//posSwordColCube1 *= 4.0f;
	//posSwordColCube1 += swordTransform.translation_;

	posSwordColCube2 = { swordSizeX2,swordSizeY2,swordSizeZ2 };


	posSwordColCube2 = matRot.transform(posSwordColCube2, swordTransform.matWorld_);
	/*posSwordColCube2 *= 4.0f;
	posSwordColCube2 += swordTransform.translation_;*/
}

void Boss::SortFishMin()
{
	Vector3 vecFishToTarget;
	//�傫���𒲂ׂ�
	for (int i = 0; i < fishes.size(); i++) {
		vecFishToTarget = fishes[i].pos.translation_ - targetPos;
		lenTargetToFishes[i] = vecFishToTarget.length();

	}

	float swapLen;
	fish swapFish;

	//�傫����g��ď��������ɕ�ׂ�
	for (int i = 0; i < fishes.size() - 1; i++) {
		for (int j = i + 1; j < fishes.size(); j++) {
			if (lenTargetToFishes[i] < lenTargetToFishes[j]) {
				swapLen = lenTargetToFishes[i];
				lenTargetToFishes[i] = lenTargetToFishes[j];
				lenTargetToFishes[j] = swapLen;
				swapFish = fishes[i];
				fishes[i] = fishes[j];
				fishes[j] = swapFish;
			}
		}
	}
}

void Boss::FishDirectionUpdate()
{
	//毎フレームの最初に魚を向かせる
	Vector3 parentPos = fishParent.pos.translation_;
	Vector3 up{ 0.0f,1.0f,0.0f };
	Vector3 vecX, vecY, vecZ;
	vecZ = targetPos - parentPos;
	vecZ.normalize();
	vecX = up.cross(vecZ);
	vecX.normalize();
	vecY = vecZ.cross(vecX);
	vecY.normalize();
	Matrix4 dirMat{
		vecX.x,vecX.y,vecX.z,0,
		vecY.x,vecY.y,vecY.z,0,
		vecZ.x,vecZ.y,vecZ.z,0,
		0,0,0,1
	};

	fishParent.pos.SetMatRot(dirMat);
}

void Boss::Damage(int atk) {
	if (damageTimer > 0) {
		return;
	}

	bossHealth -= atk;
	damageTimer = nextDamageInterval;
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
}

float Random(float num1, float num2)
{
	//引数から小さい方とおおきい方を分ける
	float min, max;
	min = num2;
	max = num1;
	if (num1 < num2) {
		min = num1;
		max = num2;
	}
	//乱数シード生成器
	std::random_device seedGem;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seedGem());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(min, max);

	//ランダムな値を生成して返す
	return dist(engine);
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
{
	return start * (1.0f - t) + end * t;
}

Vector3 LerpBezireCubic(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t)
{
	Vector3 p1, p2, p3, p4, p5, result;
	p1 = Lerp(start, contRollP1, t);
	p2 = Lerp(contRollP1, contRollP2, t);
	p3 = Lerp(contRollP2, end, t);
	p4 = Lerp(p1, p2, t);
	p5 = Lerp(p2, p3, t);
	result = Lerp(p4, p5, t);

	return result;
}

Vector3 LerpBezireQuadratic(const Vector3& start, const Vector3& contRollP, const Vector3& end, float t)
{
	Vector3 p1, p2, p3;
	p1 = Lerp(start, contRollP, t);
	p2 = Lerp(contRollP, end, t);
	p3 = Lerp(p1, p2, t);

	return p3;
}

float LerpConbertInback(float t)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return c3 * t * t * t - c1 * t * t;
}

float LerpConbertOut(float t)
{
	return 1 - pow(1 - t, 5);
}

bool IsPercent(float param)
{
	return Random(0.0f, 100.0f) < param;
}

Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target)
{
	Vector3 matRotX, matRotY, matRotZ;
	Vector3 up{ 0,1,0 };

	matRotZ = target - pos;
	matRotZ.normalize();
	matRotX = up.cross(matRotZ);
	matRotX.normalize();
	matRotY = matRotZ.cross(matRotX);
	matRotY.normalize();

	Matrix4 matRot{
				matRotX.x,matRotX.y,matRotX.z,0,
				matRotY.x,matRotY.y,matRotY.z,0,
				matRotZ.x,matRotZ.y,matRotZ.z,0,
				0,0,0,1
	};

	return matRot;
}






