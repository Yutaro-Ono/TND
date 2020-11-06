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

	// ���}�b�v�I�u�W�F�N�g�̕`�揈��
	void DrawEnvironmentMap(std::vector<class MeshComponent*> in_envMeshes);

	void SetIsVisible(bool in_visible) { m_isVisible = in_visible; }


private:


	class Texture* m_texture;

	bool m_isVisible;                // �`�悷�邩���Ȃ����̃t���O

};