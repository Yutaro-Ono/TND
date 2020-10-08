#pragma once
#include "Mesh.h"

class MeshObj : public Mesh
{

public:

	MeshObj();
	~MeshObj();

	// obj�t�@�C���̃��[�h �� ���_�z��쐬 �� �o�^
	bool Load(const std::string& in_filePath, class Renderer* in_renderer) override;


	// �^���W�F���g�X�y�[�X�v�Z�p�֐�
	void calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);
	void getPosVec(Vector3& destPos, const std::vector<float> fv, int index);
	void getUVVec(Vector2& destUV, const std::vector<float> fv, int index);
	void setTangent(std::vector<float>& destfv, int index, const Vector3& tangent);
};