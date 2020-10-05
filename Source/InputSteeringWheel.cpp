#include "InputSteeringWheel.h"
#include "GameMain.h"
#include <dinput.h>


static LPDIRECTINPUT8 dinput;                              // DirectInput
static LPDIRECTINPUTDEVICE8 dinputDevice;                  // dinputデバイス

static DIDEVCAPS devCaps;                                  // 接続デバイスの機能

// ジョイスティック列挙関数
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* p_didInstance, void* p_Context)
{
	// 列挙されたジョイスティックへのインターフェースを取得
	HRESULT hresult = dinput->CreateDevice(p_didInstance->guidInstance, &dinputDevice, NULL);
	if (FAILED(hresult))
	{
		return DIENUM_CONTINUE;
	}

	// ジョイスティックの機能を調べる
	devCaps.dwSize = sizeof(DIDEVCAPS);
	hresult = dinputDevice->GetCapabilities(&devCaps);
	if (FAILED(hresult))
	{
		printf("ジョイスティック機能の取得に失敗\n");
		return DIENUM_CONTINUE;
	}


	return DIENUM_STOP;
}

// ジョイスティックの軸列挙関数
BOOL CALLBACK EnumAxesCallBack(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	return DIENUM_STOP;
}

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
	//// インスタンスハンドルを取得
	//m_hInstance = GetModuleHandle(NULL);
	//// DirectInput作成
	//HRESULT hresult = DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, NULL);

	//if (FAILED(hresult))
	//{
	//	printf("DirectInput8オブジェクトの作成に失敗\n");
	//	return false;
	//}

	//// デバイスからオブジェクトを作成
	//hresult = m_dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	//if (FAILED(hresult) || m_dinputDevice == NULL)
	//{
	//	printf("DirectInputDevice8オブジェクトの作成に失敗\n");
	//	return false;
	//}

	// データ形式を設定
	//hresult = m_dinputDevice->EnumObjects()

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



}


// 操作状態毎の値を返す
long InputSteeringWheel::GetControlValue(int in_device, int in_control)
{


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