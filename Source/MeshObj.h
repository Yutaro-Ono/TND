#pragma once
#include "Mesh.h"

class MeshObj : public Mesh
{

public:

	MeshObj();
	~MeshObj();

	// obj�t�@�C���̃��[�h �� ���_�z��쐬 �� �o�^
	bool Load(const std::string& in_filePath, class Renderer* in_renderer) override;


};