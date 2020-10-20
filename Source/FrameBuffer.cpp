#include "FrameBuffer.h"
#include <SDL_image.h>
#include <SDL.h>
#include <GL/glew.h>
#include "GameMain.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

bool FrameBuffer::CreateFrameBuffer()
{
	// スクリーンを覆うための頂点配列作成
	float quadVertices[] = {
		// ポジション   // テクスチャ座標
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// 頂点オブジェクトの作成
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// アトリビュート指定
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//----------------------------------------------------------------------+
    //
    // ポストエフェクト用フレームバッファ定義
    //
    //----------------------------------------------------------------------+
    // フレームバッファオブジェクトを定義して作成(Gen)
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	// activeなフレームバッファとしてバインドすると、描画先になる
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// 描画命令を行った後にバインドを解除すると、描画先がスクリーンに戻る
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// フレームバッファが正常に完了したかどうかをチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		return false;

	}

	// フレームバッファオブジェクトはアプリ終了前に削除することを忘れずに
	// glDeleteFramebuffers(1, &fbo);

	//-----------------------------------------------------------------------+
	//
	// テクスチャカラーバッファの作成
	//
	//-----------------------------------------------------------------------+
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	// 現在のフレームバッファオブジェクトにアタッチする
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	//-------------------------------------------------------------+
	//
	// レンダーバッファオブジェクトの作成
	//
	//-------------------------------------------------------------+
	// レンダーバッファオブジェクトの作成
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	// レンダーバッファ操作がオブジェクトに影響を与えるようにバインド
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// レンダーバッファは書き込み専用
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// FBOにRBOをアタッチ
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//次に、フレームバッファが完全であるかどうかをチェックしたいので、完全でない場合はエラーメッセージを表示します。
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// フレームバッファ解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::DrawFrameBuffer()
{
}
