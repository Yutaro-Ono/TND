#pragma once
#include "Mesh.h"

class MeshGpmesh : public Mesh
{

public:

	MeshGpmesh();
	~MeshGpmesh();

	// gpmesh�t�@�C���̃��[�h �� ���_�z��쐬 �� �o�^
	bool Load(const std::string& in_filePath, class Renderer* in_renderer) override;

};