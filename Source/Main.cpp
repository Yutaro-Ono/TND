//-----------------------------------------------------------------------+
// メインループ.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
// インクルードファイル
#include "GameMain.h"
#include "Renderer.h"
#include "TitleScene.h"
#include "ParticleTestScene.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

// メインループ
int main(int argc, char** argv)
{

	// ゲームの初期化処理(画面の横幅, 画面の縦幅, フルスクリーン:true false)
	// 初期化に失敗したらfalseを返す
	GAME_INSTANCE.Initialize(1920, 1080, true);

	// 最初のシーンのセット
	GAME_INSTANCE.SetFirstScene(new TitleScene);
	// 
	GAME_INSTANCE.GetRenderer()->SetWindowTitle("FLAME CARRIER");
	GAME_INSTANCE.RunLoop();
	// ゲームの終了処理
	GAME_INSTANCE.CloseGame();

	return 0;

}