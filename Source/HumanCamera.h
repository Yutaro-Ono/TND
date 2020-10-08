// �l�ԑ���p�J����(��l�́E�O�l�̃n�C�u���b�h)
#pragma once
#include "CameraComponent.h"

class HumanCamera : public CameraComponent
{


public:

	HumanCamera(class PlayerHuman* in_owner);
	~HumanCamera();

	void Update(float in_deltaTime) override;

	void ProcessInput(float in_deltaTime) override;



	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	// �s�b�`�̃Z�b�^�[�E�Q�b�^�[
	float GetPitch() const { return m_pitch; }
	void SetPitch(float in_pitch) { m_pitch = in_pitch; }
	// �s�b�`���x�̃Z�b�g
	void SetPitchSpeed(float in_speed) { m_pitchSpeed = in_speed; }
	// �s�b�`�̍ő呬�x�̃Z�b�g
	void SetMaxPitch(float in_maxSpeed) { m_maxPitch = in_maxSpeed; }
	// ���[�̃Z�b�^�[�E�Q�b�^�[
	float GetYaw() const { return m_yaw; }
	void SetYaw(float in_yaw) { m_yaw = in_yaw; }

private:

	class PlayerHuman* m_player;

	Vector3 m_position;       // �J�����̍��W
	Vector3 m_offset;        // �O�l�̎��_���^�[�Q�b�g����̋����I�t�Z�b�g
	Vector3 m_upVec;           // �J�����̏���x�N�g��

	// �J���������x
	Vector3 m_velocity;

	// �I�[�i�[�A�N�^�̈ړ����Ɏg�p����O�i�x�N�g��(�J�����)
	Vector3 m_playerForward;

	// �s�b�`�֘A
	float m_pitch;
	float m_pitchSpeed;
	float m_maxPitch;

	// ���[���x(/�b)
	float m_yaw;

	// �^�[�Q�b�g�܂ł̋���
	float m_distance;


	// �}�E�X
	Vector2 m_mousePos;                    // �}�E�X�̍��W
	Vector2 m_frameMousePos;               // �}�E�X�̑O�t���[���̍��W

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ŒZ����
	static const float MAX_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ő勗��

};