#pragma once
#include <vector>


class MiniMapHUD
{

public:

	MiniMapHUD(class GameWorld* in_world);
	~MiniMapHUD();

	void WriteBuffer(std::vector<class MeshComponent*> in_mesh);

	


private:

	void CreateFBO(unsigned int& in_fbo);

	unsigned int m_miniMapFBO;
	unsigned int m_mapTex;
	unsigned int m_rbo;

	Matrix4 m_lowerViewProj;    // 下向きのビュー&プロジェクション合成行列

};