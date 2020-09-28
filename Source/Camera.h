//-----------------------------------------------------------------------+
// カメラクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Common.h"
#include "Actor.h"

class Camera
{
public:
	
	Camera(Actor* in_targetActor);                             // コンストラクタ
	virtual ~Camera();                                         // デストラクタ

	virtual void UpdateActor(float in_deltaTime);              // カメラの更新
	void Update(float in_deltaTime);                           // オーバーライド不可
	void SetActive();
	void Initialize(const Vector3& in_cameraPos, const Vector3& in_targetPos, const Vector3& in_upVec);

	const Vector3& GetPosition() const { return m_position; }

	Matrix4& GetViewMatrix() { return m_viewMatrix; }
	Vector3& GetViewVector() { return m_viewTarget; }

protected:

	void SetViewMatrix(const Matrix4& in_view);

	Actor *m_targetActor;               // 基準となるアクター
	Matrix4 m_viewMatrix;               // ビュー行列
	Vector3 m_position;
	Vector3 m_viewTarget;               // カメラの注目点
	Vector3 m_viewVector;               // 視線ベクトル
};