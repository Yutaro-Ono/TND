//-----------------------------------------------------------------------+
// �Ƃ�����(�t�B�[���h�I�u�W�F�N�g) �N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class Player;

class FlameActor : public Actor
{


public:

	FlameActor();         // �R���X�g���N�^
	~FlameActor();        // �f�X�g���N�^

	void Delete();

	void UpdateActor(float in_deltaTime) override;

	void SetSphereCollider(Vector3 in_pos, float in_radius);

	void SetFlamePosition(Vector3 in_pos);

	void SetFlameLife(const float& in_life) { m_life = in_life; }

	void CollisionCheck(Player* in_player);

	void ComputeWorldTransform2(Matrix4 in_mat);

private:

	class Particle* m_particle;
	
	class Texture* m_texture;

	class BoxCollider* m_hitBox;          // �����蔻��{�b�N�X

	struct Sphere* m_hitSphere;                   // �����蔻��(��)

	Vector3 m_velocity;                   // ���������オ��ۂɗp��������x

	float m_life;                         // ���̔�����������܂ł̎��Ԕ{��

	// �s��֘A
	Matrix4 m_view;
	Matrix4 m_projection;
	Matrix4 m_billboard;

};