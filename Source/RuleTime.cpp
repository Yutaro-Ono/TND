#include "RuleTime.h"
#include "GameMain.h"
#include "GameScene.h"

const float RuleTime::START_TIME = 15.0f;
const float RuleTime::LIMIT_TIME = 210.0f;
const float RuleTime::FINISH_TIME = 3.0f;

RuleTime::RuleTime(class GameScene* in_scene)
	:m_nowCount(START_TIME)
	,m_state(STATE_FREE)
	,m_isAddingFrame(0.0f)
	,m_isAdding(false)
	,m_nextState(false)
	,m_gameEnd(false)
	,m_scene(in_scene)
{
	m_lastTime = SDL_GetTicks() / 1000;
}

RuleTime::~RuleTime()
{
}

void RuleTime::UpdateActor(float in_deltaTime)
{

	if (m_scene->GetState() == m_scene->STATE_FREE)
	{
		// 現在時刻を取得
		m_lastTime = SDL_GetTicks();
	}

	else if (m_scene->GetState() == m_scene->STATE_START)
	{
		m_state = STATE_START;

		// 現在時刻を更新
		m_currentTime = SDL_GetTicks();

		// 1秒経過ごとに1減らす
		if (m_currentTime > m_lastTime + 1000)
		{
			m_nowCount -= 1.0f;
			// 最終時刻を更新
			m_lastTime = m_currentTime;
			
		}

		if(m_nowCount < 0.0f)
		{
			m_nextState = true;
			m_state = STATE_GAME;
		}
	}

	else if (m_scene->GetState() == m_scene->STATE_IN_GAME)
	{
		m_state = STATE_GAME;

		// 前のステートから移行してきたら
		if (m_nextState == true)
		{
			m_nowCount = LIMIT_TIME;     // 制限時間更新
			m_lastTime = SDL_GetTicks();
			m_nextState = false;
		}

		// 現在時刻を更新
		m_currentTime = SDL_GetTicks();

		// カウントダウン
		// 1秒経過ごとに1減らす
		if (m_currentTime > m_lastTime + 1000)
		{
			m_nowCount -= 1.0f;
			// 最終時刻を更新
			m_lastTime = m_currentTime;

		}
		
		if(m_nowCount < 0.0f)
		{
			m_nextState = true;
			m_state = STATE_FINISH;
		}

		// 経過時間追加の表示
		if (m_isAdding == false)
		{
			m_isAddingFrame = 0.0f;
		}
		if (m_isAdding == true)
		{
			m_isAddingFrame += 0.4f * in_deltaTime;

			if (m_isAddingFrame >= 1.0f)
			{
				m_isAdding = false;
			}
		}
	}

	else if (m_scene->GetState() == m_scene->STATE_FINISH)
	{
		if (m_nextState == true)
		{
			m_nowCount = FINISH_TIME;
			m_nextState = false;
		}

		// カウントダウン
		if (m_nowCount > 0.0f)
		{
			m_nowCount -= 0.4f * GAME_INSTANCE.GetDeltaTime();
			// printf("Timer : %f\n", m_nowCount);
		}
		else
		{
			m_gameEnd = true;
		}
	}

}

void RuleTime::AddCount(float in_addValue)
{
	m_isAdding = true;
	m_nowCount += in_addValue;
}

