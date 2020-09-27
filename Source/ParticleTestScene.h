#pragma once

#include "GameMain.h"
#include "SceneBase.h"

class ParticleTestScene : public SceneBase
{


public:
	ParticleTestScene();
	~ParticleTestScene();

	SceneBase* Update();
	void       Draw();

private:

	class Particle*  mParticle;
	class Shader*    mParticleShader;
	class Grid* mGrid;
	class Texture*   mTexture;

	Matrix4 mView;
	Matrix4 mProjection;

	Matrix4 mBillboardMat;


};