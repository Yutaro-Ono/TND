#pragma once
#include "UIScreen.h"
#include <list>
#include <string>

class MissionUI : public UIScreen
{

public:

	MissionUI(class MissionBase* in_mission);
	~MissionUI();

	void Update(float in_deltaTime) override;       // 更新処理

	void UpdateMissionInfo();                       // ミッション情報の生成・更新処理

	void Draw(class Shader* in_shader) override;    // 描画処理

	void SetUIPosition();


	//-----------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------+




private:


	class MissionBase* m_mission;

	// テクスチャ関連
	class Texture* m_detailTex[2];            // ミッション詳細(配達かタクシーか)
	class Texture* m_timeTex[2];              // 制限時間テクスチャ
	class Texture* m_distanceTex[2];          // 距離表示用テクスチャ
	class Texture* m_durableValTex[2];        // 耐久表示用テクスチャ
	class Texture* m_scoreTex[2];             // 予測スコア表示用テクスチャ

	class Texture* m_generalWindow;            // 配達依頼
	class Texture* m_missionID[2];            // ミッションのID (A or B or C)
	class Texture* m_pointTex;                // 選択ポイント用テクスチャ

	class Texture* m_windowTex;               // ウィンドウ

	Vector2 m_uiPos;                          // UI表示位置オフセット

	float m_anim;                             // アニメーション用浮動小数点
};