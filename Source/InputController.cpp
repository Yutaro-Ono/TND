#include "InputController.h"
#include <limits.h>
#include <iostream>

// XINPUTのしきい値
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30
// 入力の最大値
const float InputController::PAD_MAX_VALUE = 32767.0f;

// コンストラクタ
InputController::InputController()
	:m_gameController(nullptr)
	,m_whichController(-1)
{
	// ボタンと軸を0で初期化
	// memset -> 配列を0で初期化できる(わざわざfor文で初期化する必要がない)
	memset(m_buttonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(m_prevButtonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(m_axisValues, 0, sizeof(float) * SDL_CONTROLLER_AXIS_MAX);
	printf("InputSystem < CONTROLLER_XINPUT > : Create Success.\n");
}

// デストラクタ
InputController::~InputController()
{
}

// 初期化処理
bool InputController::Initialize()
{
	// パッドの設定ファイル gamecontrollerdb.txt の読み込みと問題が無いかのチェック
    // 様々なコントローラーの入力タイプに対応
	int iNumOfControllers = SDL_GameControllerAddMappingsFromFile("Data/Strings/gamecontrollerdb.txt");
	if (iNumOfControllers == -1)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Error loading database [%s]", SDL_GetError());
		return false;
	}

	// コントローラ開く
	m_gameController = SDL_GameControllerOpen(0);

	if (SDL_IsGameController(0) && m_gameController)
	{
		std::cout << SDL_GameControllerMapping(m_gameController) << std::endl;
	}

	// コントローライベントの無視（こちらからフレーム毎に状態を取得するため）
	SDL_GameControllerEventState(SDL_IGNORE);
	return true;
}

// 更新処理
void InputController::Update()
{
	// コントローラが無い場合は early exitする
	if (m_gameController == NULL)
	{
		return;
	}

	// 前のフレームのコントローラの状態をコピーする
	memcpy(&m_prevButtonStates, &m_buttonStates, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);

	// コントローラの状態を更新する
	SDL_GameControllerUpdate();

	// 現在のコントローラのボタン状態を保存
	for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; ++b)
	{
		m_buttonStates[b] = SDL_GameControllerGetButton(m_gameController, (SDL_GameControllerButton)b);
	}

	// 現在のコントローラの軸情報を保存
	for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; ++a)
	{
		m_axisValues[a] = SDL_GameControllerGetAxis(m_gameController, (SDL_GameControllerAxis)a);
	}

	// 左スティック入力をベクトル化する
	m_axisL.x = (float)m_axisValues[SDL_CONTROLLER_AXIS_LEFTX];
	m_axisL.y = (float)m_axisValues[SDL_CONTROLLER_AXIS_LEFTY];

	// 右スティック入力をベクトル化する
	m_axisR.x = (float)m_axisValues[SDL_CONTROLLER_AXIS_RIGHTX];
	m_axisR.y = (float)m_axisValues[SDL_CONTROLLER_AXIS_RIGHTY];

	// しきい値以下を切る
	m_axisL.x = (fabs(m_axisL.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : m_axisL.x / PAD_MAX_VALUE;
	m_axisL.y = (fabs(m_axisL.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : m_axisL.y / PAD_MAX_VALUE;
	m_axisR.x = (fabs(m_axisR.x) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : m_axisR.x / PAD_MAX_VALUE;
	m_axisR.y = (fabs(m_axisR.y) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : m_axisR.y / PAD_MAX_VALUE;
}

// 解放処理
void InputController::Delete()
{
	if (!m_gameController)
	{
		SDL_GameControllerClose(m_gameController);
	}

	m_gameController = nullptr;
}


void InputController::ReceiveEvent(const SDL_Event & event)
{
	switch (event.type)
	{

	// コントローラー追加
	case SDL_CONTROLLERDEVICEADDED:
	{
		// コントローラーが取り付けられていなかった
		if (m_gameController != NULL)
		{
			// コントローラーのオープン
			m_whichController = event.cdevice.which;
			m_gameController = SDL_GameControllerOpen(m_whichController);
			// 前に取り付けられたコントローラの問題を回避するためメモリを0にセット。
			memset(m_buttonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
			memset(m_prevButtonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
			memset(m_axisValues, 0, sizeof(float)*SDL_CONTROLLER_AXIS_MAX);
		}
		break;
	}

	// コントローラー取り外しイベント
	case SDL_CONTROLLERDEVICEREMOVED:
	{
		// 現在のコントローラが取り外されたかのチェック
		if (m_whichController == event.cdevice.which)
		{
			m_whichController = -1;
			m_gameController = NULL;
		}
		break;
	}
	
	}

}


bool InputController::IsTriggered(const SDL_GameControllerButton in_button) const
{
	return (m_buttonStates[in_button] == 1 && m_prevButtonStates[in_button] == 0);
}

bool InputController::IsPressed(const SDL_GameControllerButton in_button) const
{
	return (m_buttonStates[in_button] == 1);
}

bool InputController::IsReleased(const SDL_GameControllerButton in_button) const
{
	return (m_buttonStates[in_button] == 0 && m_prevButtonStates[in_button] == 1);
}

float InputController::GetAxisValue(const SDL_GameControllerAxis in_axis) const
{

	return m_axisValues[in_axis] / PAD_MAX_VALUE;
}
