#pragma once
#include "Actor.h"
#include <string>

class Helicopter : public Actor
{

public:

	Helicopter(const Vector3& in_pos);              // �R���X�g���N�^
	~Helicopter();             // �f�X�g���N�^

	void UpdateActor(float in_deltaTime) override;

private:


	// �@�̂̕��i�N���X
	class HeliBody* m_body;                // �{�f�B
	class HeliRotorMain* m_mainRotor;      // ���C�����[�^�[
	class HeliRotorBack* m_backRotor;      // �o�b�N���[�^�[


};