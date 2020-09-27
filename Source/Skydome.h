//-----------------------------------------------------------------------+
// �X�J�C�h�[���N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// �C���N���[�h�t�@�C��
#include "Actor.h"


class Skydome : public Actor
{


public:

	Skydome();                                             // �R���X�g���N�^
	~Skydome();                                            // �f�X�g���N�^

	void SetMesh(class Mesh* in_mesh);                     // ���b�V���̃Z�b�^�[

	void UpdateActor(float in_deltaTime) override;         // �X�V����

	void SetSpin() { m_isSpin = true; }                    // ��]����


private:

	float m_radian;

	class MeshComponent* m_meshComp;

	bool m_isSpin;


};