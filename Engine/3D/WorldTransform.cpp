#include "WorldTransform.h"
#include "DirectXCore.h"
#include <cassert>
#include <Quaternion.h>

void WorldTransform::Initialize() {
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer() {

	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

}

void WorldTransform::Map() {

	//定数バッファのマッピング
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

}

void WorldTransform::TransferMatrix() {

	Matrix4 matScale, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = MyMath::Scale(scale_);
	
	//オイラー角で回転しているなら回転用のマトリックスを初期化
	if (isEuler) {
		matRot = MyMath::Initialize();
		//matRot *= MyMath::Rotation(rotation_, 6);
		matRot = quaternion.Rotate();
	}

	matTrans = MyMath::Translation(translation_);

	//ワールド行列の合成
	matWorld_ = MyMath::Initialize();//変形をリセット
	matWorld_ *= matScale;//ワールド行列にスケーリングを反映
	matWorld_ *= matRot;//ワールド行列に回転を反映
	matWorld_ *= matTrans;//ワールド行列に平行移動を反映
	
	look = GetLook(matRot, Vector3(-1,0,0));
	lookBack = GetLook(matRot, Vector3(1, 0, 0));
	lookRight = GetLook(matRot, Vector3(0, 0, 1));
	lookLeft = GetLook(matRot, Vector3(0, 0, -1));

	//親オブジェクトがあれば
	if (parent_) {
		//親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}
	//定数バッファへのデータ転送
	constMap->matWorld = matWorld_;

	constMap->alpha = alpha;

}


void WorldTransform::SetRot(const Vector3& rot)
{
	isEuler = true;
	rotation_ = rot;

	quaternion.SeTEuler(rotation_);
}

void WorldTransform::SetMatRot(const Matrix4& mat)
{
	//オイラー角の回転フラグをfalseに
	isEuler = false;
	matRot = mat;
}

void WorldTransform::MoveRot(const Vector3& move)
{
	rotation_ += move;

	quaternion.SeTEuler(rotation_);
}

void WorldTransform::SetQuater(const Quaternion& quater)
{
	quaternion = quater;

	rotation_ = quaternion.GetEulerAngles();
}

void WorldTransform::MoveQuater(const Quaternion& move)
{
	quaternion += move;

	rotation_ = quaternion.GetEulerAngles();
}

Quaternion& WorldTransform::GetQuaternion()
{
	return quaternion;
}

Vector3 WorldTransform::GetLook(Matrix4 matRot,Vector3 at)
{
	Vector3 Pos = MyMath::GetWorldTransform(matWorld_);
	Vector3 look_ = MyMath::MatVector(matRot, at);

	return Pos + look_;
}

