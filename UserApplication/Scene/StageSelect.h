#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "LoadLevelEditor.h"
#include "GameCamera.h"
#include "Player.h"
#include "CollisionManager.h"
#include "Skydome.h"

// �^�C�g���V�[��
class StageSelect : public BaseScene
{
public: // �����o�֐�

	StageSelect();
	~StageSelect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	void PostEffectDraw() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

	//�R�s�[����
	void CopyData() override;

	//CS�A�b�v�f�[�g
	void CSUpdate() override;

private:
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//�r���[�v���W�F�N�V����
	CollisionManager* collisionManager = nullptr;//�����蔻��
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<Player> player_;
	std::unique_ptr<GameCamera> gameCamera;
	std::unique_ptr<Skydome> skydome;

	int shadeNumber = 3;
	int range = 0;//�ڂ������x
	int samples = 5;

	uint32_t loserTexture_ = 0;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;

	Vector2 center = { 0.5f,0.5f };
};