//-----------------------------------------------------------------------+
// �Ƃ����΂̐C��N���X(���U���g��ʂŎg�p).
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"


class FlameStatue : public Actor
{


public:

	FlameStatue();                                           // �R���X�g���N�^
	~FlameStatue();                                          // �f�X�g���N�^

	void SetMesh(class Mesh* in_mesh);                       // ���b�V���̃Z�b�g

	void UpdateActor(float in_deltaTime) override;           // �X�V����


private:

	class MeshComponent* m_meshComp;


};