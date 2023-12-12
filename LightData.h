#pragma once
#include <vector>
#include "LightGroup.h"
#include <memory>
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightData
{

public://基本関数

	// シングルトンインスタンスの取得
	static LightData* GetInstance();
	// 終了処理
	void Destroy();

	//初期化
	void Initialize();
	//更新
	void Update();

private://メンバ関数

public://Setter

	/// <summary>
	/// ポイントライト
	/// </summary>
	/// <param name="lightpos_">ポジション</param>
	/// <param name="lightcolor_">カラー</param>
	/// <param name="lightAtten_">減衰係数</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	uint32_t AddPointLight(const Vector3& lightpos_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_ = true);
	/// <summary>
	/// スポットライト
	/// </summary>
	/// <param name="lightpos_">ポジション</param>
	/// <param name="lightdir_">ライトの向いている方向</param>
	/// <param name="lightcolor_">色</param>
	/// <param name="lightAtten_">減衰係数</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	uint32_t AddSpotLight(const Vector3& lightpos_,const Vector4& lightdir_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_ = true);
	/// <summary>
	/// 丸影
	/// </summary>
	/// <param name="casterPos_">キャスターポジション</param>
	/// <param name="dir_">ライトの距離</param>
	/// <param name="atten_">距離減衰係数</param>
	/// <param name="factorAngle_">減衰角度</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	uint32_t AddCircleShadow(const Vector3& casterPos_,const Vector4& dir_,const Vector3& atten_,const Vector2& factorAngle_,const bool& active_ = true);

public://Getter

private://クラス関連
	std::unique_ptr<LightGroup> lightGroup;

	static LightData* LightData_;
private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数
	LightData() = default;
	~LightData();
	LightData(const LightData&) = delete;
	const LightData& operator=(const LightData&) = delete;
	

};