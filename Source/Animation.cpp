#include "Animation.h"
#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>


// アニメーションの読み込み
bool Animation::Load(const std::string& in_fileName, bool loop)
{
	// アニメーションループさせるか？
	m_isLoopAnimation = loop;

	// filenameからテキストファイルとして読み込み、rapidJSONに解析させる
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

	// JSONオブジェクトか？
	if (!doc.IsObject())
	{
		SDL_Log("Animation %s is not valid json", in_fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// メタデータのチェック。バージョンは１か？
	if (ver != 1)
	{
		SDL_Log("Animation %s unknown format", in_fileName.c_str());
		return false;
	}

	// "sequece"情報読み込めるか？
	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		SDL_Log("Animation %s doesn't have a sequence.", in_fileName.c_str());
		return false;
	}

	// "frames" "length" "bonecount"はあるか？
	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];

	if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint())
	{
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", in_fileName.c_str());
		return false;
	}

	// フレーム数、アニメーション時間、ボーン数、フレームあたりの時間を取得
	m_numFrames = frames.GetUint();
	m_duration = static_cast<float>(length.GetDouble());
	m_numBones = bonecount.GetUint();
	m_frameDuration = m_duration / (m_numFrames - 1);

	// トラック配列を確保
	m_tracks.resize(m_numBones);

	// トラック配列が取得できるか？
	const rapidjson::Value& tracks = sequence["tracks"];

	if (!tracks.IsArray())
	{
		SDL_Log("Sequence %s m_issing a tracks array.", in_fileName.c_str());
		return false;
	}

	// トラック数分ループ
	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		// tracs[i]はオブジェクトか？
		if (!tracks[i].IsObject())
		{
			SDL_Log("Animation %s: Track element %d is invalid.", in_fileName.c_str(), i);
			return false;
		}

		// tracks[i]の中の "bone"をuintで読み込み。ボーン番号を取得
		size_t boneIndex = tracks[i]["bone"].GetUint();

		// tracks[i]の中の "transforms"が取得できるか？
		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray())
		{
			SDL_Log("Animation %s: Track element %d is m_issing transforms.", in_fileName.c_str(), i);
			return false;
		}

		BoneTransform temp;
		// transformのサイズとアニメーションフレーム数が不具合ないか？
		if (transforms.Size() < m_numFrames)
		{
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", in_fileName.c_str(), i);
			return false;
		}

		// transformsのサイズ分ループ。ボーン番号boneIndexの変換情報として取り込む
		int transSize = transforms.Size();
		for (int j = 0; j < transSize; j++)
		{
			// ローテーション(quaternion)とtrans(平行移動成分)を読み込む
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray())
			{
				SDL_Log("Skeleton %s: Bone %d is invalid.", in_fileName.c_str(), i);
				return false;
			}

			// temp.mRotationに　quaternionとしてコピー、
			temp.mRotation.x = static_cast<float>(rot[0].GetDouble());
			temp.mRotation.y = static_cast<float>(rot[1].GetDouble());
			temp.mRotation.z = static_cast<float>(rot[2].GetDouble());
			temp.mRotation.w = static_cast<float>(rot[3].GetDouble());

			// temp.mTranslationに平行移動成分としてコピー
			temp.mTranslation.x = static_cast<float>(trans[0].GetDouble());
			temp.mTranslation.y = static_cast<float>(trans[1].GetDouble());
			temp.mTranslation.z = static_cast<float>(trans[2].GetDouble());

			// ボーン番号boneIndexの姿勢データとして、m_tracks配列に入れる。
			m_tracks[boneIndex].emplace_back(temp);
			//m_tracks[i].emplace_back(temp);

		}
	}

	return true;
}

// inTime時刻時点のグローバルポーズ配列の取得
void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& out_poses, const Skeleton* in_skeleton, float in_time) const
{
	if (out_poses.size() != m_numBones)
	{
		out_poses.resize(m_numBones);
	}


	// 現在のフレームと次のフレームを見つけ出す。
	// これはinTimeが [0～Anim_duration] の間にいることを前提としている。
	size_t frame = static_cast<size_t>(in_time / m_frameDuration);
	size_t nextFrame = frame + 1;

	// フレームと次のフレームの間の小数値を計算
	float pct = in_time / m_frameDuration - frame;

	// ルートのポーズをセットアップ
	if (m_tracks[0].size() > 0)
	{
		// 現在のフレームのポーズと次のフレームの間を補間する
		BoneTransform interp = BoneTransform::Interpolate(m_tracks[0][frame],
			m_tracks[0][nextFrame % m_numFrames], pct);
		out_poses[0] = interp.ToMatrix();
	}
	else
	{
		out_poses[0] = Matrix4::Identity;
	}

	const std::vector<Skeleton::Bone>& bones = in_skeleton->GetBones();

	// 残りのポーズを設定します
	for (size_t bone = 1; bone < m_numBones; bone++)
	{
		Matrix4 localMat;     // デフォルトは単位行列
		if (m_tracks[bone].size() > 0)
		{
			// [bone][frame]のボーン姿勢と[bone][nextframe]を 小数点以下の pctで補間した姿勢を interpに算出
			BoneTransform interp = BoneTransform::Interpolate(m_tracks[bone][frame],
				m_tracks[bone][nextFrame % m_numFrames], pct);
			// interp を行列に変換して、localMatに変換する
			localMat = interp.ToMatrix();
		}

		// 出力ポーズ行列[bone] = ローカルポーズ行列 * 出力ポーズ行列[親bone]
		out_poses[bone] = localMat * out_poses[bones[bone].mParent];
	}
}
