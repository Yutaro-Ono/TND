//-----------------------------------------------------------------------+
// 衝突検知(当たり判定)クラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"


//----------------------------------------------------+
// 球体
//----------------------------------------------------+
typedef struct Sphere
{

	Sphere(const Vector3& in_center, float in_radius);                                      // コンストラクタ
	bool Contains(const Vector3& in_point) const;                                           // ポイント(座標)が球体内に含まれているか
	bool ContainsSphere(const Sphere in_otherSphere) const;                                 // 相手の球体とこの球体が接触しているか

	Vector3      m_center;                                                                  // 球体の中心座標
	float        m_radius;                                                                  // 球体の半径

}Sphere;


//----------------------------------------------------+
// 平面
//----------------------------------------------------+
typedef struct Plane
{

	Plane(const Vector3& in_normal, const Vector3 in_position);                             // コンストラクタ

	Vector3      m_normal;                                                                  // 法線
	Vector3      m_position;                                                                // 平面上の点
	
}Plane;


//----------------------------------------------------+
// 軸並行ボックス (AABB:Axis-Aligned Bounding Box)
//----------------------------------------------------+
typedef struct AABB
{

	AABB();                                                                                 // コンストラクタ
	AABB(const Vector3& in_min, const Vector3& in_max);                                     // 初期化用コンストラクタ

	
	void         InitMinMax(const Vector3& in_point) { m_min = m_max = in_point; }          // モデル読み込み時の最小最大の点を求める
	void         UpdateMinMax(const Vector3& in_point);                                     // 最小最大の点を更新

	bool         Contains(const Vector3& in_point) const;                                   // 点がボックス内に含まれるか
	float        MinDistSq(const Vector3& in_point) const;                                  // 点との最小距離を求める

	void         Rotate(const Quaternion& in_quaternion);                                   // 回転
	void         SetAllowRotate(bool in_value) { m_isRotatable = in_value; }                // 回転を許可するか

	// 直接頂点座標をセット
	void         SetMinVector(const Vector3& in_minVec) { m_min = in_minVec; }
	void         SetMaxVector(const Vector3& in_maxVec) { m_max = in_maxVec; }

	Vector3      m_min;                                                                     // 最小値
	Vector3      m_max;                                                                     // 最大値

	bool         m_isRotatable;                                                             // 回転するかどうか

}AABB;


//----------------------------------------------------+
// 有向ボックス (OBB:Oriented Bounding Box)
//----------------------------------------------------+
typedef struct OBB
{

	OBB();                                                                                  // コンストラクタ
	OBB(const Vector3& in_position, const Vector3& in_degree, const Vector3& in_scale);     // 初期化用コンストラクタ
	OBB(AABB in_aabb);                                                                      // 

	Vector3     GetDirection(int in_element);                                               // 
	Vector3     GetPositionWorld();                                                         // 

	float       GetLength(int in_element);                                                  // 
	Matrix4     GetMatrix();                                                                // 
	void        Transform(Matrix4& in_matrix);                                              // 

	Vector3     m_position;                                                                 // 
	Vector3     m_normalDirection[3];                                                       // 
	float       m_length[3];                                                                // 

	AABB        m_wrapAABB;                                                                 // OBBを取り囲むAABB

}OBB;


bool Intersect(const AABB& in_a, const AABB& in_b);                                         // AABB同士の衝突
bool Intersect(const Sphere& in_sphere, const AABB& in_box);                                // 球体とAABBとの衝突

// AABBのめり込みを戻す量を計算
void CalcCollisionFixVec(const AABB& in_movableBox, const AABB& in_fixedBox, Vector3& in_calcFixVec);
// 点と平面との符号付き距離を返す
float PlaneToPointDist(const Vector3& in_position, const Plane in_plane);