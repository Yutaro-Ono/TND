#pragma once
#include "Component.h"
#include "Math.h"

class LightGlassComponent : public Component
{
public:

	LightGlassComponent(class Actor* in_owner);
	~LightGlassComponent();

	// ���b�V���̃Z�b�g
	void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }

	// �`�揈��
	void Draw(class Shader* in_shader);


	//----------------------------------------------+
	// Setter / Getter
	//----------------------------------------------+
	void SetLightColor(const Vector3& in_color) { m_lightColor = in_color; }         // ���C�g�J���[�̃Z�b�g
	void SetLightLuminance(float in_luminance) { m_luminance = in_luminance; }       // ���C�g�P�x�̃Z�b�g
	void SetVisible(bool in_visible) { m_isVisible = in_visible; }

private:

	class Mesh* m_mesh;

	Vector3 m_lightColor;           // ���C�g�J���[
	float m_luminance;              // ���C�g�̋P�x

	bool m_isVisible;               // �`�悷�邩���Ȃ���


};