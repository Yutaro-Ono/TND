#pragma once
#include <vector>
//---------------------------------------------------------+
//
// �Q�[�����[���h�����N���X
//
//---------------------------------------------------------+

class GameWorld
{

public:

	GameWorld();             // �R���X�g���N�^
	~GameWorld();            // �f�X�g���N�^



private:

	std::vector<class Actor*> m_clients;          // �˗��҃A�N�^�[


};