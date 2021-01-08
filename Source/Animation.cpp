#include "Animation.h"
#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>


// �A�j���[�V�����̓ǂݍ���
bool Animation::Load(const std::string& in_fileName, bool loop)
{
	// �A�j���[�V�������[�v�����邩�H
	m_isLoopAnimation = loop;

	// filename����e�L�X�g�t�@�C���Ƃ��ēǂݍ��݁ArapidJSON�ɉ�͂�����
	std::ifstream file(in_fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Animation %s", in_fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	// JSON�I�u�W�F�N�g���H
	if (!doc.IsObject())
	{
		SDL_Log("Animation %s is not valid json", in_fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// ���^�f�[�^�̃`�F�b�N�B�o�[�W�����͂P���H
	if (ver != 1)
	{
		SDL_Log("Animation %s unknown format", in_fileName.c_str());
		return false;
	}

	// "sequece"���ǂݍ��߂邩�H
	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		SDL_Log("Animation %s doesn't have a sequence.", in_fileName.c_str());
		return false;
	}

	// "frames" "length" "bonecount"�͂��邩�H
	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];

	if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint())
	{
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", in_fileName.c_str());
		return false;
	}

	// �t���[�����A�A�j���[�V�������ԁA�{�[�����A�t���[��������̎��Ԃ��擾
	m_numFrames = frames.GetUint();
	m_duration = static_cast<float>(length.GetDouble());
	m_numBones = bonecount.GetUint();
	m_frameDuration = m_duration / (m_numFrames - 1);

	// �g���b�N�z����m��
	m_tracks.resize(m_numBones);

	// �g���b�N�z�񂪎擾�ł��邩�H
	const rapidjson::Value& tracks = sequence["tracks"];

	if (!tracks.IsArray())
	{
		SDL_Log("Sequence %s m_issing a tracks array.", in_fileName.c_str());
		return false;
	}

	// �g���b�N�������[�v
	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		// tracs[i]�̓I�u�W�F�N�g���H
		if (!tracks[i].IsObject())
		{
			SDL_Log("Animation %s: Track element %d is invalid.", in_fileName.c_str(), i);
			return false;
		}

		// tracks[i]�̒��� "bone"��uint�œǂݍ��݁B�{�[���ԍ����擾
		size_t boneIndex = tracks[i]["bone"].GetUint();

		// tracks[i]�̒��� "transforms"���擾�ł��邩�H
		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray())
		{
			SDL_Log("Animation %s: Track element %d is m_issing transforms.", in_fileName.c_str(), i);
			return false;
		}

		BoneTransform temp;
		// transform�̃T�C�Y�ƃA�j���[�V�����t���[�������s��Ȃ����H
		if (transforms.Size() < m_numFrames)
		{
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", in_fileName.c_str(), i);
			return false;
		}

		// transforms�̃T�C�Y�����[�v�B�{�[���ԍ�boneIndex�̕ϊ����Ƃ��Ď�荞��
		int transSize = transforms.Size();
		for (int j = 0; j < transSize; j++)
		{
			// ���[�e�[�V����(quaternion)��trans(���s�ړ�����)��ǂݍ���
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray())
			{
				SDL_Log("Skeleton %s: Bone %d is invalid.", in_fileName.c_str(), i);
				return false;
			}

			// temp.mRotation�Ɂ@quaternion�Ƃ��ăR�s�[�A
			temp.mRotation.x = static_cast<float>(rot[0].GetDouble());
			temp.mRotation.y = static_cast<float>(rot[1].GetDouble());
			temp.mRotation.z = static_cast<float>(rot[2].GetDouble());
			temp.mRotation.w = static_cast<float>(rot[3].GetDouble());

			// temp.mTranslation�ɕ��s�ړ������Ƃ��ăR�s�[
			temp.mTranslation.x = static_cast<float>(trans[0].GetDouble());
			temp.mTranslation.y = static_cast<float>(trans[1].GetDouble());
			temp.mTranslation.z = static_cast<float>(trans[2].GetDouble());

			// �{�[���ԍ�boneIndex�̎p���f�[�^�Ƃ��āAm_tracks�z��ɓ����B
			m_tracks[boneIndex].emplace_back(temp);
			//m_tracks[i].emplace_back(temp);

		}
	}

	return true;
}

// inTime�������_�̃O���[�o���|�[�Y�z��̎擾
void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& out_poses, const Skeleton* in_skeleton, float in_time) const
{
	if (out_poses.size() != m_numBones)
	{
		out_poses.resize(m_numBones);
	}


	// ���݂̃t���[���Ǝ��̃t���[���������o���B
	// �����inTime�� [0�`Anim_duration] �̊Ԃɂ��邱�Ƃ�O��Ƃ��Ă���B
	size_t frame = static_cast<size_t>(in_time / m_frameDuration);
	size_t nextFrame = frame + 1;

	// �t���[���Ǝ��̃t���[���̊Ԃ̏����l���v�Z
	float pct = in_time / m_frameDuration - frame;

	// ���[�g�̃|�[�Y���Z�b�g�A�b�v
	if (m_tracks[0].size() > 0)
	{
		// ���݂̃t���[���̃|�[�Y�Ǝ��̃t���[���̊Ԃ��Ԃ���
		BoneTransform interp = BoneTransform::Interpolate(m_tracks[0][frame],
			m_tracks[0][nextFrame % m_numFrames], pct);
		out_poses[0] = interp.ToMatrix();
	}
	else
	{
		out_poses[0] = Matrix4::Identity;
	}

	const std::vector<Skeleton::Bone>& bones = in_skeleton->GetBones();

	// �c��̃|�[�Y��ݒ肵�܂�
	for (size_t bone = 1; bone < m_numBones; bone++)
	{
		Matrix4 localMat;     // �f�t�H���g�͒P�ʍs��
		if (m_tracks[bone].size() > 0)
		{
			// [bone][frame]�̃{�[���p����[bone][nextframe]�� �����_�ȉ��� pct�ŕ�Ԃ����p���� interp�ɎZ�o
			BoneTransform interp = BoneTransform::Interpolate(m_tracks[bone][frame],
				m_tracks[bone][nextFrame % m_numFrames], pct);
			// interp ���s��ɕϊ����āAlocalMat�ɕϊ�����
			localMat = interp.ToMatrix();
		}

		// �o�̓|�[�Y�s��[bone] = ���[�J���|�[�Y�s�� * �o�̓|�[�Y�s��[�ebone]
		out_poses[bone] = localMat * out_poses[bones[bone].mParent];
	}
}
