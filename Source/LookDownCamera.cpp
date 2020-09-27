#include "LookDownCamera.h"

LookDownCamera::LookDownCamera(Actor * in_targetActor)
	:Camera(in_targetActor)
{
	m_cameraHeight = 130;
	m_cameraBack = 250;
}

LookDownCamera::~LookDownCamera()
{
}

void LookDownCamera::UpdateActor(float in_deltaTime)
{

	// 接近係数
	const float dumper = 1.5f;

	// 注視点とキャラ座標の差
	Vector3 diff;
	Vector3 playerPos = m_targetActor->GetPosition();

	// 差分ベクトルからプレイヤー位置まで近付ける
	diff = playerPos - m_viewTarget;
	diff = dumper * in_deltaTime * diff;

	// 注視点・カメラ位置をセット
	m_viewTarget = m_viewTarget + diff;
	m_position = m_viewTarget;
	m_position.x -= m_cameraBack;
	m_position.z = m_cameraHeight;

}
