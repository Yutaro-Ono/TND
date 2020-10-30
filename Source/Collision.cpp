//-----------------------------------------------------------------------+
// �Փˌ��m(�����蔻��)�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Collision.h"
#include <algorithm>
#include <array>



float LensSegOnSeparateAxis(Vector3& in_separate, Vector3& in_e1, Vector3& in_e2, Vector3& in_e3);     // �������ɓ��e���ꂽ���������瓊�e���������Z�o
float LensSegOnSeparateAxis(Vector3& in_separate, Vector3& in_e1, Vector3& in_e2);                     // �������ɓ��e���ꂽ���������瓊�e���������Z�o


//-------------------------------------------------------------+
// ����
//-------------------------------------------------------------+
// �R���X�g���N�^
// (mCenter:������, mRdius:������)
Sphere::Sphere(const Vector3 & in_center, float in_radius)
	:m_center(in_center)
	,m_radius(in_radius)
{
	// �����Ȃ�
}

// ���̓����蔻�菈��(Sphere�̒���point���܂܂�Ă��邩)
bool Sphere::Contains(const Vector3 & in_point) const
{
	// ���̒��S�Ɠ_�Ƃ̋����̓��
	float distSq = (m_center - in_point).LengthSq();
	// ���Ɣ��a��2����r���A�Ԃ�
	return distSq <= m_radius * m_radius;
}

// ���̓��m�̓����蔻��
bool Sphere::ContainsSphere(const Sphere in_otherSphere) const
{
	// ���̂Ƌ��̂̒��S�_�̋����̓����擾
	float distSq = (m_center - in_otherSphere.m_center).LengthSq();
	// ���̓��m�̔��a�̓�悩���r���A�~�Ɖ~���G��Ă��邩��Ԃ�
	return distSq <= (m_radius + in_otherSphere.m_radius) * (m_radius + in_otherSphere.m_radius);
}


//-------------------------------------------------------------+
// ����
//-------------------------------------------------------------+
// �R���X�g���N�^
// (mNormal:������, mPos:������
Plane::Plane(const Vector3 & in_normal, const Vector3 in_position)
	:m_normal(in_normal)
	,m_position(in_position)
{
	// �����Ȃ�
}


//-------------------------------------------------------------+
// �����s�{�b�N�X (AABB:Axis-Aligned Bounding Box)
//-------------------------------------------------------------+
// �R���X�g���N�^
AABB::AABB()
{
	m_min = Vector3(0, 0, 0);
	m_max = Vector3(0, 0, 0);
}

// �������p�R���X�g���N�^
AABB::AABB(const Vector3 & in_min, const Vector3 & in_max)
	:m_min(in_min)
	,m_max(in_max)
{
	// �����Ȃ�
}

// �ŏ��l�ƍő�l���X�V
void AABB::UpdateMinMax(const Vector3 & in_point)
{

	// x,y,z���ꂼ��̍ŏ��l�����߂�
	m_min.x = Math::Min(m_min.x, in_point.x);
	m_min.y = Math::Min(m_min.y, in_point.y);
	m_min.z = Math::Min(m_min.z, in_point.z);

	// x,y,z���ꂼ��̍ő�l�����߂�
	m_max.x = Math::Max(m_max.x, in_point.x);
	m_max.y = Math::Max(m_max.y, in_point.y);
	m_max.z = Math::Max(m_max.z, in_point.z);

}

bool AABB::Contains(const Vector3 & in_point) const
{
	
	bool outside = in_point.x < m_min.x ||
		in_point.y < m_min.y ||
		in_point.z < m_min.z ||
		in_point.x > m_max.x ||
		in_point.y > m_max.y ||
		in_point.z > m_max.z;

	// �������true���Ȃ���΁Abox�̓����ɓ_�����݂���
	return !outside;
}

float AABB::MinDistSq(const Vector3 & in_point) const
{

	// ���ꂼ��̎��ł̍����Ƃ�
	float dx = Math::Max(m_min.x - in_point.x, 0.0f);
	dx = Math::Max(dx, in_point.x - m_max.x);
	float dy = Math::Max(m_min.y - in_point.y, 0.0f);
	dy = Math::Max(dy, in_point.y - m_max.y);
	float dz = Math::Max(m_min.z - in_point.z, 0.0f);
	dz = Math::Max(dy, in_point.z - m_max.z);

	// 3������Ԃł̋����̓��̌������
	return dx * dx + dy * dy + dz * dz;
}

