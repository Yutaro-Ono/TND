//-----------------------------------------------------------------------+
// 操作説明UIクラス
//-----------------------------------------------------------------------+

#pragma once
#include "UIScreen.h"



class PlayerControlUI : public UIScreen
{
public:

	PlayerControlUI(class PlayerManager* in_player);
	~PlayerControlUI();

	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader) override;

private:



	class PlayerManager* m_player;      // プレイヤーへのポインタ

	class WorldSpaceUI* m_rideTexture;
	
	class Texture* m_accessTex;            // アクセスの操作説明
	Vector2 m_accessTexPos;
	
	class Texture* m_dangerFontTex;            // 危険な状態 (ヘリに見つかった)表示

};

