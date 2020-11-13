//
// ���}�b�v�I�u�W�F�N�g�R���|�[�l���g
//
#pragma once
#include "Component.h"
#include <vector>

class EnvironmentMapComponent : public Component
{

public:

	EnvironmentMapComponent(class Actor* in_owner);     // �R���X�g���N�^
	~EnvironmentMapComponent();                         // �f�X�g���N�^

	void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }

	// ���}�b�v�I�u�W�F�N�g�̕`�揈��
	void DrawEnvironmentMap(class Shader* in_envShader);



private:

	bool m_isVisible;

	class Mesh* m_mesh;

};