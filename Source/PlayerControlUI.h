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

	void SetFindPlayer(bool in_find) { m_findPlayer = in_find; }


private:



	class PlayerManager* m_player;      // プレイヤーへのポインタ

	class WorldSpaceUI* m_rideTexture;
	
	class Texture* m_accessTex;            // アクセスの操作説明
	Vector2 m_accessTexPos;
	
	class Texture* m_dangerFontTex;            // 危険な状態 (ヘリに見つかった)表示
	bool m_findPlayer;                         // ヘリに見つかったかどうか
};

