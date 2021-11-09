#include "AdaptiveTimer.h"

AdaptiveTimer::AdaptiveTimer()
{
}

AdaptiveTimer::AdaptiveTimer(const AdaptiveTimer&)
{
}

AdaptiveTimer::~AdaptiveTimer()
{
}

bool AdaptiveTimer::Initialize(float phaseOnePercent, float intervalHz)
{
	m_phaseOnePercent = phaseOnePercent;
	SetIntervalHz(intervalHz);

	m_isPhaseOne = true;
	m_clock = 0;
	m_edgeTriggered = false;

	return true;
}

void AdaptiveTimer::Shutdown()
{
}


bool AdaptiveTimer::Update(int time)
{
	// a "fix"
	if(m_clock > GetPhaseOne() * 2 && m_clock > GetPhaseTwo() * 2)
		m_clock = 0;

	m_clock -= time/5000.0;
	m_edgeTriggered = false;

	if(m_clock <= 0)
	{
		m_isPhaseOne = !m_isPhaseOne;
		m_edgeTriggered = true;

		// clear in case of crazy numbers
		if((IsPhaseOne() && m_clock + GetPhaseOne() < 0)
			|| (!IsPhaseOne() && m_clock + GetPhaseTwo() < 0))
			m_clock = 0;

		if(IsPhaseOne())
		{
			m_clock += GetPhaseOne();
		}
		else
		{
			m_clock += GetPhaseTwo();
		}
	}

	return true;
}

void AdaptiveTimer::SetPhases(int phaseOneInterval, int phaseTwoInterval)
{
	m_phaseOne = phaseOneInterval;
	m_phaseTwo = phaseTwoInterval;
	m_interval = m_phaseOne + m_phaseTwo;
	m_intervalHz = MillisecondstoHz(m_interval);
	m_phaseOnePercent = m_phaseOne * 1.0f / m_interval;
	m_clock = 0;
}


void AdaptiveTimer::SetPhaseOne(int phaseOneInterval)
{
	m_phaseOne = phaseOneInterval;
	m_interval = m_phaseOne + m_phaseTwo;
	m_intervalHz = MillisecondstoHz(m_interval);
	m_phaseOnePercent = m_phaseOne * 1.0f / m_interval;
	m_clock = 0;
}

void AdaptiveTimer::SetPhaseTwo(int phaseTwoInterval)
{
	m_phaseTwo = phaseTwoInterval;
	m_interval = m_phaseOne + m_phaseTwo;
	m_intervalHz = MillisecondstoHz(m_interval);
	m_phaseOnePercent = m_phaseOne * 1.0f / m_interval;
	m_clock = 0;
}

int AdaptiveTimer::GetPhaseOne()
{
	return m_phaseOne;
}

int AdaptiveTimer::GetPhaseTwo()
{
	return m_phaseTwo;
}

bool AdaptiveTimer::IsPhaseOne()
{
	return m_isPhaseOne;
}

void AdaptiveTimer::SetPhaseOnePercent(float phaseOnePercent)
{
	m_phaseOnePercent = phaseOnePercent;
	m_phaseOne = (int)(m_interval * m_phaseOnePercent);
	m_phaseTwo = m_interval - m_phaseOne;
	m_clock = 0;
}

void AdaptiveTimer::SetPhaseTwoPercent(float phaseTwoPercent)
{
	m_phaseOnePercent = 1.0 - phaseTwoPercent;
	m_phaseOne = (int)(m_interval * m_phaseOnePercent);
	m_phaseTwo = m_interval - m_phaseOne;
	m_clock = 0;
}

float AdaptiveTimer::GetPhaseOnePercent()
{
	return m_phaseOnePercent;
}

float AdaptiveTimer::GetPhaseTwoPercent()
{
	return 1.0f - m_phaseOnePercent;
}

void AdaptiveTimer::SetInterval(int interval)
{
	m_interval = interval;
	m_intervalHz = MillisecondstoHz(m_interval);
	m_phaseOne = (int)(m_interval * m_phaseOnePercent);
	m_phaseTwo = m_interval - m_phaseOne;
	m_clock = 0;
}

int AdaptiveTimer::GetInterval()
{
	return m_interval;
}

void AdaptiveTimer::SetIntervalHz(float Hz)
{
	m_intervalHz = Hz;
	m_interval = (int)HztoMilliseconds(Hz);
	m_phaseOne = (int)(m_interval * m_phaseOnePercent);
	m_phaseTwo = m_interval - m_phaseOne;
	m_clock = 0;
}

float AdaptiveTimer::GetIntervalHz()
{
	return m_intervalHz;
}

bool AdaptiveTimer::IsEdgeTriggered()
{
	return m_edgeTriggered;
}

float HztoMilliseconds(float Hz)
{
	return 1000.0f / Hz;
}

float MillisecondstoHz(float ms)
{
	return 1000.0f / ms;
}


