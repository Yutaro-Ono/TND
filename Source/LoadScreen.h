#pragma once
#include "UIScreen.h"
#include "GameMain.h"
#include "Renderer.h"

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
	void EnableScreen() { m_state = LOAD_STATE::ENABLE; RENDERER->Draw(); }
	void DisableScreen() { Sleep(150); m_state = LOAD_STATE::DISABLE; }

	// ���[�h��ʏ�ő������������ꍇ�Ƀt���O���I���ɂ���
	void SetIsGame() { m_isGame = true; RENDERER->Draw();}

	void AddGauge();


private:



	class Texture* m_loading;

	std::vector<class Texture*> m_loadGauges;

	int m_gaugeNum;

	class Texture* m_bgTexture;

	class Texture* m_tutorialMsg;

	LOAD_STATE m_state;

	bool m_isGame;                   // ���Q�[���V�[�����ǂ���(�Q�[���V�[���Ȃ瑀�������\��)

	std::unordered_map<std::string, std::string> m_sound;               // ���ʉ�

	Vector2 m_loadingPos;

	Vector2 m_loadGaugePos;

	static const int GAUGE_NUM;
};