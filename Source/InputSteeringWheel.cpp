#include "InputSteeringWheel.h"
#include "GameMain.h"
#include <dinput.h>


static LPDIRECTINPUT8 dinput;                              // DirectInput
static LPDIRECTINPUTDEVICE8 dinputDevice;                  // dinput�f�o�C�X

static DIDEVCAPS devCaps;                                  // �ڑ��f�o�C�X�̋@�\

// �W���C�X�e�B�b�N�񋓊֐�
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* p_didInstance, void* p_Context)
{
	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�[�X���擾
	HRESULT hresult = dinput->CreateDevice(p_didInstance->guidInstance, &dinputDevice, NULL);
	if (FAILED(hresult))
	{
		return DIENUM_CONTINUE;
	}

	// �W���C�X�e�B�b�N�̋@�\�𒲂ׂ�
	devCaps.dwSize = sizeof(DIDEVCAPS);
	hresult = dinputDevice->GetCapabilities(&devCaps);
	if (FAILED(hresult))
	{
		printf("�W���C�X�e�B�b�N�@�\�̎擾�Ɏ��s\n");
		return DIENUM_CONTINUE;
	}


	return DIENUM_STOP;
}

// �W���C�X�e�B�b�N�̎��񋓊֐�
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
	//// �C���X�^���X�n���h�����擾
	//m_hInstance = GetModuleHandle(NULL);
	//// DirectInput�쐬
	//HRESULT hresult = DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, NULL);

	//if (FAILED(hresult))
	//{
	//	printf("DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s\n");
	//	return false;
	//}

	//// �f�o�C�X����I�u�W�F�N�g���쐬
	//hresult = m_dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	//if (FAILED(hresult) || m_dinputDevice == NULL)
	//{
	//	printf("DirectInputDevice8�I�u�W�F�N�g�̍쐬�Ɏ��s\n");
	//	return false;
	//}

	// �f�[�^�`����ݒ�
	//hresult = m_dinputDevice->EnumObjects()

	return true;
}

void InputSteeringWheel::Delete()
{
}

// �X�V����
void InputSteeringWheel::Update()
{
	// �X�V�ɗp����ϐ��Q
	int index = 0;             // �F�����Ă���R���g���[���̐�



}


// �����Ԗ��̒l��Ԃ�
long InputSteeringWheel::GetControlValue(int in_device, int in_control)
{


	return 0;
}

// ImGui�ɂ��f�o�b�O����
void InputSteeringWheel::RenderDebugImGui()
{
	// ImGui�t���[�����J�n
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	ImGui::NewFrame();

	// ImGui�X�V
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