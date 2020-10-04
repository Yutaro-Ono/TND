//-----------------------------------------------------------------------+
// ステアリングコントローラー(※ハンコン)クラス
// 
// 特性：シングルトン
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
//#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
//#define INITGUID
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <map>

// 操作情報の測定
enum CALIBRATE_CONTROL
{
	WHEEL = 0,
	ACCELERATOR = 1,
	BRAKE = 2,
	CLUTCH = 3,

	X_AXIS = 1,
	Y_AXIS = 2,
	Z_AXIS = 3,
	SLIDER_0 = 4,
	SLIDER_1 = 5,
	X_ROT = 6,
	Y_ROT = 7,
	Z_ROT = 8,
};

class InputSteeringWheel
{
public:

	// インスタンス生成
	static InputSteeringWheel& SteeringControllerInstance()
	{
		static InputSteeringWheel ControllerInstance;
		return ControllerInstance;
	}

	~InputSteeringWheel();                 // デストラクタ

	bool Initialize();                     // 初期化処理

	void Delete();                         // 削除処理

	void Update();                         // 更新処理


	// 指定したコントローラ部位の入力値を取得
	long GetControlValue(int in_device, int in_control);

	void RenderDebugImGui();

private:

	InputSteeringWheel();                                 // コンストラクタ
	
	LPDIRECTINPUT8 m_dinput;                              // DirectInput
	LPDIRECTINPUTDEVICE8 m_dinputDevice;                  // dinputデバイス

	DIDEVCAPS m_devCaps;                                  // 接続デバイスの機能

	HINSTANCE m_hInstance;                                // インスタンスハンドル

	DIJOYSTATE2 m_joyPadData;                             // ジョイパッドデータ構造体

	bool m_isWheelActive;                                 // ハンコンが接続されているか


	float m_accelParam;                                   // アクセル入力値(float)
	float m_brakeParam;                                   // ブレーキ入力値(float)

	static const int MODEL_NAME;                          // 使用するステアリングコントローラのモデル番号

};

// インスタンスの取得
#define STEERING_CONTROLLER_INSTANCE InputSteeringWheel::SteeringControllerInstance()