//-----------------------------------------------------------------------+
// 天使像オブジェクト.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"


class AngelStatue : public Actor
{


public:

	AngelStatue();
	~AngelStatue();


	void SetMesh(class Mesh* in_mesh);

	void UpdateActor(float in_deltaTime) override;


private:


	class MeshComponent* m_meshComp;


};