//-----------------------------------------------------------------------+
// タイトルシーンクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// インクルードファイル
#include "SceneBase.h"
#include <string>
#include <unordered_map>
#include "AudioManager.h"

class TitleScene : public SceneBase
{


public:


	enum TitleState
	{
		FADE_IN,
		FADE_OUT,
		PRESS_ANY_KEY,
		GAME_START,
		STAGE_SELECT,
		GAME_QUIT
	};

	TitleScene();
	~TitleScene();

	void Initialize() override;

	SceneBase* Update() override;
	void Draw() override;

	TitleState GetState() { return m_state; }                           // タイトルシーンの状態ゲッター


	const int& GetNowSelectStage() const { return m_selectedStage; }    // 現在選択中のステージ番号のゲッター

	const int& GetAllStageNum() const { return STAGE_ALL_NUM; }         // ステージ数取得


private:


	//class PlayerCar* m_car;                                             // 車
	class TitleCar* m_car;

	class BridgeObject* m_bridge[8];

	class ClientActor* m_client;

	class Environment* m_environment;                                   // 環境

	int m_selectedStage;                                                // 選択中のステージ
	std::vector<int> m_stage;                                           // ステージ番号

	std::unordered_map<std::string, std::string> m_sound;               // 効果音

	TitleState m_state;                                                 // タイトルシーンのステート


	static const int STAGE_ALL_NUM;                                     // 全ステージ


};