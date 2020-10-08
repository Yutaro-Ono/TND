#pragma once
#include "Mesh.h"

class MeshObj : public Mesh
{

public:

	MeshObj();
	~MeshObj();

	// objファイルのロード → 頂点配列作成 → 登録
	bool Load(const std::string& in_filePath, class Renderer* in_renderer) override;


	// タンジェントスペース計算用関数
	void calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);
	void getPosVec(Vector3& destPos, const std::vector<float> fv, int index);
	void getUVVec(Vector2& destUV, const std::vector<float> fv, int index);
	void setTangent(std::vector<float>& destfv, int index, const Vector3& tangent);
};