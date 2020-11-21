#pragma once
#include "UIScreen.h"

class MissionListUI : public UIScreen
{

public:

	MissionListUI(std::vector<class MissionUI*> in_mission);     // �R���X�g���N�^
	~MissionListUI();                                            // �f�X�g���N�^



private:


	class Texture* m_cursol;                      // �J�[�\��
	class Texture* m_point;                       // �|�C���g

	class Texture* m_general;                     // �u�z�B�C���v

	std::vector<class MissionUI*> m_missions;     // �~�b�V�������Ƃ�UI


};