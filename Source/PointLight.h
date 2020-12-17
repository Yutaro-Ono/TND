//-------------------------------------------------+
// �|�C���g���C�g�A�N�^�[
// 2020 YutaroOno.
//-------------------------------------------------+
#pragma once
#include "Actor.h"

class PointLight : public Actor
{
public:

	// ���C�g�{�����[�� (���C�g�e���͈�)
	enum LIGHT_VOLUME
	{
		VL_SMALL = 0,
		VL_MEDIUM,
		VL_BIG
	};

	PointLight(LIGHT_VOLUME in_vol = VL_MEDIUM);
	~PointLight();

	void SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular);          // ���C�g�J���[�̃Z�b�g

	void UpdateActor(float in_deltaTime) override;

	void SwitchLighting();                                                              // �_�����ON�EOFF


	//-------------------------------------------+
	// Setter / Getter
	//-------------------------------------------+
	// �e��J���[�̎擾
	const Vector3& GetDiffuseColor() { return m_diffuse; }
	const Vector3& GetAmbientColor() { return m_ambient; }
	const Vector3& GetSpecularColor() { return m_specular; }
	
	LIGHT_VOLUME GetLightVolume() { return m_lightVolume; }     // ���C�g�e���͈͂̃Q�b�^�[

	void SetLuminance(float in_luminance) { m_luminance = in_luminance; }
	float GetLuminance() { return m_luminance; }

	void SetFlash(bool in_flash) { m_flash = in_flash; }        // �_�ł����邩�����Ȃ����̐؂�ւ�

private:


	LIGHT_VOLUME m_lightVolume;                                 // ���C�g�e���͈� (�|�C���g���C�g�R���|�[�l���g�ɂĎg�p)

	bool m_lighting;              // �_�����

	// �J���[���
	Vector3 m_diffuse;
	Vector3 m_ambient;
	Vector3 m_specular;

	// �P�x���
	float m_luminance;

	bool m_calcRadius;       // ���a���Čv�Z���邩

	bool m_flash;            // �_�ł����邩
	float m_flashOffset;     // �t���b�V���̊Ԋu

	// �|�C���g���C�g�R���|�[�l���g
	class PointLightComponent* m_lightComp;
};