#include "LevelObject.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshComponent.h"



LevelObject::LevelObject(Mesh * in_mesh)
{
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);
}

LevelObject::~LevelObject()
{
}
