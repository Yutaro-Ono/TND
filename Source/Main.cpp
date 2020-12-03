//-----------------------------------------------------------------------+
// ���C�����[�v.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
// �C���N���[�h�t�@�C��
#include "GameMain.h"
#include "Renderer.h"
#include "SceneBase.h"
#include "TitleScene.h"
#include "GameScene.h"
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <string>


// ���C�����[�v
int main(int argc, char** argv)
{

	// �Q�[���̏���������(��ʂ̉���, ��ʂ̏c��, �t���X�N���[��:true false)
	// �������Ɏ��s������false��Ԃ�
	GAME_INSTANCE.Initialize();

	// �ŏ��̃V�[���̃Z�b�g
	//GAME_INSTANCE.SetFirstScene(new GameScene(0));
	GAME_INSTANCE.SetFirstScene(new TitleScene());

	// �Q�[�����[�v
	GAME_INSTANCE.RunLoop();
	// �Q�[���̏I������
	GAME_INSTANCE.CloseGame();

	return 0;

}