void AABB::Rotate(const Quaternion & in_quaternion)
{
	// �{�b�N�X��8�̒��_�̔z����
	std::array<Vector3, 8> points;
	// �ŏ��l�͏�ɃR�[�i�[�ł���
	points[0] = m_min;
	// 2�̍ŏ��l��1�̍ő�l�̕��בւ�
	points[1] = Vector3(m_max.x, m_min.y, m_min.z);
	points[2] = Vector3(m_min.x, m_max.y, m_min.z);
	points[3] = Vector3(m_min.x, m_min.y, m_max.z);
	// 2�̍ő�l��1�̍ŏ��l�̕��בւ�
	points[4] = Vector3(m_min.x, m_max.y, m_max.z);
	points[5] = Vector3(m_max.x, m_min.y, m_max.z);
	points[6] = Vector3(m_max.x, m_max.y, m_min.z);
	// �ő�l�͏�ɃR�[�i�[�ł���
	points[7] = Vector3(m_max);

	// �ŏ��̓_����]
	Vector3 p = Vector3::Transform(points[0], in_quaternion);
	// �ŏ��l�ƍő�l�����߂邽�߂ɍŏ��̓_�����ă��Z�b�g
	m_min = p;
	m_max = p;
	// ��]�ɂ���čő�l�ƍŏ��l�����߂Ȃ����B
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], in_quaternion);
		UpdateMinMax(p);
	}
}


//----------------------------------------------------+
// �L���{�b�N�X (OBB:Oriented Bounding Box)
//----------------------------------------------------+
// �R���X�g���N�^
OBB::OBB()
	:m_position(0.0f, 0.0f, 0.0f)
{
	for (int i = 0; i < 3; i++)
	{
		m_normalDirection[i] = Vector3::Zero;
		m_length[i] = 0.0f;
	}
}


OBB::OBB(const Vector3 & in_position, const Vector3 & in_degree, const Vector3 & in_scale)
{

	Matrix4 matrix[3], finalMat;
	float theta[3];
	m_position = in_position;

	// ��]�p(�x��)��胉�W�A���p�ɕϊ�
	theta[0] = Math::ToRadians(in_degree.x);
	theta[1] = Math::ToRadians(in_degree.y);
	theta[2] = Math::ToRadians(in_degree.z);

	// ��]�p����]�s��쐬
	matrix[0] = Matrix4::CreateRotationX(theta[0]);
	matrix[1] = Matrix4::CreateRotationY(theta[1]);
	matrix[2] = Matrix4::CreateRotationZ(theta[2]);

	// ��]�s��̍��������x�N�g�����擾
	finalMat = matrix[0] * matrix[1] * matrix[2];

	m_normalDirection[0] = finalMat.GetXAxis();
	m_normalDirection[1] = finalMat.GetYAxis();
	m_normalDirection[2] = finalMat.GetZAxis();

	// �X�P�[��(�{�b�N�X����)�̔�����ۑ�
	m_length[0] = in_scale.x * 0.5f;
	m_length[1] = in_scale.y * 0.5f;
	m_length[2] = in_scale.z * 0.5f;

}

// AABB�������l�ɂ���OBB�����
OBB::OBB(AABB in_aabb)
{

	// ��]�Ȃ�
	m_normalDirection[0] = Vector3::UnitX;
	m_normalDirection[1] = Vector3::UnitY;
	m_normalDirection[2] = Vector3::UnitZ;

	// �e���̒���
	m_length[0] = (in_aabb.m_max.x - in_aabb.m_min.x) * 0.5f;
	m_length[1] = (in_aabb.m_max.y - in_aabb.m_min.y) * 0.5f;
	m_length[2] = (in_aabb.m_max.z - in_aabb.m_min.z) * 0.5f;

	// �{�b�N�X���S�̃��[���h���W
	m_position.x = in_aabb.m_min.x + m_length[0];
	m_position.y = in_aabb.m_min.y + m_length[1];
	m_position.z = in_aabb.m_min.z + m_length[2];

}

// �e���̌������擾 0 = x��, 1 = y��, 2 = z��
Vector3 OBB::GetDirection(int in_element)
{
	return m_normalDirection[in_element];
}

Vector3 OBB::GetPositionWorld()
{
	return m_position;
}

// �e���̒������擾 0 = x��, 1 = y��, 2 = z��
float OBB::GetLength(int in_element)
{
	return m_length[in_element];
}

Matrix4 OBB::GetMatrix()
{
	Matrix4 finalMat, rotmat, scale;

	rotmat.mat[0][0] = m_normalDirection[0].x;
	rotmat.mat[0][1] = m_normalDirection[1].x;
	rotmat.mat[0][2] = m_normalDirection[2].x;
	rotmat.mat[0][3] = 0.0f;
	rotmat.mat[1][0] = m_normalDirection[0].y;
	rotmat.mat[1][1] = m_normalDirection[1].y;
	rotmat.mat[1][2] = m_normalDirection[2].y;
	rotmat.mat[1][3] = 0.0f;
	rotmat.mat[2][0] = m_normalDirection[0].z;
	rotmat.mat[2][1] = m_normalDirection[1].z;
	rotmat.mat[2][2] = m_normalDirection[2].z;
	rotmat.mat[2][3] = 0.0f;
	rotmat.mat[3][0] = 0.0f;
	rotmat.mat[3][1] = 0.0f;
	rotmat.mat[3][2] = 0.0f;
	rotmat.mat[3][3] = 1.0f;

	scale = Matrix4::CreateScale(Vector3(m_length[0], m_length[1], m_length[2]));
	finalMat = scale * rotmat;
	finalMat.mat[3][0] = m_position.x;
	finalMat.mat[3][1] = m_position.y;
	finalMat.mat[3][2] = m_position.z;

	return finalMat;

}

