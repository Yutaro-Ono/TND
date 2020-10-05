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

	const Matrix4& GetViewMatrix() const { return GAME_INSTANCE.GetViewMatrix(); }

protected:

	void SetViewMatrix(const Matrix4& in_view);
};