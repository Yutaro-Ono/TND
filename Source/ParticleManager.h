//-----------------------------------------------------------------------+
// ParticleManagerクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "Particle.h"
#include <vector>
#include <map>


class ParticleManager
{
public:

	ParticleManager();                                                 // コンストラクタ
	~ParticleManager();                                                // デストラクタ

	void Draw();                                                       // 描画処理
	void SetParticle(class Particle* in_particle);                     // パーティクルのセット
	void Update(float in_deltaTime);                                   // 更新処理

	void AllDeadParticle();                                            // パーティクルスタックの削除


private:

	void ChangeBlendMode(Particle::PARTICLE_BLEND in_blendType);
	void ChangeTexture(int in_changeTextureID);
	Vector3 CalcCameraPos();

	class Shader* m_shader;                                             // シェーダー
	std::vector<class Particle*> m_particles;                           // パーティクル行列
	std::vector<class Particle*> m_pendingParticles;                    // ペンディングアクター行列

	Matrix4 m_viewMat;                                                  // ビュー行列
	Matrix4 m_projectionMat;                                            // プロジェクション行列
	Matrix4 m_billboardMat;                                             // ビルボード行列
};