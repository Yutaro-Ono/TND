#pragma once
#include "Actor.h"

class PointLight : public Actor
{
public:

	PointLight();
	~PointLight();

	void SetLightParameter(float in_constant, float in_linear, float in_quadratic);     // ���C�g�p�����[�^�̃Z�b�g
	void SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular);                                     // ���C�g�J���[�̃Z�b�g

	void SwitchLighting();                                                              // �_�����ON�EOFF


	//-------------------------------------------+
	// Setter / Getter
	//-------------------------------------------+
	// ���C�g�{�����[���p���a�̎擾
	float GetLightRadius() { return m_lightRadius; }
	// �e�팸���p�����[�^�̎擾
	float GetConstant() { return m_constant; }
	float GetLinear() { return m_linear; }
	float GetQuadratic() { return m_quadratic; }
	// �e��J���[�̎擾
	const Vector3& GetDiffuseColor() { return m_diffuse; }
	const Vector3& GetAmbientColor() { return m_ambient; }
	const Vector3& GetSpecularColor() { return m_specular; }

	void SetLightRadius(float in_radius) { m_lightRadius = in_radius; }

private:

	void CalcLightRadius();       // ���C�g�̌������a�v�Z

	bool m_lighting;        // �_�����

	float m_lightRadius;    // ���C�g���a (�X�P�[���l)

	// �����p�����[�^
	float m_constant;
	float m_linear;
	float m_quadratic;

	// �J���[���
	Vector3 m_diffuse;
	Vector3 m_ambient;
	Vector3 m_specular;

	// �|�C���g���C�g�R���|�[�l���g
	class PointLightComponent* m_lightComp;
};