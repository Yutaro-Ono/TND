//------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X(���[���h��ԗp��UI)
//
//------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <string>

class WorldSpaceUI
{

public:

	WorldSpaceUI(const Vector3& in_pos, const std::string& in_filePath, float in_scale = 1.0f);
	~WorldSpaceUI();


	void SetPosition(const Vector3& in_pos) { m_position = in_pos; }
	int GetTextureID() { return m_textureID; }

	void Draw(class Shader* in_shader);

	// ���W�̃Z�b�g
	void SetUIPosition(const Vector3& in_pos);

	// �r���{�[�h�s����`�E�擾
	Matrix4 GetBillboardMatrix();

	// �\����\���t���O�̃Z�b�^�[
	void SetVisible(bool in_visible) { m_isVisible = in_visible; }

private:


	Vector3 m_position;            // ���W

	float m_scale;                 // �g�嗦

	class Texture* m_texture;      // �e�N�X�`��
	int m_textureID;               // �e�N�X�`��ID

	bool m_isVisible;              // �\��(false), ��\��(true)�t���O

	// �r���{�[�h�s�� (�J�����ƕ��s�ɂȂ�悤�ɔz�u���邽�߂̍s��)
	static Matrix4 m_staticBillboardMat;


};