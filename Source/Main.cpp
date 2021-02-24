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
#include "TutorialScene.h"
#include "ResultScene.h"
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

// �����`���[(Launcher.exe)�Ăяo���֐� 
// path�ɂ�Launcher.exe�ւ̑��΃p�X�ւ̕����������
void runLauncher(const char* path)
{
	#ifndef _DEBUG
	char fullPathexe[512];
#pragma warning (disable:4996)
	sprintf(fullPathexe, "%s%s", path, "Launcher.exe");

	// �v���Z�X�N������
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);

	CreateProcess((LPCSTR)fullPathexe, (LPSTR)"", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, (LPCSTR)path, &si, &pi);

	#endif
}


// ���C�����[�v
int main(int argc, char** argv)
{

	// �Q�[���̏���������(��ʂ̉���, ��ʂ̏c��, �t���X�N���[��:true false)
	// �������Ɏ��s������false��Ԃ�
	bool success = GAME_INSTANCE.Initialize();

	if (success)
	{
		// �ŏ��̃V�[���̃Z�b�g
		GAME_INSTANCE.SetFirstScene(new TitleScene());
		//GAME_INSTANCE.SetFirstScene(new ResultScene(100, 100));
		//GAME_INSTANCE.SetFirstScene(new TutorialScene());

		// �Q�[�����[�v
		GAME_INSTANCE.RunLoop();

	}

	// �Q�[���̏I������
	GAME_INSTANCE.CloseGame();

	// �I���������`���[�Ăяo��
	runLauncher("../../");

	return 0;

}