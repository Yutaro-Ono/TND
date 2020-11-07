#pragma once
#include "UIScreen.h"
#include <list>
#include <string>

class MissionUI : public UIScreen
{

public:

	MissionUI(class MissionBase* in_mission);
	~MissionUI();

	void Update(float in_deltaTime) override;       // �X�V����

	void UpdateMissionInfo();                       // �~�b�V�������̐����E�X�V����

	void Draw(class Shader* in_shader) override;    // �`�揈��

	void SetUIPosition();


	//-----------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------+




private:


	class MissionBase* m_mission;

	// �e�N�X�`���֘A
	class Texture* m_detailTex[2];            // �~�b�V�����ڍ�(�z�B���^�N�V�[��)
	class Texture* m_timeTex[2];              // �������ԃe�N�X�`��
	class Texture* m_distanceTex[2];          // �����\���̃e�N�X�`��
	class Texture* m_durableValTex[2];        // �ϋv�\���̃e�N�X�`��


	Vector2 m_uiPos;                       // UI�\���ʒu�I�t�Z�b�g


};