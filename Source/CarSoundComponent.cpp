#include "CarSoundComponent.h"
#include "GameMain.h"

// コンストラクタ
CarSoundComponent::CarSoundComponent(PlayerCar* in_owner)
	:Component(in_owner)
	,m_car(in_owner)
	,m_isStop(false)
{
	LoadSound();
}

// デストラクタ
CarSoundComponent::~CarSoundComponent()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="in_deltaTime"></param>
void CarSoundComponent::Update(float in_deltaTime)
{
	// 停止フラグがオフの場合のみ
	// 更新を許可
	if (!m_isStop)
	{
		// アクセル値・ブレーキ値取得
		float accel = m_car->GetAccelValue();
		float brake = m_car->GetBrakeValue();

		// エンジン音の再生
		PlayEngineSound(accel - brake);
	}
}

void CarSoundComponent::PlayEngineSound(float in_speed)
{
	// 速度の基準
	float slowSpeed = 10.0f;
	float normSpeed = 80.0f;
	float fastSpeed = 100.0f;

	// アクセルを踏んでいなかったら
	if (m_car->GetDriveState() != PlayerCar::DRIVE_ACCEL)
	{
		StopEngineSound();
		return;
	}

	// 速度が0↑かつ50↓
	if (in_speed > slowSpeed && in_speed < normSpeed)
	{
		// 他のエンジン音を停止
		AUDIO->StopSound(m_sound["ENGINE_NORMAL"]);
		AUDIO->StopSound(m_sound["ENGINE_FAST"]);

		// SLOWエンジン音を再生し続ける
		if (!AUDIO->IsPlayingSound(m_sound["ENGINE_SLOW"]))
		{
			AUDIO->PlaySoundTND(m_sound["ENGINE_SLOW"]);
		}
		
		return;
	}

	// 速度が50↑かつ100↓
	if (in_speed >= slowSpeed && in_speed < fastSpeed)
	{
		// 他のエンジン音を停止
		AUDIO->StopSound(m_sound["ENGINE_SLOW"]);
		AUDIO->StopSound(m_sound["ENGINE_FAST"]);

		// SLOWエンジン音を再生し続ける
		if (!AUDIO->IsPlayingSound(m_sound["ENGINE_NORMAL"]))
		{
			AUDIO->PlaySoundTND(m_sound["ENGINE_NORMAL"]);
		}

		return;
	}

	// 速度が100↑
	if (in_speed >= fastSpeed)
	{
		// 他のエンジン音を停止
		AUDIO->StopSound(m_sound["ENGINE_SLOW"]);
		AUDIO->StopSound(m_sound["ENGINE_NORMAL"]);

		// SLOWエンジン音を再生し続ける
		if (!AUDIO->IsPlayingSound(m_sound["ENGINE_FAST"]))
		{
			AUDIO->PlaySoundTND(m_sound["ENGINE_FAST"]);
		}

		return;
	}


}

/// <summary>
/// 全エンジン音停止
/// </summary>
void CarSoundComponent::StopEngineSound()
{
	AUDIO->StopSound(m_sound["ENGINE_SLOW"]);
	AUDIO->StopSound(m_sound["ENGINE_NORMAL"]);
	AUDIO->StopSound(m_sound["ENGINE_FAST"]);
}

/// <summary>
/// エンジン開始時 (プレイヤーが車に乗り込んだ時)効果音再生処理
/// 親クラスから指示
/// </summary>
void CarSoundComponent::PlayEngineStart()
{
	if (AUDIO->IsPlayingSound(m_sound["ENGINE_START"]))
	{
		AUDIO->StopSound(m_sound["ENGINE_START"]);
		AUDIO->PlaySoundTND(m_sound["ENGINE_START"]);
	}
	else
	{
		AUDIO->PlaySoundTND(m_sound["ENGINE_START"]);
	}
	
}

void CarSoundComponent::PlayCrushMed()
{
	AUDIO->PlaySoundTND(m_sound["CRUSH_MEDIUM"]);
}

void CarSoundComponent::PlayCrushHard()
{
	AUDIO->PlaySoundTND(m_sound["CRUSH_HARD"]);
}

/// <summary>
/// 効果音のロード
/// </summary>
void CarSoundComponent::LoadSound()
{
	// エンジン開始音
	m_sound["ENGINE_START"] = "Data/Music/SE/TND/Car/SUV/start_SUV.WAV";
	AUDIO->GetSound(m_sound["ENGINE_START"]);
	// 走行エンジン音(SLOW)
	m_sound["ENGINE_SLOW"] = "Data/Music/SE/TND/Car/SUV/SUVLowEngine.WAV";
	AUDIO->GetSound(m_sound["ENGINE_SLOW"]);
	// 走行エンジン音 (NORMAL)
	m_sound["ENGINE_NORMAL"] = "Data/Music/SE/TND/Car/SUV/SUVNormalEngine1.WAV";
	AUDIO->GetSound(m_sound["ENGINE_NORMAL"]);
	// 走行エンジン音 (FAST)
	m_sound["ENGINE_FAST"] = "Data/Music/SE/TND/Car/SUV/SUVFastEngine1.WAV";
	AUDIO->GetSound(m_sound["ENGINE_FAST"]);

	// クラッシュ(衝突)時の効果音
	m_sound["CRUSH_MEDIUM"] = "Data/Music/SE/TND/Car/crush/medium1.WAV";
	AUDIO->GetSound(m_sound["CRUSH_MEDIUM"]);
	m_sound["CRUSH_HARD"] = "Data/Music/SE/TND/Car/crush/high2.WAV";
	AUDIO->GetSound(m_sound["CRUSH_HARD"]);

}
