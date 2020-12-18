#pragma once
#include "Math.h"
#include "BoneTransform.h"
#include <vector>
#include <string>

class Animation
{

public:

	bool Load(const std::string& in_fileName, bool loop);                             // �A�j���[�V�����ǂݍ���

	size_t GetNumBones() const { return m_numBones; }                                 // �{�[�����̎擾
	size_t GetNum_frames() const { return m_numFrames; }                              // �t���[�����̎擾
	float  GetDuration() const { return m_duration; }                                 // �A�j���[�V�������Ԃ̎擾
	float  GetFrameDuration() const { return m_frameDuration; }                       // �t���[�����Ԃ̎擾
	bool   IsLoopAnimation() const { return m_isLoopAnimation; }                      // �A�j���[�V�����̓��[�v�A�j���[�V�������H

	// �w�肳�ꂽ�z����A�A�j���[�V�����̎w�肳�ꂽ���ԂɁA
	// �e�{�[���̃O���[�o���i���݂́j�|�[�Y�}�g���b�N�X�Ŗ��߂܂��B
	// ���Ԃ� > = 0.0f���� <= ���Ԃł���Ɨ\�z�����
	void GetGlobalPoseAtTime(std::vector<Matrix4>& out_Poses, const class Skeleton* in_skeleton, float in_time) const;

private:

	// �A�j���[�V�����̂��߂̃{�[����
	size_t m_numBones;

	// �A�j���[�V�����̃t���[����
	size_t m_numFrames;
	
	// �A�j���[�V�����̍Đ�����
	float m_duration;
	
	// �A�j���[�V�����̃t���[���Ԃ̎���
	float m_frameDuration;
	
	// �g���b�N��̊e�t���[���̕ϊ����B
	// �O���̃x�N�g���̊e�C���f�b�N�X�̓{�[���A
	// �����̃x�N�g���̓t���[���@�@�@�@�@�@�@mTracks[�{�[��][�t���[����]
	std::vector<std::vector<BoneTransform>> m_tracks;
	bool m_isLoopAnimation;                                       // ���[�v�A�j���[�V�������邩

};
