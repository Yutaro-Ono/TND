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
		// ���ݎ������擾
		m_lastTime = SDL_GetTicks();
	}

	else if (m_scene->GetState() == m_scene->STATE_START)
	{
		m_state = STATE_START;

		// ���ݎ������X�V
		m_currentTime = SDL_GetTicks();

		// 1�b�o�߂��Ƃ�1���炷
		if (m_currentTime > m_lastTime + 1000)
		{
			m_nowCount -= 1.0f;
			// �ŏI�������X�V
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

		// �O�̃X�e�[�g����ڍs���Ă�����
		if (m_nextState == true)
		{
			m_nowCount = LIMIT_TIME;     // �������ԍX�V
			m_lastTime = SDL_GetTicks();
			m_nextState = false;
		}

		// ���ݎ������X�V
		m_currentTime = SDL_GetTicks();

		// �J�E���g�_�E��
		// 1�b�o�߂��Ƃ�1���炷
		if (m_currentTime > m_lastTime + 1000)
		{
			m_nowCount -= 1.0f;
			// �ŏI�������X�V
			m_lastTime = m_currentTime;

		}
		
		if(m_nowCount < 0.0f)
		{
			m_nextState = true;
			m_state = STATE_FINISH;
		}

		// �o�ߎ��Ԓǉ��̕\��
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

		// �J�E���g�_�E��
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

