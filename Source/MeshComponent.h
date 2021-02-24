//-----------------------------------------------------------------------+
// ���b�V���R���|�[�l���g�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"

class MeshComponent : public Component
{
public:

	MeshComponent(class Actor* in_owner, bool in_isSkeletal = false);       // �R���X�g���N�^(�e�A�N�^�[���K�v)
	~MeshComponent();                                                       // �f�X�g���N�^

	virtual void Draw(class Shader* in_shader);                             // ���b�V���R���|�[�l���g�̕`��
	virtual void DrawShadow(class Shader* in_shader);                       // �V���h�E�����p�̕`��
	void DrawMap(class Shader* in_shader);                                  // �}�b�vHUD�`��p

	virtual void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }         // ���b�V���R���|�[�l���g�Ŏg�p���郁�b�V���̃Z�b�g

	void SetTexturesToUniform(class Shader* in_shader);                     // �V�F�[�_uniform�Ƀe�N�X�`�����Z�b�g����֐�
	void SetTextureIndex(size_t in_index) { m_textureIndex = in_index; }    // �e�N�X�`���̃Z�b�g

	void SetVisible(bool in_visible) { m_visible = in_visible; }            // �\���t���O�̃Z�b�^�[
	bool GetVisible() const { return m_visible; }                           // �\���t���O�̃Q�b�^�[

	bool GetIsSkeletal() const { return m_isSkeletal; }                     // �X�P���^�����ǂ����̃Q�b�^�[

	void SetIntensityVal(float in_val) { m_intensity = in_val; }            // �P�x���x�̃Z�b�^�[

	void SetDrawMap(bool in_flg) { m_isDrawMap = in_flg; }
	void SetMapColor(const Vector3& in_color) { m_mapColor = in_color; }    // �}�b�v�`��p�̃J���[�Z�b�^�[

protected:

	class Mesh* m_mesh;                                                     // ���b�V���I�u�W�F�N�g�̊i�[�p
	size_t m_textureIndex;                                                  // �e�N�X�`���[�̃C���f�b�N�X

	bool m_visible;                                                         // �\���t���O
	bool m_isDrawMap;                                                       // �}�b�v��ʂɕ\�����邩
	bool m_isSkeletal;                                                      // �X�P���^�����f�����ǂ���

	float m_intensity;                                                      // �P�x�̋��x��� (�G�~�b�V�u�}�b�v��K�p���Ă��郁�b�V���̂�)

	Vector3 m_mapColor;                                                     // �}�b�v�`�掞�̃J���[���
};