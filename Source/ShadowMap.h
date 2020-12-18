#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Math.h"

class ShadowMap
{
public:


	ShadowMap();
	~ShadowMap();

	// 光の視点からシーンをレンダリングしデプスバッファを得る
	void RenderDepthMapFromLightView(const std::vector<class MeshComponent*>& in_mesh, 
		                             const std::vector<class SkeletalMeshComponent*> in_skelMesh,
		                             const std::vector<class CarMeshComponent*> in_carMesh);          // 通常メッシュ・スキンメッシュ・車メッシュ



	void DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh);
	void DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*>& in_skelMesh);

	// Getter/Setter
	class Shader* GetDepthShader() { return m_depthShader; }
	class Shader* GetShadowShader() { return m_shadowShader; }
	unsigned int GetDepthMap() { return m_depthMap; }

private:


	unsigned int m_depthMapFBO;              // デプスバッファオブジェクト
	unsigned int m_depthMap;                 // 深度を書き込むための2Dテクスチャ

	class Shader* m_depthShader;          // デプスシェーダ (影描画の前準備に使用する)
	class Shader* m_depthSkinShader;      // デプスシェーダ (スキンメッシュ用)
	class Shader* m_shadowShader;         // シャドウマッピングシェーダ
	class Shader* m_skinShadowShader;     // シャドウ適用のスキンシェーダ

	// ライト空間用行列
	Matrix4 m_lightView, m_lightProj, m_lightSpace;


	// シャドウマップサイズ
	static const int SHADOW_WIDTH;
	static const int SHADOW_HEIGHT;

};