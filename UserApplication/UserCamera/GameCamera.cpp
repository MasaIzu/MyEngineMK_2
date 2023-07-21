#include "GameCamera.h"
#include <windef.h>
#include "WinApp.h"
#include "MyMath.h"
#include"ImGuiManager.h"
#include <SphereCollider.h>
#include <CollisionManager.h>
#include <CollisionAttribute.h>

GameCamera::GameCamera(uint32_t window_width, uint32_t window_height)
{

	input_ = Input::GetInstance();

	easing_ = new Easing();

	winWidth = window_width;
	winHeight = window_height;

	// 画面サイズに対する相対的なスケールに調整
	scaleX_ = 1.0f / (float)window_width;
	scaleY_ = 1.0f / (float)window_height;

	MaxCameraTime = 400;
	cameraTime = MaxCameraTime;
	oldMousePos = mousePos;
	mousePos = input_->GetMousePos();

	cameraPos = { 5,5,5 };
}

GameCamera::~GameCamera()
{
	delete easing_;
}

void GameCamera::Initialize(ViewProjection* viewProjection_, const float& cameraAngle, const Vector3& pos) {


	viewProjection = viewProjection_;
	playerPos_ = pos;
	InitializeCameraPosition(cameraAngle);

	// コリジョンマネージャに追加
	CameraCollider = new SphereCollider(Vector4(0, CameraCollisionRadius, 0, 0), CameraCollisionRadius);
	CollisionManager::GetInstance()->AddCollider(CameraCollider);
	CameraCollider->SetAttribute(COLLISION_ATTR_CAMERA);
	CameraCollider->Update(MyMath::Translation(viewProjection->eye));
}

void GameCamera::InitializeCameraPosition(const float& cameraAngle)
{
	windowWH = Uint32Vector2(winWidth / 2, winHeight / 2);

	mouseMoved = Vector2(0, cameraAngle);
	CameraRot = MyMath::MakeIdentity();


	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	uint32_t xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	uint32_t yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	target = playerPos_ + cameraHigh;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	//レールカメラの回転を反映
	forward = MyMath::MatVector(CameraRot, forward);
	forward.normalize();

	//target = pos;
	vTargetEye = target + (forward * cameraDistance_);
	cameraPos = vTargetEye;

	//距離
	//cameraPos += PlayerMoveMent;
	Vector3 dVec = vTargetEye - cameraPos;
	dVec *= cameraDelay;
	cameraPos += viewProjection->eye;
	Vector3 player_camera = cameraPos - target;
	player_camera.normalize();
	cameraPos = target + (player_camera * cameraDistance_);

	viewProjection->target = target;
	viewProjection->eye = vTargetEye;
	viewProjection->UpdateMatrix();
}

void GameCamera::Update() {
	if (input_->TriggerKey(DIK_F1)) {
		if (cameraMode == false) {
			cameraMode = true;
		}
		else {
			cameraMode = false;
		}
	}
	if (cameraMode == false) {
		/*if (input_->PushKey(DIK_LSHIFT)) {
			PlayerLockOnCamera(viewProjection_);
		}
		else {
			PlaySceneCamera(viewProjection_);
		}*/

		if (input_->PushKey(DIK_UP)) {
			mouseMoved.x += 0.001f;
		}
		if (input_->PushKey(DIK_DOWN)) {
			mouseMoved.x -= 0.001f;
		}


		PlaySceneCamera();



		ImGui::Begin("camera");
		ImGui::Text("eye:%f", viewProjection->eye.x);
		ImGui::Text("eye:%f", viewProjection->eye.y);
		ImGui::Text("eye:%f", viewProjection->eye.z);

		ImGui::Text("mouseMoved:%f", mouseMoved.x);
		ImGui::Text("mouseMoved:%f", mouseMoved.y);
		ImGui::Text("PlayerToCameraVecDistance:%f", PlayerToCameraVecDistance);
		ImGui::Text("AngleX:%f", MyMath::GetRadAngle(mouseMoved.y));
		ImGui::Text("AngleY:%f", MyMath::GetRadAngle(mouseMoved.x));
		MyMath::MatrixText(CameraRot);

		ImGui::Text("HowMachMovePointer:%d", HowMachMovePointer);

		ImGui::End();
	}
	else {
		ImGui::Begin("camera");
		ImGui::SliderFloat("eye:x", &vTargetEye.x, -100.0f, 100.0f);
		ImGui::SliderFloat("eye:y", &vTargetEye.y, -100.0f, 700.0f);
		ImGui::SliderFloat("eye:z", &vTargetEye.z, -100.0f, 100.0f);

		ImGui::SliderFloat("target:x", &target.x, -100.0f, 100.0f);
		ImGui::SliderFloat("target:y", &target.y, -100.0f, 100.0f);
		ImGui::SliderFloat("target:z", &target.z, -100.0f, 100.0f);

		ImGui::End();
	}

	viewProjection->target = target;
	viewProjection->eye = vTargetEye;

	//オブジェクトが挟まったらオブジェクトの前にカメラを持ってくる
	CheckBetweenToCameraCollider();

	CameraCollider->Update(MyMath::Translation(viewProjection->eye));

	if (CameraCollider->GetSphereMeshHit()) {
		CameraCollider->SphereMeshHitReset();
	}

	viewProjection->UpdateMatrix();


}

