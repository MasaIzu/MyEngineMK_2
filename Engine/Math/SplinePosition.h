#pragma once
#include <Vector3.h>
#include <memory>
#include <vector>

class SplinePosition {

public://��{�֐�
	/// <summary>
	/// ��Ō��߂��
	/// </summary>
	SplinePosition();

	/// <summary>
	/// Vector3��4�����o�[�W����(vector�^�œn����������)
	/// </summary>
	/// <param name="Start">�X�^�[�g�n�_</param>
	/// <param name="p1">����_����1</param>
	/// <param name="p2">����_����2</param>
	/// <param name="end">�S�[���n�_</param>
	SplinePosition(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end);

	/// <summary>
	/// �X�^�[�g�n�_,����_,�S�[���n�_���D���Ɍ��߂����
	/// </summary>
	/// <param name="points">points{ start, start, p1, p2, p3, end, end };�݂�����p3��ǉ�������</param>
	SplinePosition(const std::vector<Vector3>& points);
	~SplinePosition();

	/// <summary>
	/// �X�v���C���Ȑ��A�b�v�f�[�g
	/// </summary>
	/// <param name="time">�f�t�HMaxTime=1�̂����̂ǂ���</param>
	void Update(float& time);

	/// <summary>
	/// �X�v���C���Ȑ��A�b�v�f�[�g4�ł�����Ver
	/// </summary>
	/// <param name="time">�f�t�HMaxTime=1�̂����̂ǂ���</param>
	void Update(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end, float& time);

	/// <summary>
	/// �X�v���C���Ȑ��A�b�v�f�[�g5�ł�����Ver(5�܂łȂ��ł��ł�����)
	/// </summary>
	/// <param name="time">�f�t�HMaxTime=1�̂����̂ǂ���</param>
	void Update(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& p3, Vector3& end, float& time);

	/// <summary>
	/// �X�v���C���Ȑ��A�b�v�f�[�gpoints���A�v�fVer
	/// </summary>
	/// <param name="time">�f�t�HMaxTime=1�̂����̂ǂ���</param>
	void Update(const std::vector<Vector3>& points, float& time);

	/// <summary>
	/// �X�^�[�g�ɖ߂�Ver
	/// </summary>
	void Reset();

	/// <summary>
	/// �ׂ������Z�b�g�����߂���Ver
	/// </summary>
	/// <param name="ResetIndex">Index�̓X�^�[�g�ɖ߂������ꍇ��1�ɂ���(0�͖���)</param>
	/// <param name="time">MaxTime=1��time��0.5�Ȃ甼������n�܂�</param>
	void Reset(const size_t& ResetIndex, float& time);

private://�v���C�x�[�g�֐�
	Vector3 SplinePositionUpdate(const std::vector<Vector3>& points, size_t& startIndex, float& t);

public://Setter
	void SetSplineMaxTime(const float& MaxTime) { this->MaxTime = MaxTime; }

public://Getter
	bool GetFinishSpline()const { return isFinishSpline; }

public://���J�ϐ�
	Vector3 NowPos;

private://�N���X�ϐ�
	bool isFinishSpline = false;
	size_t startIndex = 1;
	float timeRate_ = 0.0f;
	float MaxTime = 1.0f;
	std::vector<Vector3> points{};

};