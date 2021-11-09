#include "FPSMonitor.h"

FpsMonitor::FpsMonitor()
{
	m_clock = 0;
	m_frames = 0;
	m_value = 0;
	m_frameTime = 0;
}

FpsMonitor::~FpsMonitor()
{
}

void FpsMonitor::Update(double time)
{
	m_clock += time;
	if(m_clock >= 1000)
	{
		m_value = (m_frames / (m_clock / 1000.0));
		m_frameTime = m_clock / m_frames;
		m_clock = 0;
		m_frames = 0;
	}
}

void FpsMonitor::frameOccurence()
{
	m_frames++;
}

void FpsMonitor::SetFPS(double)
{
	// dummy method
}

double FpsMonitor::GetFPS()
{
	return m_value;
}

void FpsMonitor::SetFrameTime(double)
{
	// dummy method
}

double FpsMonitor::GetFrameTime()
{
	return m_frameTime;
}