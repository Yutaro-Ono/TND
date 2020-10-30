#pragma once
#include "GameMain.h"
#include "Renderer.h"
#include "Component.h"
#include <list>

class Particle
{
public:

	enum PARTICLE_BLEND
	{
		BLEND_ALPHA,
		BLEND_ADD,
		BLEND_MULT
	};

	// コンストラクタ
	Particle();
	Particle(const Vector3& in_pos, const Vector3& in_v, float in_scale, float in_alpha, float in_life);

	// デストラクタ
	~Particle();

	void Draw(class Shader* in_shader);
	void Update(float in_deltaTime);

	// Getter / Setter
	const Vector3& GetPosition() { return m_position; }
	const Vector3& GetVelocity() { return m_velocity; }
	
	int GetTextureID() { return m_textureID; }

	PARTICLE_BLEND GetBlendType() { return m_blendType; }

	// Getter / Setter
	void SetPosition(const Vector3& in_pos) { m_position = in_pos; }
	void SetVelocity(const Vector3& in_v) { m_velocity = in_v; }
	void SetColor(const Vector3& in_color) { m_color = in_color; }
	void SetTextureID(int in_texID) { m_textureID = in_texID; }
	void SetScale(float in_scale) { m_scale = in_scale; }
	void SetAlpha(float in_alpha) { m_alpha = in_alpha; }
	void SetBillboardMat(const Matrix4& in_mat) { m_staticBillboardMat = in_mat; }

	void SetBlendMode(PARTICLE_BLEND in_blendType) { m_blendType = in_blendType; }

	bool IsAlive() const;

	// カメラ距離でのソート用
	bool operator < (const Particle& in_rhs) const;
	bool operator > (const Particle& in_rhs) const;

private:

	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_color;

	float m_alpha;
	float m_life;
	float m_nowTime;
	float m_scale;

	int m_textureID;

	PARTICLE_BLEND m_blendType;

	static Matrix4 m_staticBillboardMat;
	static Vector3 m_staticCameraWorldPos;

	friend class ParticleManager;

};

Matrix4 GetBillboardMatrix();