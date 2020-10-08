#pragma once
#include "MoveComponent.h"

class MoveComponentHuman : public MoveComponent
{

public:

	MoveComponentHuman(class PlayerHuman* in_owner);     // �R���X�g���N�^
	~MoveComponentHuman();                               // �f�X�g���N�^


	void Update(float in_deltaTime) override;                      // �R���|�[�l���g�̍X�V

	void ProcessInput(float in_deltaTime) override {}                                    // �R���|�[�l���g�̓��͏���

	void OnUpdateWorldTransform() override {}                      // ���[���h���W�ϊ��p


	void MovementByController(float in_deltaTime) override;

	void MovementByKeyboard(float in_deltaTime) override;


protected:

	class PlayerHuman* m_playerHuman;       // �v���C���[(�l�Ԍ^)�ւ̃|�C���^

	static const float PLAYER_SPEED;

};