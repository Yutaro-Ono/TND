#pragma once
#include "Component.h"
#include "Math.h"
#include "GameMain.h"

class CameraComponent : public Component
{
public:

	CameraComponent(class Actor* in_target, int updateOrder = 200);
	~CameraComponent();


	void SetActive() { GAME_INSTANCE.SetCamera(this); }


protected:

	void SetViewMatrix(const Matrix4& in_view);
};