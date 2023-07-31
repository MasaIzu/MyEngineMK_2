#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <assert.h>
#include "ViewProjection.h"
#include "affin.h"

/// <summary>
/// �V��
/// </summary>
class Skydome {

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_;

};