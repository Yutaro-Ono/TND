//-----------------------------------------------------------------------+
// �v���C���[�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

#include <vector>
#include <unordered_map>


class Player : public Actor
{


public:

	enum AnimState
	{
		IDLE,
		SPIN,
		CHARGE
	};

	enum MoveState
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	Player(bool in_isTitle);
	~Player();

	void UpdateActor(float in_deltaTime) override;

	void CollisionFix(class BoxCollider *in_hitPlayerBox, class BoxCollider *in_hitBox);

	void UpdateMovement(float in_deltaTime);          // �v���C���[�̈ړ�����(�A�N�Z���E�u���[�L�E��])

	void UpdateSpeedRank();         // ���x���Ƃ̃����N (�������ɃX�R�A�̔{����ύX)

	void SpinTitle(float in_deltaTime);               // �^�C�g����ʂ̉�]����

	// �ǉ�����
	void ComputeWorldTransform2();

	// Setter / Getter
	void SetAccelLimit(const float& in_limitVal) { m_accelLimit += in_limitVal; }           // �A�N�Z���ő�l�̃Z�b�^�[
	void SetBrakeLimit(const float& in_limitVal) { m_brakeLimit = in_limitVal; }            // �u���[�L�ő�l�̃Z�b�^�[

	MoveState GetMoveState() { return m_moveState; }
	void SetMoveState(MoveState in_state) { m_moveState = in_state; }                       // �ړ���ԃX�e�[�g�Z�b�^�[

	// �`�F�C���n�֐�
	void AddFlameChain();                                                                   // �`�F�C���̉��Z
	void SubFlameChain();                                                                   // �`�F�C���̌��Z����
	void RemoveFlameChain() { m_flameChain = 0; }                                           // �`�F�C���̏I��
	const int& GetFlameChain() const { return m_flameChain; }                               // �`�F�C���̃Q�b�^�[
	const float& GetChainTimer() const { return m_chainTimer; }                             // �`�F�C���^�C�}�[�̃Q�b�^�[
	const float& GetChainTimerLimit() const { return CHAIN_TIMER; }                         // �`�F�C���^�C�}�[�̋K��l�̃Q�b�^�[

	const float& GetAccel() const { return m_accel; }                                       // �A�N�Z���l(���x)�̃Q�b�^�[
	const float& GetAccelLimit() const { return m_accelLimit; }                             // �A�N�Z���l(�������x)�̃Q�b�^�[
	const float& GetAccelMax() const { return ACCEL_LIMIT_MAX; }                            // �A�N�Z���l(�Q�[�����ł̍ō����x)�̃Q�b�^�[
	const float& GetBrake() const { return m_brake; }                                       // �u���[�L�l(���x)�̃Q�b�^�[
	const int& GetSpeedRank() const { return m_maxSpeedRank; }                              // �X�s�[�h�����N�̃Q�b�^�[
	
	void AllStopSound();

	void InitRadian() { m_radian = 0.0f; }                                                  // Z����]�l�̏�����
	void InitStatus();                                                                      // �v���C���[�̃X�e�[�^�X������(��Ƀt���[���[�h�Ŏ擾�������̏�����)

	void SetTomosuFire();
	void SetTitlePlayer();

private:

	class MeshComponent* m_meshComp;

	// �����蔻��{�b�N�X
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;

	// �Ƃ�����
	class FlameActor* m_flame;
	std::vector<class FlameActor*> m_tomosuFire;


	// ����
	class SandSmoke* m_smoke;

	// �Ή�
	class FlameSpark* m_spark;

	// �X�e�[�g
	AnimState m_animState;                                                                  // �A�j���[�V�����X�e�[�g
	MoveState m_moveState;                                                                  // �ړ��X�e�[�g(�A�N�Z���E�o�b�N�E�ҋ@)

	bool m_isJump;
	bool m_onGround;

	// �A�N�Z���E�u���[�L�����x
	float m_accel;
	float m_brake;
	// �n���h�u���[�L
	float m_handBrake;
	// �A�N�Z���E�u���[�L���E�l
	float m_accelLimit;
	float m_brakeLimit;

	// �ō����x�̃����N(���Q�[�W�ɔ��f)
	int m_maxSpeedRank;
	// ���`�F�C���V�X�e���p(������)
	int m_flameChain;
	// ���`�F�C���V�X�e���p�^�C���J�E���^�[
	float m_chainTimer;


	// ��]�p
	float m_radian;                                                                          // Z��
	float m_spinRadian;                                                                      // Y��

	float m_spinAngle;                                                                       // ��]���[�V�����̉�]�l

	bool m_isTitle;                                                                          // �^�C�g���p���ǂ���

	Vector3 m_jumpVec;
	Vector3 m_velocityVec;

	static const int CHAIN_LIMIT;                                                            // �`�F�C�����̌��E
	static const int RANK_MAX;                                                               // �����N�̍ő�l
	static const float CHAIN_TIMER;                                                          // �`�F�C�����^�C�}�[
	static const float HAND_BRAKE_VALUE;                                                     // �n���h�u���[�L�̌��������鑬�x
	static const float ACCEL_LIMIT_MAX;                                                      // �����x�̃��~�b�g

    std::unordered_map<std::string, std::string> m_sound;                                    // �v���C���[�֘A�̌��ʉ�

};