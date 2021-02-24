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

	Environment(GAME_TIME in_gameTime, const Vector3& in_pos);     // �R���X�g���N�^1
	Environment(class GameWorld* in_world, GAME_TIME in_gameTime);     // �R���X�g���N�^2
	~Environment();                         // �f�X�g���N�^

	void Update();

	void SetSkyBox(GAME_TIME in_gameTime);
	void SetDirectionalLight(GAME_TIME in_gameTime, const Vector3& in_pos);             // ���ԑт��Ƃɕ��s���C�g��ݒ肷��

	void SetDirectionalLightPos(const Vector3& in_pos);

	// �Q�[���^�C���̃Q�b�^�[�E�Z�b�^�[
	GAME_TIME GetGameTime() { return m_gameTime; }
	void SetGameTime(GAME_TIME in_gameTime) { m_gameTime = in_gameTime; }


private:


	GAME_TIME m_gameTime;

	class GameWorld* m_world;

	DirectionalLight* m_dirLight;

	// �e��X�J�C�{�b�N�X
	class SkyBox* m_morningBox;
	class SkyBox* m_eveningBox;
	class SkyBox* m_nightBox;
};