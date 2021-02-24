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

	void SetLuminance(float in_lum) { m_luminance = in_lum; }
	void SetAlpha(float in_alpha) { m_alpha = in_alpha; }

private:

	bool m_isVisible;

	float m_luminance;       // �P�x
	float m_alpha;           // ���ߓx

	class Mesh* m_mesh;

};