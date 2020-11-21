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
	class Texture* m_distanceTex[2];          // �����\���p�e�N�X�`��
	class Texture* m_durableValTex[2];        // �ϋv�\���p�e�N�X�`��
	class Texture* m_scoreTex[2];             // �\���X�R�A�\���p�e�N�X�`��

	class Texture* m_generalWindow;            // �z�B�˗�
	class Texture* m_missionID[2];            // �~�b�V������ID (A or B or C)
	class Texture* m_pointTex;                // �I���|�C���g�p�e�N�X�`��

	class Texture* m_windowTex;               // �E�B���h�E

	Vector2 m_uiPos;                          // UI�\���ʒu�I�t�Z�b�g

	float m_anim;                             // �A�j���[�V�����p���������_
};