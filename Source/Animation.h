#pragma once
#include "Math.h"
#include "BoneTransform.h"
#include <vector>
#include <string>

class Animation
{

public:

	bool Load(const std::string& in_fileName, bool loop);                             // アニメーション読み込み

	size_t GetNumBones() const { return m_numBones; }                                 // ボーン数の取得
	size_t GetNum_frames() const { return m_numFrames; }                              // フレーム数の取得
	float  GetDuration() const { return m_duration; }                                 // アニメーション期間の取得
	float  GetFrameDuration() const { return m_frameDuration; }                       // フレーム期間の取得
	bool   IsLoopAnimation() const { return m_isLoopAnimation; }                      // アニメーションはループアニメーションか？

	// 指定された配列を、アニメーションの指定された時間に、
	// 各ボーンのグローバル（現在の）ポーズマトリックスで埋めます。
	// 時間は > = 0.0fかつ <= 期間であると予想される
	void GetGlobalPoseAtTime(std::vector<Matrix4>& out_Poses, const class Skeleton* in_skeleton, float in_time) const;

private:

	// アニメーションのためのボーン数
	size_t m_numBones;

	// アニメーションのフレーム数
	size_t m_numFrames;
	
	// アニメーションの再生時間
	float m_duration;
	
	// アニメーションのフレーム間の時刻
	float m_frameDuration;
	
	// トラック上の各フレームの変換情報。
	// 外側のベクトルの各インデックスはボーン、
	// 内側のベクトルはフレーム　　　　　　　mTracks[ボーン][フレーム数]
	std::vector<std::vector<BoneTransform>> m_tracks;
	bool m_isLoopAnimation;                                       // ループアニメーションするか

};
