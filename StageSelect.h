#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "LoadLevelEditor.h"
#include "GameCamera.h"
#include "Player.h"

// タイトルシーン
class StageSelect : public BaseScene
{
public: // メンバ関数

	StageSelect() = default;
	~StageSelect() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

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

private:
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション

	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<Player> player_;
	std::unique_ptr<GameCamera> gameCamera;

	uint32_t loserTexture_ = 0;
};