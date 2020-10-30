//-----------------------------------------------------------------------+
// �Փˌ��m(�����蔻��)�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"


//----------------------------------------------------+
// ����
//----------------------------------------------------+
typedef struct Sphere
{

	Sphere(const Vector3& in_center, float in_radius);                                      // �R���X�g���N�^
	bool Contains(const Vector3& in_point) const;                                           // �|�C���g(���W)�����̓��Ɋ܂܂�Ă��邩
	bool ContainsSphere(const Sphere in_otherSphere) const;                                 // ����̋��̂Ƃ��̋��̂��ڐG���Ă��邩

	Vector3      m_center;                                                                  // ���̂̒��S���W
	float        m_radius;                                                                  // ���̂̔��a

}Sphere;


//----------------------------------------------------+
// ����
//----------------------------------------------------+
typedef struct Plane
{

	Plane(const Vector3& in_normal, const Vector3 in_position);                             // �R���X�g���N�^

	Vector3      m_normal;                                                                  // �@��
	Vector3      m_position;                                                                // ���ʏ�̓_
	
}Plane;


//----------------------------------------------------+
// �����s�{�b�N�X (AABB:Axis-Aligned Bounding Box)
//----------------------------------------------------+
typedef struct AABB
{

	AABB();                                                                                 // �R���X�g���N�^
	AABB(const Vector3& in_min, const Vector3& in_max);                                     // �������p�R���X�g���N�^

	
	void         InitMinMax(const Vector3& in_point) { m_min = m_max = in_point; }          // ���f���ǂݍ��ݎ��̍ŏ��ő�̓_�����߂�
	void         UpdateMinMax(const Vector3& in_point);                                     // �ŏ��ő�̓_���X�V

	bool         Contains(const Vector3& in_point) const;                                   // �_���{�b�N�X���Ɋ܂܂�邩
	float        MinDistSq(const Vector3& in_point) const;                                  // �_�Ƃ̍ŏ����������߂�

	void         Rotate(const Quaternion& in_quaternion);                                   // ��]
	void         SetAllowRotate(bool in_value) { m_isRotatable = in_value; }                // ��]�������邩

	// ���ڒ��_���W���Z�b�g
	void         SetMinVector(const Vector3& in_minVec) { m_min = in_minVec; }
	void         SetMaxVector(const Vector3& in_maxVec) { m_max = in_maxVec; }

	Vector3      m_min;                                                                     // �ŏ��l
	Vector3      m_max;                                                                     // �ő�l

	bool         m_isRotatable;                                                             // ��]���邩�ǂ���

}AABB;


//----------------------------------------------------+
// �L���{�b�N�X (OBB:Oriented Bounding Box)
//----------------------------------------------------+
typedef struct OBB
{

	OBB();                                                                                  // �R���X�g���N�^
	OBB(const Vector3& in_position, const Vector3& in_degree, const Vector3& in_scale);     // �������p�R���X�g���N�^
	OBB(AABB in_aabb);                                                                      // 

	Vector3     GetDirection(int in_element);                                               // 
	Vector3     GetPositionWorld();                                                         // 

	float       GetLength(int in_element);                                                  // 
	Matrix4     GetMatrix();                                                                // 
	void        Transform(Matrix4& in_matrix);                                              // 

	Vector3     m_position;                                                                 // 
	Vector3     m_normalDirection[3];                                                       // 
	float       m_length[3];                                                                // 

	AABB        m_wrapAABB;                                                                 // OBB�����͂�AABB

}OBB;


bool Intersect(const AABB& in_a, const AABB& in_b);                                         // AABB���m�̏Փ�
bool Intersect(const Sphere& in_sphere, const AABB& in_box);                                // ���̂�AABB�Ƃ̏Փ�

// AABB�̂߂荞�݂�߂��ʂ��v�Z
void CalcCollisionFixVec(const AABB& in_movableBox, const AABB& in_fixedBox, Vector3& in_calcFixVec);
// �_�ƕ��ʂƂ̕����t��������Ԃ�
float PlaneToPointDist(const Vector3& in_position, const Plane in_plane);