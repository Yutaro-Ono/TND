#pragma once
#include <vector>
#include "Math.h"

class MiniMapHUD
{

public:

	MiniMapHUD(class PlayerManager* in_target);
	~MiniMapHUD();

	void WriteBuffer(class Shader* in_shader, std::vector<class MeshComponent*> in_mesh);

	void Draw(class Shader* in_shader);


private:

	void CreateFBO(unsigned int& in_fbo);

	unsigned int m_miniMapFBO;
	unsigned int m_mapBuffer;
	unsigned int m_rbo;

	class Actor* m_target;                 // �^�[�Q�b�g
	class Texture* m_mapTex;               // �}�b�v�e�N�X�`�� (�~�`)

	class LandMarkIndicator* m_landMark;

	int m_width;
	int m_height;
	float m_scale;                         // �}�b�vUI�X�P�[��

	Vector3 m_viewPos;                     // �r���[���W (�������N�_�Ƀ}�b�v�`��)

	Vector2 m_screenPos;                   // �X�N���[����̍��W

	Matrix4 m_projection;
	//Matrix4 m_lowerViewProj;             // �������̃r���[&�v���W�F�N�V���������s��

};