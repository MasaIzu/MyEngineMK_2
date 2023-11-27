#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "LoadLevelEditor.h"
#include "GameCamera.h"
#include "Player.h"
#include "CollisionManager.h"
#include "BossEnemy.h"
#include "Skydome.h"
#include "MiddleBossEnemy.h"
#include "ClearUI.h"

// 中ボスシーン
class MediumBossStage : public BaseScene
{
public: // メンバ関数

	MediumBossStage();
	~MediumBossStage();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//ポストエフェクト
	void PostEffectDraw() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	//CSアップデート
	void CSUpdate() override;

	//ブレイクするか
	bool IsBreak() override;

	//レティクル内に敵がいたらロックオンする
	void LockOn();


private:
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	CollisionManager* collisionManager = nullptr;//当たり判定
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<Player> player_;
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<MiddleBossEnemy> middleBossEnemy;
	std::unique_ptr<ClearUI> clearUI;

	bool VsBoss = false;
	bool isLockOn = false;
	bool isFinishGame = false;
	bool isBlackoutStart = false;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 5;

	uint32_t loserTexture_ = 0;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;
	float SpriteAlpha = 1.0f;
	float AddSpriteAlpha = 0.007f;
	float DownSpriteAlpha = 0.01f;

	Vector2 center = { 0.5f,0.5f };

	Vector3 start = Vector3(0,50,-1000);
	Vector3 end = Vector3(0,50,150);
};