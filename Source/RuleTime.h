#pragma once
#include "Actor.h"



class RuleTime : public Actor
{
public:

	enum CountState
	{
		STATE_FREE,
		STATE_START,
		STATE_GAME,
		STATE_FINISH
	};

	RuleTime(class GameScene* in_scene);
	~RuleTime();


	void UpdateActor(float in_deltaTime) override;

	CountState GetCountState() { return m_state; }

	const float& GetNowCount() const { return m_nowCount; }

	const float& GetLimitTime() const { return LIMIT_TIME; }

	bool GetGameEnd() { return m_gameEnd; }

	void SetStartTime() { m_nowCount = START_TIME; }      // 制限時間のセッター(開始時)

	void SetLimitTime() { m_nowCount = LIMIT_TIME; }      // 制限時間のセッター(ゲーム中)

	void AddCount(float in_addValue);         // カウント加算

	bool GetIsAdding() { return m_isAdding; }

private:


	unsigned int m_lastTime, m_currentTime;

	float m_nowCount;

	CountState m_state;

	float m_isAddingFrame;

	bool m_isAdding;          // ともし火を取ったことで制限時間が加算された

	bool m_nextState;

	bool m_gameEnd;

	static const float START_TIME;

	static const float LIMIT_TIME;

	static const float FINISH_TIME;

	class GameScene* m_scene;

};