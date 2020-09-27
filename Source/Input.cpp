//-----------------------------------------------------------------------+
// 入力クラス(シングルトン)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Input.h"

Input::Input()
{
	m_nowKey = 0;
	m_prevKey = 1;

	for (int i = 0; i < 256; i++)
	{
		m_key[i] = 0;
		m_keyState[m_nowKey][i] = KEY_OFF;
		m_keyState[m_prevKey][i] = KEY_OFF;
	}

	printf("InputSystem < KEY_BOARD > : Create Success.\n");
}

void Input::Update()
{
	// 現在のキーと1つ前のキーの状態を入れ替える  0を１に、1を0に
	m_nowKey ^= 1;
	m_prevKey = m_nowKey ^ 1;

	//キー状態取得
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	memcpy(m_keyState[m_nowKey], keys, sizeof(char) * 256);

	for (int i = 0; i < 256; i++)
	{
		char nowInput = m_keyState[m_nowKey][i];
		char prevInput = m_keyState[m_prevKey][i];

		if (nowInput) //現在押されている
		{
			//前のフレームも押されていた？
			if (prevInput)
			{
				m_key[i] = KEY_PRESSED; // 押されっぱなし
			}
			else {
				m_key[i] = KEY_PUSH_DOWN;// 今押された
			}

		}
		else // 現在キーは押されていない
		{
			// 前のフレームで押されていた？
			if (prevInput)
			{
				m_key[i] = KEY_PULL_UP;
			}
			else {
				m_key[i] = KEY_OFF;
			}
		}
	}
}

char Input::GetInput(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key];
}

bool Input::IsKeyPressed(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_PRESSED;
}

bool Input::IsKeyPullUp(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_PULL_UP;

}

bool Input::IsKeyPushDown(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_PUSH_DOWN;
}

bool Input::IsKeyOff(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_OFF;
}
