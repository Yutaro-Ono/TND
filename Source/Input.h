//-----------------------------------------------------------------------+
// 入力クラス(シングルトン)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"

class Input
{
public:

	// キー押下状態
	enum KEY_STATE
	{
		KEY_OFF = 0,                        // キーが押されていない
		KEY_PUSH_DOWN,                      // キーが押下された
		KEY_PRESSED,                        // キーが押され続けている
		KEY_PULL_UP                         // 押されていたのが離された
	};

	// インスタンス
	static Input& InputInstance()
	{
		static Input InputInstance;
		return InputInstance;
	}

	~Input() {};

	void Update();                                     // 入力システム更新

	char GetInput(Uint8 SDL_SCANCODE_key);             // 指定したキーの入力状態の取得

	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);         // 指定したキーは押され続けている状態かを取得
	bool IsKeyPullUp(Uint8 SDL_SCANCODE_key);          // 指定したキーのボタンが離されたか？
	bool IsKeyPushDown(Uint8 SDL_SCANCODE_key);        // 指定したキーは押され続けているか
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);             // あるキーは押されていないか

private:

	Input();                                           // シングルトン

	char m_key[256];                                   // 全てのキー状態取得用

	char m_keyState[2][256];                           // キー状態 [現在 / 過去] [全キーコード]

	int m_nowKey, m_prevKey;                           // 現在のフレーム、過去のフレーム

};

#define INPUT_INSTANCE Input::InputInstance()