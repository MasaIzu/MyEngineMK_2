#pragma once
#include "BaseScene.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"

// �^�C�g���V�[��
class ClearScene : public BaseScene
{
public: // �����o�֐�

	ClearScene() = default;
	~ClearScene() = default;

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
	Input* input = nullptr;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	uint32_t loserTexture_ = 0;
};