void GameCamera::PlaySceneCamera() {

	oldCameraPos = vTargetEye;
	if (cameraTime < MaxCameraTime) {
		cameraTime++;
	}
	if (shakeTime > 0) {
		shakeTime--;
	}
	else {
		isShake = false;
	}

	//カメラの回転ベクトル
	Vector3 rotat = { 0, 0, 0 };
	//カメラの移動の速さ
	const float cameraSpeed = 0.0005f;

	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	uint32_t xPos_absolute, yPos_absolute;

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	//マウスの移動量を取得
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(static_cast<float>(mousePosition.y), static_cast<float>(mousePosition.x)) - Uint32Vector2(windowWH.y, windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)
	Mous_UP_DOWN = Vector2(MouseMove.x, MouseMove.y) / (318.5f + kand);
	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / (318.5f + kand);
	HowMachMovePointer += static_cast<uint32_t>(MouseMove.y);
	//どっち向きに移動したのか
	if (Mous_UP_DOWN.x > 0) {
		cameraUp = true;
		cameraDown = false;
	}
	else if (Mous_UP_DOWN.x < 0) {
		cameraUp = false;
		cameraDown = true;
	}
	else {
		cameraUp = false;
		cameraDown = false;
	}

	//カメラ制限
	if (mouseMoved.x < -0.80f) {
		mouseMoved.x = -0.80f;
	}
	else if (mouseMoved.x > 1.30f) {
		mouseMoved.x = 1.30f;
	}
	
	//if (cameraDown == true) {
	//	if (mouseMoved.x < -0.10f + CameraMouseMoved) {
	//		mouseMoved.x = -0.10f + CameraMouseMoved;
	//		if (cameraDis - 10.0f > CameraDistanceMinus) {
	//			CameraDistanceMinus -= Mous_UP_DOWN.x * 40;
	//			if (CameraMouseMoved > -0.2f) {
	//				CameraMouseMoved += Mous_UP_DOWN.x;
	//			}
	//			if (cameraDis - 10.0f < CameraDistanceMinus) {
	//				CameraDistanceMinus = cameraDis - 10.0f;
	//				CameraMouseMoved = -0.2f;
	//			}

	//		}
	//		else {
	//			CameraMouseMoved = -0.2f;
	//		}
	//	}
	//	else {
	//	}
	//}
	//if (cameraUp == true) {

	//	if (mouseMoved.x < -0.10f) {
	//		mouseMoved.x = -0.10f + CameraMouseMoved;
	//	}

	//	if (CameraDistanceMinus > 0) {
	//		CameraDistanceMinus -= Mous_UP_DOWN.x * 60;
	//		if (CameraMouseMoved < 0.0f) {
	//			CameraMouseMoved += Mous_UP_DOWN.x / 2;
	//		}
	//		if (CameraDistanceMinus < 0) {
	//			CameraDistanceMinus = 0;
	//		}
	//	}
	//	else {
	//		CameraMouseMoved = 0;
	//	}

	//	if (mouseMoved.x > 1.30f) {
	//		mouseMoved.x = 1.30f;
	//	}
	//}


	Vector3 rotation = Vector3(-mouseMoved.x, mouseMoved.y, 0);

	Matrix4 cameraRot;

	cameraRot = MyMath::Rotation(rotation, 6);

	rot = rotation;
	CameraRot = cameraRot;

	target = playerPos_ + cameraHigh;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	//レールカメラの回転を反映
	forward = MyMath::MatVector(CameraRot, forward);

	forward.normalize();

	//target = pos;
	vTargetEye = target + (forward * cameraDistance_);

	CameraAngle(vTargetEye.z - target.z, vTargetEye.x - target.x);


	////遅延カメラ
	////距離
	//cameraPos += PlayerMoveMent;
	//Vector3 dVec = vTargetEye - cameraPos;
	//dVec *= cameraDelay;
	//cameraPos += dVec * cameraSpeed_;
	//Vector3 player_camera = cameraPos - target;
	//player_camera.normalize();

	//float DISTANCE = cameraDis - CameraDistanceMinus;

	//player_camera.x = player_camera.x * DISTANCE;
	//player_camera.y = player_camera.y * (cameraDis - CameraDistanceMinus / 2);
	//player_camera.z = player_camera.z * DISTANCE;

	//cameraPos = target + (player_camera);


}

