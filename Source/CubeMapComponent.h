#pragma once
#include "MeshComponent.h"
#include <string>
#include <vector>
class CubeMapComponent : public Component
{

public:

	CubeMapComponent(class Actor* in_owner);
	~CubeMapComponent();

	void CreateTexture(const std::string& in_filePath);

	void Draw(class Shader* in_shader);

	class Texture* GetSkyBoxTexture() { return m_texture; }

	
	void SetIsVisible(bool in_visible) { m_isVisible = in_visible; }         // �L���[�u��\�����邩���Ȃ����̃Z�b�g
	void SetLuminance(float in_luminance) { m_luminance = in_luminance; }    // �P�x���̃Z�b�g


private:

	class Texture* m_texture;

	float m_luminance;               // �P�x���

	bool m_isVisible;                // �`�悷�邩���Ȃ����̃t���O

};