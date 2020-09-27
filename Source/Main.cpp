//-----------------------------------------------------------------------+
// ���C�����[�v.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
// �C���N���[�h�t�@�C��
#include "GameMain.h"
#include "Renderer.h"
#include "TitleScene.h"
#include "ParticleTestScene.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

// ���C�����[�v
int main(int argc, char** argv)
{

	// �Q�[���̏���������(��ʂ̉���, ��ʂ̏c��, �t���X�N���[��:true false)
	// �������Ɏ��s������false��Ԃ�
	GAME_INSTANCE.Initialize(1920, 1080, true);

	// �ŏ��̃V�[���̃Z�b�g
	GAME_INSTANCE.SetFirstScene(new TitleScene);
	// 
	GAME_INSTANCE.GetRenderer()->SetWindowTitle("FLAME CARRIER");
	GAME_INSTANCE.RunLoop();
	// �Q�[���̏I������
	GAME_INSTANCE.CloseGame();

	return 0;

}