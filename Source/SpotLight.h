//-------------------------------------------------+
// スポットライトアクター
// 2020 YutaroOno.
//-------------------------------------------------+
#pragma once
#include "Actor.h"

class SpotLight : public Actor
{
public:

	// ライトボリューム (ライト影響範囲)
	enum LIGHT_VOLUME
	{
		VL_SMALL = 0,
		VL_VERY_SMALL,
		VL_MEDIUM,
		VL_BIG
	};

	SpotLight(const Vector3& in_pos, LIGHT_VOLUME in_vol = VL_MEDIUM);
	~SpotLight();

	void SetLightColor(const Vector3& in_diffuse, const Vector3& in_specular);          // ライトカラーのセット

	void UpdateActor(float in_deltaTime) override;

	void SwitchLighting();                                                              // 点灯状態ON・OFF


	//-------------------------------------------+
	// Setter / Getter
	//-------------------------------------------+
	// ターゲットアクターのセット
	void SetTargetActor(class Actor* in_target) { m_target = in_target; }
	// ライト方向の取得・セット
	const Vector3& GetLightDirection() { return m_direction; }
	void SetLightDirection(const Vector3& in_dir) { m_direction = in_dir; }
	// 各種カラーの取得
	const Vector3& GetDiffuseColor() { return m_diffuse; }
	const Vector3& GetAmbientColor() { return m_ambient; }
	const Vector3& GetSpecularColor() { return m_specular; }

	LIGHT_VOLUME GetLightVolume() { return m_lightVolume; }     // ライト影響範囲のゲッター

	void SetLuminance(float in_luminance) { m_luminance = in_luminance; }
	float GetLuminance() { return m_luminance; }

	void SetFlash(bool in_flash) { m_flash = in_flash; }        // 点滅させるかさせないかの切り替え

private:

	class Actor* m_target;

	LIGHT_VOLUME m_lightVolume;      // ライト影響範囲 (ポイントライトコンポーネントにて使用)

	bool m_lighting;                 // 点灯状態

	Vector3 m_direction;

	// カラー情報
	Vector3 m_diffuse;
	Vector3 m_ambient;
	Vector3 m_specular;

	// 輝度情報
	float m_luminance;

	bool m_calcRadius;               // 半径を再計算するか

	bool m_flash;                    // 点滅させるか
	float m_flashOffset;             // フラッシュの間隔

	// ポイントライトコンポーネント
	class SpotLightComponent* m_lightComp;
};