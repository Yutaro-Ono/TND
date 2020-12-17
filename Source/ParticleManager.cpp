//-----------------------------------------------------------------------+
// ParticleManagerクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ParticleManager.h"
#include "Particle.h"
#include "Shader.h"
#include "Renderer.h"
#include <algorithm>

// コンストラクタ
ParticleManager::ParticleManager()
{
	m_shader = new Shader;

	if (!m_shader->Load("Data/Shaders/Phong.vert", "Data/Shaders/Particle.frag"))
	{
		printf("ParticleManager / Shader Load : Failed\n");
	}
}

// デストラクタ
ParticleManager::~ParticleManager()
{
	// 全パーティクルの削除
	while (!m_particles.empty())
	{
		delete m_particles.back();
		m_particles.pop_back();
	}

	delete m_shader;
}

// 描画処理
void ParticleManager::Draw()
{

	if (m_particles.size() == 0)
	{
		return;
	}
	// ブレンドモード初期状態取得
	Particle::PARTICLE_BLEND blendType, prevType;
	auto itr = m_particles.begin();
	blendType = prevType = (*itr)->GetBlendType();

	// テクスチャID初期状態取得
	int nowTexture, prevTexture;
	nowTexture = prevTexture = (*itr)->GetTextureID();

	// ビュープロジェクション行列
	Matrix4 viewProjectionMat;
	viewProjectionMat = m_viewMat * m_projectionMat;

	// シェーダーON
	m_shader->SetActive();
	m_shader->SetMatrixUniform("u_viewProj", viewProjectionMat);
	m_shader->SetInt("uTexture", 0);

	// 全てのパーティクルのビルボード行列をセット
	(*itr)->SetBillboardMat(GetBillboardMatrix());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	// すべてのパーティクルを描画
	ChangeBlendMode(blendType);
	ChangeTexture(nowTexture);


	// mParticle分の描画ループ
	for (auto particles : m_particles)
	{
		
		//ブレンドモード変更が必要なら変更する
		blendType = particles->GetBlendType();

		// ブレンドタイプが更新されたら変更
		if (blendType != prevType)
		{
			ChangeBlendMode(blendType);
		}


		// テクスチャ変更が必要なら変更する
		nowTexture = particles->GetTextureID();
		if (nowTexture != prevTexture)
		{
			ChangeTexture(nowTexture);
		}

		// テクスチャのアクティブ化
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nowTexture);

		// パーティクル描画
		particles->Draw(m_shader);


		// 前回描画状態として保存
		prevType = blendType;
		prevTexture = nowTexture;

	}


	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

// パーティクルをベクトル配列にセット
void ParticleManager::SetParticle(Particle * in_particle)
{
	// ペンディング配列に追加
	m_pendingParticles.emplace_back(in_particle);
}

// 更新処理
void ParticleManager::Update(float in_deltaTime)
{
	// 全てのパーティクルの更新
	{
 		for (auto particles : m_particles)
		{
			particles->Update(in_deltaTime);
		}
	}

	// ペンディング中のパーティクルをアクティブパーティクルに追加
	{
		for (auto pending : m_pendingParticles)
		{
			m_particles.emplace_back(pending);
		}
	}

	// ペンディングをクリア
	if (m_pendingParticles.size() != 0)
	{
		m_pendingParticles.clear();
	}


	// アクティブなパーティクルが存在しない場合
	if (m_particles.size() == 0)
	{
		return;
	}

	// ビュー行列・プロジェクション行列・ビルボード行列
	m_viewMat = RENDERER->GetViewMatrix();
	m_projectionMat = RENDERER->GetProjectionMatrix();
	m_billboardMat = GetBillboardMatrix();

	// 現在のカメラ位置をパーティクルに教える
	auto iter = m_particles.begin();

	// 死んでいるパーティクルを削除
	while (iter != m_particles.end())
	{
		// パーティクルが死んでいるなら削除
		if (!((*iter)->IsAlive()))
		{
			// erase(iter)は要素の削除に加え、次の要素を指す
			iter = m_particles.erase(iter);
		}
		else
		{
				iter++;
		}
	}


	// アクティブなパーティクルが存在しない場合はreturn
	if (m_particles.size() == 0)
	{
		return;
	}


	iter = m_particles.begin();
	// ビルボード位置
	(*iter)->m_staticBillboardMat = m_billboardMat;

	// カメラ位置
	(*iter)->m_staticCameraWorldPos = CalcCameraPos();


	// パーティクルをカメラ距離で降順ソート (カメラにより近いパーティクルを前面に出す)
	std::sort(m_particles.begin(), m_particles.end(), std::greater<Particle *>());

}

void ParticleManager::AllDeadParticle()
{
	auto iter = m_particles.begin();

	while (iter != m_particles.end())
	{
		// erase(iter)は要素の削除に加え、次の要素を指す
		iter = m_particles.erase(iter);
	}
}

// ブレンドモード切替
void ParticleManager::ChangeBlendMode(Particle::PARTICLE_BLEND in_blendType)
{
	switch (in_blendType)
	{
	case Particle::BLEND_ADD:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);  //加算合成
		break;
	case Particle::BLEND_ALPHA:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // アルファブレンド
		break;
	case Particle::BLEND_MULT:
		glBlendFunc(GL_ZERO, GL_SRC_COLOR); //乗算合成
		break;
	default:
		break;
	}
}


void ParticleManager::ChangeTexture(int in_changeTextureID)
{
	glBindTexture(GL_TEXTURE_2D, in_changeTextureID);
}


Vector3 ParticleManager::CalcCameraPos()
{
	// ビュー行列よりワールドでのカメラ位置算出
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 transMat = view;

	// ビュー行列の移動成分抜き取る
	Vector3 v;
	v.x = -1.0f * view.mat[3][0];
	v.y = -1.0f * view.mat[3][1];
	v.z = -1.0f * view.mat[3][2];

	//移動成分を取り除いたあと転置して、回転部分の逆変換を作る
	transMat.mat[3][0] = transMat.mat[3][1] = transMat.mat[3][2] = 0.0f;
	transMat.Transpose();

	return Vector3(Vector3::Transform(v, transMat));
}
