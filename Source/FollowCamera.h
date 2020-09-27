//-----------------------------------------------------------------------+
// �ǔ��J����.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{


public:

	FollowCamera(class Actor* in_target);                    // �R���X�g���N�^

	void Update(float in_deltaTime) override;          // �J�����X�V(�I�[�o�[���C�h)

	void SnapToIdeal();                                     // ���z�̈ʒu�Ɍ��݈ʒu�������Ă���

	// Setter
	void SetHorizonDist(float in_dist) { m_horizonDist = in_dist; }
	void SetVerticalDist(float in_dist) { m_verticalDist = in_dist; }
	void SetTargetDist(float in_dist) { m_targetDist = in_dist; }
	void SetSpringConstant(float in_spring) { m_springConstant = in_spring; }


private:

	Vector3 ComputeCameraPos() const;


	Vector3 m_actualPos;          // ���ۂ̃J�����ʒu

	Vector3 m_velocity;           // actualPos�̉����x

	float m_horizonDist;          // ������������
	float m_verticalDist;         // ������������
	float m_targetDist;           // �^�[�Q�b�g�܂ł̋���

	float m_springConstant;       


};