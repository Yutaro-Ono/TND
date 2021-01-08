#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include "ShadowMap.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* in_owner)
	:MeshComponent(in_owner, true)
	, m_skeleton(nullptr)
{
}

void SkeletalMeshComponent::Draw(Shader* in_shader)                         // 描画
{
	if (m_mesh && m_visible)
	{
		// ワールド変換行列セット
		in_shader->SetMatrixUniform("u_worldTransform",
			m_owner->GetWorldTransform());
		// 行列パレットをセット    
		in_shader->SetMatrixUniforms("u_matrixPalette", &m_palette.mEntry[0],
			MAX_SKELETON_BONES);
		// スペキュラー強度をセット
		in_shader->SetFloatUniform("u_specPower", 21);

		// 各種テクスチャをシェーダにセットする
		// テクスチャが読み込まれていない場合は無効な数字 "0" が返される
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::DIFFUSE_MAP));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::SPECULAR_MAP));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::NORMAL_MAP));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::EMISSIVE_MAP));
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, RENDERER->GetShadowMap()->GetDepthMap());


		// メッシュの頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::DrawShadow(Shader* in_shader)
{
	if (m_mesh)
	{
		// ワールド変換行列セット
		in_shader->SetMatrixUniform("u_worldTransform",
			m_owner->GetWorldTransform());
		// 行列パレットをセット    
		in_shader->SetMatrixUniforms("u_matrixPalette", &m_palette.mEntry[0],
			MAX_SKELETON_BONES);

		// メッシュの頂点配列をアクティブに
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// 描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float in_deltaTime)
{
	if (m_animation && m_skeleton)
	{
		m_animTime += in_deltaTime * m_animPlayRate;

		// アニメーションがループアニメーションなら巻き戻し処理
		if (m_animation->IsLoopAnimation())
		{
			// アニメを巻き戻して再生
			while (m_animTime > m_animation->GetDuration())
			{
				m_animTime -= m_animation->GetDuration();
			}
		}
		// ループしないアニメで再生時間超えたら最終時間までとする
		else if (m_animTime > m_animation->GetDuration())
		{
			m_animTime = m_animation->GetDuration();
		}
		// 行列パレットの再計算
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)  // アニメーションの再生
{
	m_animation = anim;
	m_animTime = 0.0f;
	m_animPlayRate = playRate;

	if (!m_animation) { return 0.0f; }

	ComputeMatrixPalette();

	return m_animation->GetDuration();
}

// 現在アニメーション再生中か？ true : 再生中 / false : 再生終了
bool SkeletalMeshComponent::IsPlaying()
{
	if (!m_animation->IsLoopAnimation())
	{
		if (m_animTime >= m_animation->GetDuration())
		{
			return false;
		}
	}
	return true;
}

// 行列パレットの計算
void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = m_skeleton->GetGlobalInvBindPoses();   // グローバル逆バインド行列配列の取得
	std::vector<Matrix4> currentPoses;                                                      // 現在のポーズ行列
	m_animation->GetGlobalPoseAtTime(currentPoses, m_skeleton, m_animTime);                 // アニメ時刻時点のグローバルポーズの取得

	// それぞれのボーンの行列パレットのセット
	for (size_t i = 0; i < m_skeleton->GetNumBones(); i++)
	{
		// 行列パレット[i] = グローバル逆バインド行列[i]　×　現在のポーズ行列[i]  (iはボーンID)         
		m_palette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
