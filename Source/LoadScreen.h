#pragma once
#include "UIScreen.h"


class LoadScreen : public UIScreen
{


public:

	// ���[�h��ʂ��A�N�e�B�u��
	enum LOAD_STATE
	{
		ENABLE,
		DISABLE
	};

	LoadScreen();                                      // �R���X�g���N�^
	~LoadScreen();                                     // �f�X�g���N�^

	void Initialize();

	void Update(float in_deltaTime) override;          // �X�V����

	void Draw(class Shader* in_shader);                // �`�揈��


	// ���[�h��ʂ̕\���A��\��
	void EnableScreen() { m_state = LOAD_STATE::ENABLE; }
	void DisableScreen() { m_state = LOAD_STATE::DISABLE; }

	// ���[�h��ʏ�ő������������ꍇ�Ƀt���O���I���ɂ���
	void SetIsGame() { m_isGame = true; }

	void AddGauge();


private:



	class Texture* m_loading;

	class Texture* m_loadGauge[11];

	int m_gaugeNum;

	class Texture* m_bgTexture;

	class Texture* m_tutorialMsg;

	LOAD_STATE m_state;

	bool m_isGame;                   // ���Q�[���V�[�����ǂ���(�Q�[���V�[���Ȃ瑀�������\��)

	Vector2 m_loadingPos;

	Vector2 m_loadGaugePos;

};