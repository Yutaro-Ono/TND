//------------------------------------------------------------+
// ��(�v���C���[����)��SE�Đ��R���|�[�l���g
//------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "AudioManager.h"
#include "PlayerCar.h"
#include <unordered_map>

class CarSoundComponent : public Component
{
public:


	CarSoundComponent(class PlayerCar* in_owner);
	~CarSoundComponent();

	void Update(float in_deltaTime) override;

	void PlayEngineSound(float in_speed);     // �G���W�����Đ�

	void StopEngineSound();                   // �G���W������~

	void PlayEngineStart();                   // �J�n�G���W�����̍Đ�����
	
	void PlayCrushMed();                      // �Փˉ��Đ�(MEDIUM)
	void PlayCrushHard();                     // �Փˉ��Đ�(HARD)

	// ���ʉ��Đ��̒�~�Z�b�^�[
	void SetStop(bool in_stop) { m_isStop = in_stop; }

	

private:

	void LoadSound();    // ���ʉ��̃��[�h



	class PlayerCar* m_car;

	std::unordered_map<std::string, std::string> m_sound;     // �e����ʉ�

	bool m_isStop;                                            // ���ʉ��Đ��̒�~�t���O

};