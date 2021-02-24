#pragma once
#include "SceneBase.h"

class TutorialScene : public SceneBase
{
public:

	// チュートリアルステート
	enum TUTORIAL_STATE
	{
		STATE_FADE_IN,
		STATE_FADE_OUT,
		STATE_TUTORIAL,
		STATE_0_MOVING,
		STATE_1_RIDING,
		STATE_2_DRIVING,

	};

	TutorialScene();
	~TutorialScene();

	void Initialize() override;

	SceneBase* Update() override;

	void Draw() override;

private:

	TUTORIAL_STATE m_state;

	class PlayerManager* m_player;

	class BridgeObject* m_bridge[8];

	class Environment* m_environment;                                   // 環境

};