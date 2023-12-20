#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "LoadLevelEditor.h"
#include "GameCamera.h"
#include "Player.h"
#include "CollisionManager.h"
#include "Skydome.h"


// ステージセレクトシーン
class StageSelect : public BaseScene
{
public: // メンバ関数

	StageSelect();
	~StageSelect();

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

private:
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Sprite> StageSelectSprite_ = nullptr;
	std::unique_ptr<Sprite> OneToOneSprite_ = nullptr;
	std::unique_ptr<Sprite> OneToTwoSprite_ = nullptr;
	std::unique_ptr<Sprite> SelectBarSprite_ = nullptr;
	std::unique_ptr<Sprite> NowLoadingSprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<Sprite> PushSprite;
	std::unique_ptr<Sprite> SpaceSprite;
	std::unique_ptr<Sprite> ASprite;

	bool isFinishGame = false;
	bool isBlackoutStart = false;
	bool isSelectBarTop = true;
	bool isSelectBarDown = true;
	bool isTop = false;
	bool isNext = false;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 5;

	uint32_t loserTexture_ = 0;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;
	float SpriteAlpha = 1.0f;
	float NowLoadingAlpha = 0.0f;

	Vector2 center = { 0.5f,0.5f };
	Vector2 OneToOnePos = {300.0f,145.0f};
	Vector2 OneToTwoPos = { 300.0f,180.0f };
	Vector2 SelectBarPos = { 300.0f,145.0f };
	Vector2 PushSpritePos = { 617,609 };
	Vector2 PushSpriteSize = { 45,22 };
	Vector2 SpaceSpritePos = { 673,609 };
	Vector2 SpaceSpriteSize = { 55,30 };
	Vector2 ASpriteSize = { 30,30 };
};