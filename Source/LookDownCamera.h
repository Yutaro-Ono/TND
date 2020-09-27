//-----------------------------------------------------------------------+
// å©â∫ÇÎÇµéãì_ÉJÉÅÉâ
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Camera.h"

class LookDownCamera : public Camera
{
public:

	LookDownCamera(Actor *in_targetActor);
	~LookDownCamera();

	void UpdateActor(float in_deltaTime) override;

private:

	float m_cameraHeight;
	float m_cameraBack;
};