#pragma once
#include "Component.h"
#include "Math.h"
#include "GameMain.h"

class CameraComponent : public Component
{
public:

	CameraComponent(class Actor* in_target, int updateOrder = 200);
	~CameraComponent();

	void ChangeProjectionMat(float in_fov, float in_near, float in_far);

	void SetActive() { GAME_INSTANCE.SetCamera(this); }

	const Vector3 GetPosition() const { return m_position; }

	const Matrix4& GetViewMatrix() const { return GAME_INSTANCE.GetViewMatrix(); }

protected:

	void SetViewMatrix(const Matrix4& in_view);


	Vector3 m_position;       // ÉJÉÅÉâÇÃç¿ïW
};