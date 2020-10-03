#include "InputSteeringWheel.h"
#include "GameMain.h"
#include <dinput.h>




InputSteeringWheel::InputSteeringWheel()
	:m_dinput(NULL)
	,m_dinputDevice(NULL)
	,m_accelParam(0.0f)
	,m_brakeParam(0.0f)
{
	
	
	
}

InputSteeringWheel::~InputSteeringWheel()
{
}

bool InputSteeringWheel::Initialize()
{
	// インスタンスハンドルを取得
	m_hInstance = GetModuleHandle(NULL);
	// DirectInput作成
	HRESULT hresult = DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)m_dinput, NULL);

	return true;
}

void InputSteeringWheel::Delete()
{
}

// 更新処理
void InputSteeringWheel::Update()
{
	// 更新に用いる変数群
	int index = 0;             // 認識しているコントローラの数

	// 更新処理
	LogiUpdate();

	//m_isWheelActive = LogiIsDeviceConnected(0, LOGI_DEVICE_TYPE_WHEEL);
	m_isWheelActive = LogiIsModelConnected(0, MODEL_NAME);
	printf("Steering Wheel Connet : %d\n", m_isWheelActive);

	if (!m_isWheelActive)
	{
		// 入力状態を取得
		m_DIJoyState = LogiGetState(index);

		// ロジクール製コントローラ用プロパティ生成
		LogiControllerPropertiesData propertiesData;
		ZeroMemory(&propertiesData, sizeof(propertiesData));     // 初期化
		// 接続された番号(index)のコントローラのプロパティを取得
		LogiGetCurrentControllerProperties(index, propertiesData);

		// ペダルの入力値を取得(long型なのでfloatへ変換する)
		if (propertiesData.combinePedals)
		{
			m_accelParam = max(((-(float)(GetControlValue(index, ACCELERATOR))) / 32767), 0);
			m_brakeParam = max(((-(float)(GetControlValue(index, BRAKE))) / 32767), 0);
		}

	}

}

// 操作状態毎の値を返す
long InputSteeringWheel::GetControlValue(int in_device, int in_control)
{
	if (in_control == WHEEL)
	{
		switch (m_controlMap[in_control])
		{
		case X_AXIS:
			return m_DIJoyState->lX;
		case Y_AXIS:
			return m_DIJoyState->lY;
		case Z_AXIS:
			return m_DIJoyState->lZ;
		case SLIDER_0:
			return m_DIJoyState->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState->rglSlider[1];
		case X_ROT:
			return m_DIJoyState->lRx;
		case Y_ROT:
			return m_DIJoyState->lRy;
		case Z_ROT:
			return m_DIJoyState->lRz;
		default:
			return m_DIJoyState->lX;
		}
	}
	if (in_control == ACCELERATOR)
	{
		switch (m_controlMap[in_control])
		{
		case X_AXIS:
			return m_DIJoyState->lX;
		case Y_AXIS:
			return m_DIJoyState->lY;
		case Z_AXIS:
			return m_DIJoyState->lZ;
		case SLIDER_0:
			return m_DIJoyState->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState->rglSlider[1];
		case X_ROT:
			return m_DIJoyState->lRx;
		case Y_ROT:
			return m_DIJoyState->lRy;
		case Z_ROT:
			return m_DIJoyState->lRz;
		default:
			return m_DIJoyState->lY;
		}
	}
	if (in_control == BRAKE)
	{
		switch (m_controlMap[in_control])
		{
		case X_AXIS:
			return m_DIJoyState->lX;
		case Y_AXIS:
			return m_DIJoyState->lY;
		case Z_AXIS:
			return m_DIJoyState->lZ;
		case SLIDER_0:
			return m_DIJoyState->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState->rglSlider[1];
		case X_ROT:
			return m_DIJoyState->lRx;
		case Y_ROT:
			return m_DIJoyState->lRy;
		case Z_ROT:
			return m_DIJoyState->lRz;
		default:
			return m_DIJoyState->lRz;
		}
	}
	if (in_control == CLUTCH)
	{
		switch (m_controlMap[in_control])
		{
		case X_AXIS:
			return m_DIJoyState->lX;
		case Y_AXIS:
			return m_DIJoyState->lY;
		case Z_AXIS:
			return m_DIJoyState->lZ;
		case SLIDER_0:
			return m_DIJoyState->rglSlider[0];
		case SLIDER_1:
			return m_DIJoyState->rglSlider[1];
		case X_ROT:
			return m_DIJoyState->lRx;
		case Y_ROT:
			return m_DIJoyState->lRy;
		case Z_ROT:
			return m_DIJoyState->lRz;
		default:
			return m_DIJoyState->rglSlider[0];
		}
	}

	return 0;
}

// ImGuiによるデバッグ処理
void InputSteeringWheel::RenderDebugImGui()
{
	// ImGuiフレームを開始
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	ImGui::NewFrame();

	// ImGui更新
	ImGui::Begin("Debug Console : SteeringWheel Input");
	ImGui::InputFloat("Accel Pedal Value", &m_accelParam);
	ImGui::InputFloat("Brake Pedal Value", &m_brakeParam);
	ImGui::End();
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	//glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}