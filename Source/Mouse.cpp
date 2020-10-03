#include"mouse.h"
#include <iostream>

Mouse::Mouse()
{
	m_mousePos = Vector2(0, 0);
	m_currentButtons = 0;
	m_prevButtons = 0;
	m_isRelative = false;
}

void Mouse::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);
	m_isRelative = value;
}

bool Mouse::GetButtonValue(int button) const
{
	return button & m_currentButtons;
}

Mouse::MouseButtonState Mouse::GetButtonState(int button) const
{

	if (button & m_currentButtons)
	{
		return (button & m_prevButtons) ? MOUSE_BUTTON_PRESSED : MOUSE_BUTTON_PUSHDOWN;
	}

	return (button & m_prevButtons) ? MOUSE_BUTTON_PULLUP : MOUSE_BUTTON_OFF;
}

void Mouse::OnMouseWheelEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		m_mouseWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y)
		);
		break;

	default:
		break;
	}
}

// 入力処理の更新。レンダリングループの先頭で1回だけ呼ぶようにする
void Mouse::Update()
{
	m_prevButtons = m_currentButtons;

	int x = 0, y = 0;
	if (m_isRelative)
	{
		m_currentButtons = SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		m_currentButtons = SDL_GetMouseState(&x, &y);
	}
	m_mousePos.x = static_cast<float>(x);
	m_mousePos.y = static_cast<float>(y);


	m_mouseWheel = Vector2::Zero;
}

void Mouse::RenderDebugImGui()
{
	// ImGuiフレームを開始
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	ImGui::NewFrame();

	// ImGui更新
	ImGui::Begin("Debug Console : Mouse Input");
	ImGui::SliderFloat("Mouse Position : x", &m_mousePos.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Mouse Position : y", &m_mousePos.y, 0.0f, 1.0f);

	ImGui::End();
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	//glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
