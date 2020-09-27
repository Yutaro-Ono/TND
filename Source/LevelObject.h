#pragma once
#include "Actor.h"


class LevelObject : public Actor
{


public:

	LevelObject(class Mesh* in_mesh);
	~LevelObject();


private:


	class MeshComponent* m_meshComp;

};