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
	class Texture* m_distanceTex[2];          // 距離表示のテクスチャ
	class Texture* m_durableValTex[2];        // 耐久表示のテクスチャ


	Vector2 m_uiPos;                       // UI表示位置オフセット


};