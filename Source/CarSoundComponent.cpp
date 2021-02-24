#include "CarSoundComponent.h"
#include "GameMain.h"

// �R���X�g���N�^
CarSoundComponent::CarSoundComponent(PlayerCar* in_owner)
	:Component(in_owner)
	,m_car(in_owner)
	,m_isStop(false)
{
	LoadSound();
}

// �f�X�g���N�^
CarSoundComponent::~CarSoundComponent()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="in_deltaTime"></param>
void CarSoundComponent::Update(float in_deltaTime)
{
	// ��~�t���O���I�t�̏ꍇ�̂�
	// �X�V������
	if (!m_isStop)
	{
		// �A�N�Z���l�E�u���[�L�l�擾
		float accel = m_car->GetAccelValue();
		float brake = m_car->GetBrakeValue();

		// �G���W�����̍Đ�
		PlayEngineSound(accel - brake);
	}
}

void CarSoundComponent::PlayEngineSound(float in_speed)
{
	// ���x�̊
	float slowSpeed = 10.0f;
	float normSpeed = 80.0f;
	float fastSpeed = 100.0f;

	// �A�N�Z���𓥂�ł��Ȃ�������
	if (m_car->GetDriveState() != PlayerCar::DRIVE_ACCEL)
	{
		StopEngineSound();
		return;
	}

	// ���x��0������50��
	if (in_speed > slowSpeed && in_speed < normSpeed)
	{
		// ���̃G���W�������~
		AUDIO->StopSound(m_sound["ENGINE_NORMAL"]);
		AUDIO->StopSound(m_sound["ENGINE_FAST"]);

		// SLOW�G���W�������Đ���������
		if (!AUDIO->IsPlayingSound(m_sound["ENGINE_SLOW"]))
		{
			AUDIO->PlaySoundTND(m_sound["ENGINE_SLOW"]);
		}
		
		return;
	}

	// ���x��50������100��
	if (in_speed >= slowSpeed && in_speed < fastSpeed)
	{
		// ���̃G���W�������~
		AUDIO->StopSound(m_sound["ENGINE_SLOW"]);
		AUDIO->StopSound(m_sound["ENGINE_FAST"]);

		// SLOW�G���W�������Đ���������
		if (!AUDIO->IsPlayingSound(m_sound["ENGINE_NORMAL"]))
		{
			AUDIO->PlaySoundTND(m_sound["ENGINE_NORMAL"]);
		}

		return;
	}

	// ���x��100��
	if (in_speed >= fastSpeed)
	{
		// ���̃G���W�������~
		AUDIO->StopSound(m_sound["ENGINE_SLOW"]);
		AUDIO->StopSound(m_sound["ENGINE_NORMAL"]);

		// SLOW�G���W�������Đ���������
		if (!AUDIO->IsPlayingSound(m_sound["ENGINE_FAST"]))
		{
			AUDIO->PlaySoundTND(m_sound["ENGINE_FAST"]);
		}

		return;
	}


}

/// <summary>
/// �S�G���W������~
/// </summary>
void CarSoundComponent::StopEngineSound()
{
	AUDIO->StopSound(m_sound["ENGINE_SLOW"]);
	AUDIO->StopSound(m_sound["ENGINE_NORMAL"]);
	AUDIO->StopSound(m_sound["ENGINE_FAST"]);
}

/// <summary>
/// �G���W���J�n�� (�v���C���[���Ԃɏ�荞�񂾎�)���ʉ��Đ�����
/// �e�N���X����w��
/// </summary>
void CarSoundComponent::PlayEngineStart()
{
	if (AUDIO->IsPlayingSound(m_sound["ENGINE_START"]))
	{
		AUDIO->StopSound(m_sound["ENGINE_START"]);
		AUDIO->PlaySoundTND(m_sound["ENGINE_START"]);
	}
	else
	{
		AUDIO->PlaySoundTND(m_sound["ENGINE_START"]);
	}
	
}

void CarSoundComponent::PlayCrushMed()
{
	AUDIO->PlaySoundTND(m_sound["CRUSH_MEDIUM"]);
}

void CarSoundComponent::PlayCrushHard()
{
	AUDIO->PlaySoundTND(m_sound["CRUSH_HARD"]);
}

/// <summary>
/// ���ʉ��̃��[�h
/// </summary>
void CarSoundComponent::LoadSound()
{
	// �G���W���J�n��
	m_sound["ENGINE_START"] = "Data/Music/SE/TND/Car/SUV/start_SUV.WAV";
	AUDIO->GetSound(m_sound["ENGINE_START"]);
	// ���s�G���W����(SLOW)
	m_sound["ENGINE_SLOW"] = "Data/Music/SE/TND/Car/SUV/SUVLowEngine.WAV";
	AUDIO->GetSound(m_sound["ENGINE_SLOW"]);
	// ���s�G���W���� (NORMAL)
	m_sound["ENGINE_NORMAL"] = "Data/Music/SE/TND/Car/SUV/SUVNormalEngine1.WAV";
	AUDIO->GetSound(m_sound["ENGINE_NORMAL"]);
	// ���s�G���W���� (FAST)
	m_sound["ENGINE_FAST"] = "Data/Music/SE/TND/Car/SUV/SUVFastEngine1.WAV";
	AUDIO->GetSound(m_sound["ENGINE_FAST"]);

	// �N���b�V��(�Փ�)���̌��ʉ�
	m_sound["CRUSH_MEDIUM"] = "Data/Music/SE/TND/Car/crush/medium1.WAV";
	AUDIO->GetSound(m_sound["CRUSH_MEDIUM"]);
	m_sound["CRUSH_HARD"] = "Data/Music/SE/TND/Car/crush/high2.WAV";
	AUDIO->GetSound(m_sound["CRUSH_HARD"]);

}
