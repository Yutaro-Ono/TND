#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>

class Skeleton
{
public:
	// Definition for each bone in the skeleton                           ���ꂼ��̃{�[���̍��i��`
	struct Bone
	{
		BoneTransform mLocalBindPose;                                     // ���[�J���o�C���h�|�[�Y
		std::string mName;                                                // �{�[����
		int mParent;                                                      // �eID
	};

	// Load from a file                                                   // �t�@�C������̃��[�h
	bool Load(const std::string& fileName);

	// Getter functions                                                   // �Q�b�^�[
	size_t GetNumBones() const { return mBones.size(); }                  // �{�[�����̎擾
	const Bone& GetBone(size_t idx) const { return mBones[idx]; }         // id�ɂ��{�[���̎擾
	const std::vector<Bone>& GetBones() const { return mBones; }          // �{�[���z��̎擾
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; } // �O���[�o���t�o�C���h�s��z��̎擾

protected:
	// �X�P���g�������[�h���ꂽ�Ƃ��Ɏ����I�ɌĂяo�����   
	// �e�{�[���̃O���[�o���t�o�C���h�|�[�Y���v�Z
	void ComputeGlobalInvBindPose();

private:

	// �{�[���z��
	std::vector<Bone> mBones;
	// ���ꂼ��̃{�[���̋t�o�C���h�|�[�Y�s��
	std::vector<Matrix4> mGlobalInvBindPoses;
};
