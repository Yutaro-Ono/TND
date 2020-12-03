#include "FrameBuffer.h"
#include <SDL_image.h>
#include <SDL.h>
#include <glad/glad.h>
#include "GameMain.h"
#include "Shader.h"
#include "VertexArray.h"

// コンストラクタ
FrameBuffer::FrameBuffer()
{
	// 全ポストエフェクト分のシェーダーを生成・ロード
	for (int i = 0; i < POST_EFFECT_TYPE::ALL_NUM; i++)
	{
		m_postEffectShaders.push_back(new Shader());
	}

	// ポストエフェクトシェーダのロード
	m_postEffectShaders[NONE]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag");
	m_postEffectShaders[INVERT_COLOR]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/InvertColorScreen.frag");
	m_postEffectShaders[GREY_SCALE]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/GreyScaleScreen.frag");
	m_postEffectShaders[CERNEL]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/CernelScreen.frag");
	m_postEffectShaders[BLUR]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/BlurScreen.frag");
	m_postEffectShaders[SHARP_EDGE]->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/SharpEdgeScreen.frag");

	m_activeShader = m_postEffectShaders[NONE];

	m_shaderNum = 0;

}

// デストラクタ
FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RBO);

	for (auto shader : m_postEffectShaders)
	{
		shader->Delete();
	}
	m_postEffectShaders.clear();

}

bool FrameBuffer::CreateFrameBuffer()
{

	//----------------------------------------------------------------------+
    //
    // ポストエフェクト用フレームバッファ定義
    //
    //----------------------------------------------------------------------+
    // フレームバッファオブジェクトを定義して作成(Gen)
	glGenFramebuffers(1, &m_FBO);
	// activeなフレームバッファとしてバインドすると、描画先になる
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// フレームバッファが正常に完了したかどうかをチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		return false;

	}

	//-----------------------------------------------------------------------+
	//
	// テクスチャカラーバッファの作成
	//
	//-----------------------------------------------------------------------+
	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 現在のフレームバッファオブジェクトにアタッチする
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

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

// フレームバッファへの書き込み処理 (この直後にメッシュなどの通常描画処理を行う)
void FrameBuffer::WriteFrameBuffer()
{
	if (m_shaderNum != 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
}

// 書き込まれたフレームを編集し描画する
void FrameBuffer::DrawFrameBuffer()
{
	if (m_shaderNum != 0)
	{
		// ブレンドの有効化
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// 2.フレームバッファ内容をスクリーンに描画
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 深度テストオフ
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// シェーダの有効化
		m_activeShader->SetActive();
		m_activeShader->SetInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
		// VAOバインド
		RENDERER->GetScreenVAO()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);
	}

	// Imguiデバッグ
	DebugFrameBuffer();
}

// ポストエフェクト変更 (デバッグ用)
void FrameBuffer::DebugFrameBuffer()
{
#ifdef  _DEBUG

	// ImGui更新
	//ImGui::Begin("Debug Console : PostProcess");
	ImGui::SliderInt("PostProcessShader", &m_shaderNum, 0, ALL_NUM - 1);
	// 指定したポストエフェクトをアクティブシェーダとして更新
	m_activeShader = m_postEffectShaders[m_shaderNum];

#endif //  _DEBUG

}
