#pragma once
#include "CameraComponent.h"


class ThirdPersonCamera : public CameraComponent
{
public:

	ThirdPersonCamera(class Actor* in_owner);
	~ThirdPersonCamera();

	void Update(float in_deltaTime) override;           // �X�V����

	void ProcessInput(float in_deltaTime) override;     // ���͏���

	// �����蔻�菈��
	void CollisionFix(class BoxCollider* in_hitCameraBox, class BoxCollider* in_hitBox);


	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	// �s�b�`�̃Q�b�^�[
	float GetPitch() const { return m_pitch; }
	// ���[�̃Q�b�^�[
	float GetYaw() const { return m_yaw; }

	// �s�b�`���x�̃Z�b�g
	void SetPitchSpeed(float in_speed) { m_pitchSpeed = in_speed; }
	// �s�b�`�̍ő呬�x�̃Z�b�g
	void SetMaxPitch(float in_maxSpeed) { m_maxPitch = in_maxSpeed; }

	// �J���������̃Z�b�^�[
	void SetDistance(float in_dist);

	// �O�i�x�N�g�����I�[�i�[�ƍ��킹�邩�ǂ����̃Z�b�^�[
	void SetAdjustForward(bool in_adjust) { m_adjustForward = in_adjust; }
	// �J�����Ǐ]�������s�����ǂ����̃Z�b�^�[
	void SetChaseOwnerForward(bool in_chase) { m_chaseOwnerForward = in_chase; }

	// �J�����O�i�x�N�g���̃Q�b�^�[
	const Vector3& GetForward() { return m_forwardVec; }

private:

	// �I�[�i�[�̌�������苗���ɃJ�����ʒu�𒲐����A���̍��W��Ԃ�
	const Vector3& ComputeCameraPos() const;

	Vector3 m_offset;        // �O�l�̎��_���^�[�Q�b�g����̋����I�t�Z�b�g

	Vector3 m_forwardVec;      // �J�����̑O�i�x�N�g��
	Vector3 m_upVec;           // �J�����̏���x�N�g��

	// �J���������x
	Vector3 m_velocity;

	// �I�[�i�[�A�N�^�̈ړ����Ɏg�p����O�i�x�N�g��(�J�����)
	Vector3 m_ownerForward;

	// �s�b�`�֘A
	float m_pitch;
	float m_pitchSpeed;
	float m_maxPitch;

	// ���[���x(/�b)
	float m_yaw;

	// �^�[�Q�b�g�܂ł̋���
	float m_distance;

	// �J�����̑O�i�x�N�g���ƃI�[�i�[�A�N�^�̌��������킹�邩 (�f�t�H���g�FON)
	bool m_adjustForward;
	// �J�����̑O�i�x�N�g�����I�[�i�[�A�N�^�̑O�i�x�N�g���֒Ǐ]�����邩 (�f�t�H���g�FOFF)
	bool m_chaseOwnerForward;

	// �����蔻��
	class BoxCollider* m_hitBox;
	class Actor* m_cameraActor;

	// �}�E�X����
	Vector2 m_mousePos;                    // �}�E�X�̍��W
	Vector2 m_frameMousePos;               // �}�E�X�̑O�t���[���̍��W

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ŒZ����
	static const float MAX_TARGET_DISTANCE;        // �^�[�Q�b�g�܂ł̍ő勗��
};