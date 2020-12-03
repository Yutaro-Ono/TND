#pragma once
#include<SDL.h>
#include "Math.h"
#include "GameMain.h"
#include "Renderer.h"
#include <glad/glad.h>
#include "imgui/imconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

class Mouse
{
public:
	typedef enum MouseButtonState
	{
		MOUSE_BUTTON_OFF,
		MOUSE_BUTTON_PUSHDOWN,
		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_PULLUP
	}MouseButtonState;

	// インスタンス
	static Mouse& MouseInstance()
	{
		static Mouse MouseInstance;
		return MouseInstance;
	}

	~Mouse() {};

	void             SetRelativeMouseMode(bool value);
	const Vector2&   GetPosition() const { return m_mousePos; }
	float            GetPositionX() const { return m_mousePos.x; }
	float            GetPositionY() const { return m_mousePos.y; }

	const Vector2& GetWheel() const { return m_mouseWheel; }
	bool             GetButtonValue(int button) const;
	MouseButtonState GetButtonState(int button) const;
	void             OnMouseWheelEvent(SDL_Event& event);
	void             Update();

	void             RenderDebugImGui();

private:
	Mouse(); //シングルトン

	Vector2 m_mousePos;
	Vector2 m_mouseWheel;

	//ボタンのデータ
	Uint32 m_currentButtons;
	Uint32 m_prevButtons;

	// 相対モード
	bool   m_isRelative;
};

#define MOUSE_INSTANCE Mouse::MouseInstance()
