#pragma once
#include "Mesh.h"

class MeshGpmesh : public Mesh
{

public:

	MeshGpmesh();
	~MeshGpmesh();

	// gpmeshファイルのロード → 頂点配列作成 → 登録
	bool Load(const std::string& in_filePath, class Renderer* in_renderer) override;

};