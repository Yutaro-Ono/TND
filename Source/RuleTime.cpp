#include "RuleTime.h"
#include "GameMain.h"
#include "GameScene.h"

const float RuleTime::START_TIME = 6.0f;
const float RuleTime::LIMIT_TIME = 61.0f;
const float RuleTime::FINISH_TIME = 2.0f;

RuleTime::RuleTime(class GameScene* in_scene)
	:m_nowCount(START_TIME)
	,m_state(STATE_FREE)
	,m_isAddingFrame(0.0f)
	,m_isAdding(false)
	,m_nextState(false)
	,m_gameEnd(false)
	,m_scene(in_scene)
{
}

RuleTime::~RuleTime()
{
}

void RuleTime::UpdateActor(float in_deltaTime)
{

	if (m_scene->GetState() == m_scene->STATE_FREE)
	{

	}

	else if (m_scene->GetState() == m_scene->STATE_START)
	{
		m_state = STATE_START;

		if (m_nowCount > 0.0f)
		{
			m_nowCount -= 0.4f * GAME_INSTANCE.GetDeltaTime();
		}
		else
		{
			m_nextState = true;
			m_state = STATE_GAME;
		}
	}

	else if (m_scene->GetState() == m_scene->STATE_IN_GAME)
	{
		m_state = STATE_GAME;

		if (m_nextState == true)
		{
			m_nowCount = LIMIT_TIME;
			m_nextState = false;
		}

		// カウントダウン
		if (m_nowCount > 1.0f)
		{
			m_nowCount -= 0.4f * GAME_INSTANCE.GetDeltaTime();
			// printf("Timer : %f\n", m_nowCount);
		}
		else
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


	//switch (m_state)
	//{
	//case STATE_START:

	//	if (m_nowCount > 0.0f)
	//	{
	//		m_nowCount -= 0.4f * GAME_INSTANCE.GetDeltaTime();
	//	}
	//	else
	//	{
	//		m_nextState = true;
	//		m_state = STATE_GAME;
	//	}

	//	break;

	//case STATE_GAME:

	//	if (m_nextState == true)
	//	{
	//		m_nowCount = LIMIT_TIME;
	//		m_nextState = false;
	//	}

	//	// カウントダウン
	//	if (m_nowCount > 1.0f)
	//	{
	//		m_nowCount -= 0.4f * GAME_INSTANCE.GetDeltaTime();
	//		// printf("Timer : %f\n", m_nowCount);
	//	}
	//	else
	//	{
	//		m_nextState = true;
	//		m_state = STATE_FINISH;
	//	}

	//	// 経過時間追加の表示
	//	if (m_isAdding == false)
	//	{
	//		m_isAddingFrame = 0.0f;
	//	}
	//	if (m_isAdding == true)
	//	{
	//		m_isAddingFrame += 0.4f * in_deltaTime;

	//		if (m_isAddingFrame >= 1.0f)
	//		{
	//			m_isAdding = false;
	//		}
	//	}
	//	

	//	break;

	//case STATE_FINISH:

	//	if (m_nextState == true)
	//	{
	//		m_nowCount = FINISH_TIME;
	//		m_nextState = false;
	//	}

	//	// カウントダウン
	//	if (m_nowCount > 0.0f)
	//	{
	//		m_nowCount -= 0.4f * GAME_INSTANCE.GetDeltaTime();
	//		// printf("Timer : %f\n", m_nowCount);
	//	}
	//	else
	//	{
	//		m_gameEnd = true;
	//	}

	//	break;

	//default:

	//	break;
	//}


}

void RuleTime::AddCount(float in_addValue)
{
	m_isAdding = true;
	m_nowCount += in_addValue;
}

