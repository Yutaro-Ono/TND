#pragma once
#include "Renderer.h"

class Environment
{
public:

	// �Q�[���̎��ԑ� (����ɉ����ăX�J�C�{�b�N�X���ω�)
	enum GAME_TIME
	{
		MORNING = 0,
		AFTERNOON = 1,
		EVENING = 2,
		NIGHT = 3
	};

	Environment(GAME_TIME in_gameTime);     // �R���X�g���N�^
	~Environment();                         // �f�X�g���N�^

	void Update();

	void SetDirectionalLight(GAME_TIME in_gameTime);             // ���ԑт��Ƃɕ��s���C�g��ݒ肷��

	// �Q�[���^�C���̃Q�b�^�[�E�Z�b�^�[
	GAME_TIME GetGameTime() { return m_gameTime; }
	void SetGameTime(GAME_TIME in_gameTime) { m_gameTime = in_gameTime; }


private:


	GAME_TIME m_gameTime;

	DirectionalLight m_dirLight;

	class SkyBox* m_morningBox;
	class SkyBox* m_eveningBox;
	class SkyBox* m_nightBox;
};