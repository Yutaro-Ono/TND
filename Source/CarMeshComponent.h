#pragma once
#include "Component.h"

class CarMeshComponent : public Component
{

public:

	CarMeshComponent(class Actor* in_owner);
	~CarMeshComponent();


	virtual void Draw(class Shader* in_shader);                             // ���b�V���R���|�[�l���g�̕`��
	virtual void DrawShadow(class Shader* in_shader);                       // �V���h�E�����p�̕`��

	virtual void SetMesh(class Mesh* in_mesh) { m_mesh = in_mesh; }         // ���b�V���R���|�[�l���g�Ŏg�p���郁�b�V���̃Z�b�g

	void SetTextureIndex(size_t in_index) { m_textureIndex = in_index; }    // �e�N�X�`���̃Z�b�g

	void SetReflect(bool in_ref) { m_reflect = in_ref; }                    // ���ˌv�Z�̗L��

	void SetVisible(bool in_visible) { m_visible = in_visible; }            // �\���t���O�̃Z�b�^�[
	bool GetVisible() const { return m_visible; }                           // �\���t���O�̃Q�b�^�[



protected:

	class Mesh* m_mesh;                                                     // ���b�V���I�u�W�F�N�g�̊i�[�p
	size_t m_textureIndex;                                                  // �e�N�X�`���[�̃C���f�b�N�X

	bool m_reflect;                                                         // ���ˌv�Z���s����

	bool m_visible;                                                         // �\���t���O
};