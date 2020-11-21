#pragma once
#include "UIScreen.h"

class MissionListUI : public UIScreen
{

public:

	MissionListUI(std::vector<class MissionUI*> in_mission);     // コンストラクタ
	~MissionListUI();                                            // デストラクタ



private:


	class Texture* m_cursol;                      // カーソル
	class Texture* m_point;                       // ポイント

	class Texture* m_general;                     // 「配達任務」

	std::vector<class MissionUI*> m_missions;     // ミッションごとのUI


};