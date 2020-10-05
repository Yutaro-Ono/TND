// �l�ԑ���p�J����(��l�́E�O�l�̃n�C�u���b�h)
#pragma once
#include "CameraComponent.h"

class HumanCamera : public CameraComponent
{


public:

	HumanCamera(class PlayerHuman* in_owner);
	~HumanCamera();

	void Update(float in_deltaTime) override;


	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	// �s�b�`���x�̃Z�b�g
	void SetPitchSpeed(float in_speed) { m_pitchSpeed = in_speed; }
	// �s�b�`�̍ő呬�x�̃Z�b�g
	void SetMaxPitch(float in_maxSpeed) { m_maxPitch = in_maxSpeed; }

private:

	Vector3 m_offset;        // �O�l�̎��_���^�[�Q�b�g����̋����I�t�Z�b�g

	// �s�b�`�֘A
	float m_pitch;
	float m_pitchSpeed;
	float m_maxPitch;
};