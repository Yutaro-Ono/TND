//------------------------------------------------------------+
// 車(プレイヤー操作)のSE再生コンポーネント
//------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "AudioManager.h"
#include "PlayerCar.h"
#include <unordered_map>

class CarSoundComponent : public Component
{
public:


	CarSoundComponent(class PlayerCar* in_owner);
	~CarSoundComponent();

	void Update(float in_deltaTime) override;

	void PlayEngineSound(float in_speed);     // エンジン音再生

	void StopEngineSound();                   // エンジン音停止

	void PlayEngineStart();                   // 開始エンジン音の再生処理
	
	void PlayCrushMed();                      // 衝突音再生(MEDIUM)
	void PlayCrushHard();                     // 衝突音再生(HARD)

	// 効果音再生の停止セッター
	void SetStop(bool in_stop) { m_isStop = in_stop; }

	

private:

	void LoadSound();    // 効果音のロード



	class PlayerCar* m_car;

	std::unordered_map<std::string, std::string> m_sound;     // 各種効果音

	bool m_isStop;                                            // 効果音再生の停止フラグ

};