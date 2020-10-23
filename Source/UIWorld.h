//------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X(���[���h��ԗp��UI)
//
//------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <string>

class UIWorld
{

public:

	UIWorld(const Vector3& in_pos, const std::string& in_filePath, float in_scale = 1.0f);
	~UIWorld();


	void SetPosition(const Vector3& in_pos) { m_position = in_pos; }
	int GetTextureID() { return m_textureID; }

	void Draw(class Shader* in_shader);

private:


	Vector3 m_position;            // ���W

	float m_scale;                 // �g�嗦

	class Texture* m_texture;      // �e�N�X�`��
	int m_textureID;               // �e�N�X�`��ID

	bool m_isVisible;              // �\��(false), ��\��(true)�t���O

	static Matrix4 m_staticBillboardMat;
	static Vector3 m_cameraWorldPos;


};