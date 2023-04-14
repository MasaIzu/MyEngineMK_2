#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>
#include "Quaternion.h"

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
	float alpha=1;       // アルファ
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	//任意軸のローカル回転
	Quaternion quaternion = { 0,0,0,0 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	//アルファ値
	float alpha = 1;
	//その物体の向いている方向
	Vector3 look = { 0,0,0 };
	Vector3 lookBack = { 0,0,0 };
	Vector3 lookRight = { 0,0,0 };
	Vector3 lookLeft = { 0,0,0 };
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	Matrix4 matRot;

	//回転がオイラー角による回転か
	bool isEuler = false;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void SetRot(const Vector3& rot);

	void SetMatRot(const Matrix4& mat);

	void MoveRot(const Vector3& move);

	void SetQuater(const Quaternion& quater);

	void MoveQuater(const Quaternion& move);
	
	Quaternion& GetQuaternion();
	
	Vector3 GetLook(Matrix4 matRot,Vector3 at);
};