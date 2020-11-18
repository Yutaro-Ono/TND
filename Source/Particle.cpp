//-----------------------------------------------------------------------+
// Particleクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Particle.h"
#include "Component.h"
#include "Actor.h"
#include "Shader.h"
#include "Renderer.h"
#include "ParticleManager.h"

Matrix4 Particle::m_staticBillboardMat;
Vector3 Particle::m_staticCameraWorldPos;

// コンストラクタ
Particle::Particle()
	:m_life(0.0f)
	,m_alpha(1.0f)
{
}

// コンストラクタ
Particle::Particle(const Vector3 & in_pos, const Vector3 & in_v, float in_scale, float in_alpha, float in_life)
	:m_position(in_pos)
	,m_velocity(in_v)
	,m_scale(in_scale)
	,m_life(in_life)
	,m_alpha(in_alpha)
	,m_nowTime(0.0f)
	,m_blendType(BLEND_ALPHA)
{

}

// デストラクタ
Particle::~Particle()
{
}

// 描画
void Particle::Draw(Shader * in_shader)
{
	Matrix4 mat, scale;
	scale = Matrix4::CreateScale(m_scale);
	mat = Matrix4::CreateTranslation(m_position);

	in_shader->SetMatrixUniform("u_worldTransform", scale * m_staticBillboardMat * mat);
	in_shader->SetFloatUniform("uAlpha", m_alpha);
	in_shader->SetVectorUniform("uColor", m_color);

	RENDERER->SetWorldSpriteVertex();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

// 更新処理
void Particle::Update(float in_deltaTime)
{
	m_nowTime += in_deltaTime;
	m_velocity = m_velocity + m_acceleration;
	m_position = m_position + m_velocity;

	// 時間とともにアルファを薄くする
	m_alpha = (m_life - m_nowTime) / m_life;
}

// 
bool Particle::IsAlive() const
{
	return m_nowTime < m_life;
}

bool Particle::operator<(const Particle & in_rhs) const
{
	float lenThis, lenRhs;

	lenThis = (m_staticCameraWorldPos - m_position).LengthSq();
	lenRhs = (m_staticCameraWorldPos - in_rhs.m_position).LengthSq();
	
	return lenThis < lenRhs;
}

bool Particle::operator>(const Particle & in_rhs) const
{
	float lenThis, lenRhs;

	lenThis = (m_staticCameraWorldPos - m_position).LengthSq();
	lenRhs = (m_staticCameraWorldPos - in_rhs.m_position).LengthSq();

	return lenThis > lenRhs;
}

// ビルボード行列を取得
Matrix4 GetBillboardMatrix()
{
	Matrix4 ret;
	ret = RENDERER->GetViewMatrix();
	ret.mat[3][0] = ret.mat[3][1] = ret.mat[3][2] = 0.0f;
	ret.Transpose();
	//ret.mat[1][1] *= -1;
	ret.mat[2][2] *= -1;

	Matrix4 Rot;
	Rot = Matrix4::CreateRotationX(-0.5f * Math::Pi);
	ret = Rot * ret ;

	return Matrix4(ret);
}
