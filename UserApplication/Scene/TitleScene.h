#pragma once
#include "BaseScene.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Input.h"
#include "SceneManager.h"
#include "MeshParticle.h"
#include "ViewProjection.h"
#include "GameCamera.h"
#include "Model.h"
#include <LoadLevelEditor.h>
#include "Player.h"
#include <MiddleBossEnemy.h>

// タイトルシーン
class TitleScene : public BaseScene
{
public: // メンバ関数

	TitleScene() = default;
	~TitleScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ポストエフェクトかける
	/// </summary>
	void PostEffectDraw() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	//コピー処理
	void CopyData() override;

	//CSアップデート
	void CSUpdate() override;

	/// <summary>
	/// ブレイクするか
	/// </summary>
	/// <returns>ブレイク</returns>
	bool IsBreak() override;

	//カメラアップデート
	void CameraUpdate();

private:
	enum class MovieCameraCount
	{
		FirstCamera,
		SecondCamera,
	};

private:
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Sprite> TitleSprite_ = nullptr;
	std::unique_ptr<Sprite> StartSprite_ = nullptr;
	std::unique_ptr<Sprite> EndSprite_ = nullptr;
	std::unique_ptr<Sprite> OutLineSprite_ = nullptr;

	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model_;
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<LoadLevelEditor> levelData_;
	std::list<BulletShotEnemy*> bulletShotEnemy;
	std::list<TutorialEnemy*> tutorialEnemyList;
	std::unique_ptr<Player> player_;
	WorldTransform DebugTrans;
	std::unique_ptr<MiddleBossEnemy> middleBossEnemy;

	MovieCameraCount movieCameraCount = MovieCameraCount::FirstCamera;

	bool isBlackout = false;
	bool isStart = false;
	bool isFinishGame = false;
	bool isStartChangeScene = false;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 5;

	uint32_t MaxFream = 60;
	uint32_t MaxParticleCount = 1000000;
	uint32_t CameraPosCount = 1;
	uint32_t BlackoutPreliminaryTime = 0;
	uint32_t MaxBlackoutPreliminaryTime = 20;
	uint32_t EasingStartTime = 14;
	uint32_t EasingTime = 14;
	uint32_t MaxEasingTime = 100;
	uint32_t MiddleEnemyBossMakeBulletTime = 48;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;
	float lerpTime = 0.02f;
	float MovieCameraSpeed = 1.0f;
	float SpriteAlpha = 0.0f;
	float OutLinePos = 500.0f;
	float OutLinePosition1 = 500.0f;
	float OutLinePosition2 = 550.0f;

	Vector2 center = { 0.5f,0.5f };

	Vector3 CameraPos;
	Vector3 CameraVelocity;

	Vector3 EnemyStartPos;
	Vector3 EnemyEndPos;
	Vector3 EasingStartPos;


	std::vector<Vector3> FirstCameraPoints{};
	std::vector<Vector3> SecondCameraPoints{};

};

