#pragma once
#include "MeshComponent.h"

class EnvironmentMap : public MeshComponent
{

public:

	EnvironmentMap(class Actor* in_owner);
	~EnvironmentMap();

	virtual void Draw(class Shader* in_shader) override;                             // ���b�V���R���|�[�l���g�̕`��
	virtual void DrawShadow(class Shader* in_shader) override;                               // �V���h�E�����p�̕`��

private:



};