void GameCamera::Collision()
{
	isShake = true;
	shakeTime = 10;
}

void GameCamera::MousePositionReset()
{
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	uint32_t xPos_absolute, yPos_absolute;

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる
	
}

bool GameCamera::CheckBetweenToCameraCollider()
{
	bool isGroundHit = false;

	// 範囲レイキャスト
	Ray GroundRay;
	GroundRay.start = MyMath::Vec3ToVec4(playerPos_);
	//LookRay.start.y += CameraRayCollisionRadius;
	PlayerToCameraVec = (viewProjection->eye - Vector3(0, CameraCollisionRadius, 0)) - playerPos_;
	PlayerToCameraVecDistance = PlayerToCameraVec.length();
	GroundRay.dir = MyMath::Vec3ToVec4(PlayerToCameraVec.norm());
	RaycastHit raycastHit;

	//カメラとの間に地面があれば位置を変える
	if (CollisionManager::GetInstance()->Raycast(GroundRay, COLLISION_ATTR_LANDSHAPE, &raycastHit, PlayerToCameraVecDistance)) {
		Vector3 eyeToInter = viewProjection->eye - MyMath::Vec4ToVec3(raycastHit.inter);
		viewProjection->target = viewProjection->target - eyeToInter;
		viewProjection->eye = MyMath::Vec4ToVec3(raycastHit.inter) + Vector3(0, CameraCollisionRadius, 0);
		PlayerToCameraVec = viewProjection->eye - playerPos_;
		PlayerToCameraVecDistance = PlayerToCameraVec.length();
		isGroundHit = true;
	}

	// 範囲レイキャスト
	Ray LookRay;
	LookRay.start = MyMath::Vec3ToVec4(playerPos_);
	//LookRay.start.y += CameraRayCollisionRadius;
	PlayerToCameraVec = viewProjection->eye - playerPos_;
	PlayerToCameraVecDistance = PlayerToCameraVec.length();
	LookRay.dir = MyMath::Vec3ToVec4(PlayerToCameraVec.norm());
	RaycastHit ObjectRaycastHit;

	//カメラとの間にオブジェクトがあれば位置を変える
	if (CollisionManager::GetInstance()->Raycast(LookRay, COLLISION_ATTR_OBJECT, &ObjectRaycastHit, PlayerToCameraVecDistance)) {
		Vector3 eyeToInter = viewProjection->eye - MyMath::Vec4ToVec3(ObjectRaycastHit.inter);
		viewProjection->target = viewProjection->target - eyeToInter;
		viewProjection->eye = MyMath::Vec4ToVec3(ObjectRaycastHit.inter) + Vector3(0, CameraCollisionRadius, 0);
		PlayerToCameraVec = viewProjection->eye - playerPos_;
		PlayerToCameraVecDistance = PlayerToCameraVec.length();
		isGroundHit = true;
	}

	return isGroundHit;
}

Vector3 GameCamera::GetEyeToTagetVecDistance(const float& distance) const
{
	Vector3 eyeToTargetVec = viewProjection->target - viewProjection->eye;
	eyeToTargetVec.normalize();

	return eyeToTargetVec * distance + viewProjection->eye;
}


void GameCamera::CameraAngle(const float& x, const float& z)
{
	angle = atan2(x, z);

	if (angle < 0) {
		angle = angle + 2 * MyMath::PI;
	}

	angle = floor(angle * 360 / (2 * MyMath::PI));

}
