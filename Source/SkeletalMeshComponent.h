#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* in_owner);
	// メッシュコンポーネントの描画
	virtual void Draw(class Shader* in_shader) override;

	// シャドウ用の描画
	virtual void DrawShadow(class Shader* in_shader) override;

	void Update(float in_deltaTime) override;

	// スケルトンのセット
	void SetSkeleton(const class Skeleton* sk) { m_skeleton = sk; }

	// アニメーションの再生。アニメーションの残り長さを返す
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);
	// 現在再生中のアニメーションは再生中か？ true : 再生中 false : 再生終了
	bool IsPlaying();
	float GetPlayTime() { return m_animTime; }

protected:
	void ComputeMatrixPalette();                                           // 行列パレットの計算

	MatrixPalette m_palette;                                                // 行列パレット
	const class Skeleton* m_skeleton;                                       // スケルトン
	const class Animation* m_animation;                                     // アニメーション
	float m_animPlayRate;                                                   // アニメーションの再生速度
	float m_animTime;                                                       // アニメーション時間

};