void OBB::Transform(Matrix4 & in_matrix)
{
	m_position = Vector3::Transform(m_position, in_matrix);

	m_normalDirection[0] = in_matrix.GetXAxis();
	m_normalDirection[1] = in_matrix.GetYAxis();
	m_normalDirection[2] = in_matrix.GetZAxis();
}

// OBB�ǂ����̏Փ�
bool ColOBBs(OBB &in_obbA, OBB &in_obbB)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	Vector3 NAe1 = in_obbA.GetDirection(0), Ae1 = NAe1 * in_obbA.GetLength(0);
	Vector3 NAe2 = in_obbA.GetDirection(1), Ae2 = NAe2 * in_obbA.GetLength(1);
	Vector3 NAe3 = in_obbA.GetDirection(2), Ae3 = NAe3 * in_obbA.GetLength(2);
	Vector3 NBe1 = in_obbB.GetDirection(0), Be1 = NBe1 * in_obbB.GetLength(0);
	Vector3 NBe2 = in_obbB.GetDirection(1), Be2 = NBe2 * in_obbB.GetLength(1);
	Vector3 NBe3 = in_obbB.GetDirection(2), Be3 = NBe3 * in_obbB.GetLength(2);
	Vector3 Interval = in_obbA.GetPositionWorld() - in_obbB.GetPositionWorld();

	// ������ : Ae1
	float rA = Ae1.Length();
	float rB = LensSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabs(Vector3::Dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	 // ������ : Ae2
	rA = Ae2.Length();
	rB = LensSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = Ae3.Length();
	rB = LensSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LensSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.Length();
	L = fabs(Vector3::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LensSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.Length();
	L = fabs(Vector3::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LensSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.Length();
	L = fabs(Vector3::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	Vector3 Cross;
	Vector3 Zero = Vector3::Zero;
	Cross = Vector3::Cross(NAe1, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LensSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LensSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;

}



// AABB�ǂ����̏Փ�
bool Intersect(const AABB & in_a, const AABB & in_b)
{
	bool no = in_a.m_max.x < in_b.m_min.x ||
		in_a.m_max.y < in_b.m_min.y ||
		in_a.m_max.z < in_b.m_min.z ||
		in_b.m_max.x < in_a.m_min.x ||
		in_b.m_max.y < in_a.m_min.y ||
		in_b.m_max.z < in_a.m_min.z;
	// ���true���Ȃ���΁A��͏Փ˂��Ă���
	return !no;
}

// ����AABB�̏Փ�
bool Intersect(const Sphere & in_sphere, const AABB & in_box)
{
	float distSq = in_box.MinDistSq(in_sphere.m_center);
	return distSq <= (in_sphere.m_radius * in_sphere.m_radius);
}

//----------------------------------------------------------------------------------------------------+
// �Փˎ��A�߂荞�񂾕�����߂�����
// in     movableBox �ړ����� (ex �v���C���[)
// in     fixedBox   �ړ����Ȃ����� (ex �u���b�N)
// inout  calcFixVec �ړ����̂̕␳�����x�N�g��
//----------------------------------------------------------------------------------------------------+
void CalcCollisionFixVec(const AABB & in_movableBox, const AABB & in_fixedBox, Vector3 & in_calcFixVec)
{
	in_calcFixVec = Vector3(0, 0, 0);
	float dx1 = in_fixedBox.m_min.x - in_movableBox.m_max.x;
	float dx2 = in_fixedBox.m_max.x - in_movableBox.m_min.x;
	float dy1 = in_fixedBox.m_min.y - in_movableBox.m_max.y;
	float dy2 = in_fixedBox.m_max.y - in_movableBox.m_min.y;
	float dz1 = in_fixedBox.m_min.z - in_movableBox.m_max.z;
	float dz2 = in_fixedBox.m_max.z - in_movableBox.m_min.z;

	// dx, dy, dz �ɂ� ���ꂼ��1,2�̂�����Βl�������������Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		in_calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		in_calcFixVec.y = dy;
	}
	else
	{
		in_calcFixVec.z = dz;
	}
}

float PlaneToPointDist(const Vector3 & in_position, const Plane in_plane)
{
	Vector3 planePos = in_plane.m_position;
	Vector3 v;

	v = in_position - planePos;
	
	float dist = Vector3::Dot(v, in_plane.m_normal);

	return dist;
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o

float LensSegOnSeparateAxis(Vector3 & in_separate, Vector3 & in_e1, Vector3 & in_e2)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
// ������Seq�͕W��������Ă��邱��
	float r1 = fabs(Vector3::Dot(in_separate, in_e1));
	float r2 = fabs(Vector3::Dot(in_separate, in_e2));

	return r1 + r2;

}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float LensSegOnSeparateAxis(Vector3 & in_separate, Vector3 & in_e1, Vector3 & in_e2, Vector3 & in_e3)
{
	float r1r2 = LensSegOnSeparateAxis(in_separate, in_e1, in_e2);
	float r3 = fabs(Vector3::Dot(in_separate, in_e3));

	return r1r2 + r3;

}


