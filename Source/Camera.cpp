//-----------------------------------------------------------------------+
// �J�����N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Camera.h"
#include "GameMain.h"
#include "Renderer.h"

// �R���X�g���N�^
Camera::Camera(Actor * in_targetActor)
	:m_targetActor(in_targetActor)
{
	// m_position = Vector3(0, 100, 100);

	// �f�t�H���g�J�����l�Z�b�g
	m_viewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100), Vector3(0, 0, 0), Vector3(0, 0, 1));

	// �Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ�ʒm
	SetActive();
}

// �f�X�g���N�^
Camera::~Camera()
{
	// �Q�[���V�X�e�����ɃA�N�e�B�u�J���������Ȃ��Ȃ������Ƃ�ʒm
	// �J�������̂͌p������Actor�Ńf�X�g���N�g�����
	GAME_INSTANCE.InActiveCamera(this);
}

void Camera::UpdateActor(float in_deltaTime)
{
	
}

void Camera::Update(float in_deltaTime)
{
	UpdateActor(in_deltaTime);

	// �J���������x�N�g���A�J�����s��쐬
	m_viewVector = m_viewTarget - m_position;
	m_viewMatrix = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3::UnitZ);
}

// �Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ��蓮�Œʒm����
void Camera::SetActive()
{
	// �Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ�ʒm
	GAME_INSTANCE.SetCamera(this);
}

// �ʒu����������
void Camera::Initialize(const Vector3 & in_cameraPos, const Vector3 & in_targetPos, const Vector3 & in_upVec)
{
	m_position = in_cameraPos;
	m_viewTarget = in_targetPos;
	m_viewVector = in_targetPos - in_cameraPos;

	m_viewMatrix = Matrix4::CreateLookAt(in_cameraPos, in_targetPos, in_upVec);
}

void Camera::SetViewMatrix(const Matrix4 & in_view)
{
	GAME_INSTANCE.GetRenderer()->SetViewMatrix(in_view);
}
