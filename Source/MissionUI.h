#pragma once
#include "UIScreen.h"


class MissionUI : public UIScreen
{

public:

	MissionUI(class MissionBase* in_mission);
	~MissionUI();




private:


	class MissionBase* m_mission;

};