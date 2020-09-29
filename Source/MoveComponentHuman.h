#pragma once
#include "MoveComponent.h"

class MoveComponentHuman : public MoveComponent
{

public:

	MoveComponentHuman(class Actor* in_owner);
	~MoveComponentHuman();


	void Update(float in_deltaTime) override {}                          // �R���|�[�l���g�̍X�V

	void ProcessInput(float in_deltaTime) override {}                                    // �R���|�[�l���g�̓��͏���

	void OnUpdateWorldTransform() override {}                      // ���[���h���W�ϊ��p


	void MovementByController(float in_deltaTime) override;

	void MovementByKeyboard(float in_deltaTime) override;


protected:

	bool m_padIsActive;                 // �R���g���[�����A�N�e�B�u���ǂ���
};