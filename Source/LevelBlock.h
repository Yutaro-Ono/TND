//-----------------------------------------------------------------------+
// �X�e�[�W�p�u���b�N�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class LevelBlock : public Actor
{


public:

	LevelBlock();                            // �R���X�g���N�^
	~LevelBlock();                           // �f�X�g���N�^

	void SetMesh(class Mesh* in_mesh);       // ���b�V���̃Z�b�g


private:

	class MeshComponent* m_meshComp;
	class BoxCollider* m_box;
};