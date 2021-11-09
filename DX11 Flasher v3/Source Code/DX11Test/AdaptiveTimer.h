////////////////////////////////////////////////////////////////////////////////
// Filename: AdaptiveTimer.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ADAPTIVETIMER_H_
#define _ADAPTIVETIMER_H_

float HztoMilliseconds(float);
float MillisecondstoHz(float);

////////////////////////////////////////////////////////////////////////////////
// Class name: AdaptiveTimer
////////////////////////////////////////////////////////////////////////////////
class AdaptiveTimer
{
public:
	AdaptiveTimer();
	AdaptiveTimer(const AdaptiveTimer&);
	~AdaptiveTimer();

	bool Initialize(float, float);
	void Shutdown();

	bool Update(int);

	void SetPhases(int,int);
	void SetPhaseOne(int);
	void SetPhaseTwo(int);
	int GetPhaseOne();
	int GetPhaseTwo();
	bool IsPhaseOne();

	void SetPhaseOnePercent(float);
	void SetPhaseTwoPercent(float);
	float GetPhaseOnePercent();
	float GetPhaseTwoPercent();
	void SetInterval(int);
	int GetInterval();

	void SetIntervalHz(float);
	float GetIntervalHz();

	bool IsEdgeTriggered();

private:
	float m_phaseOnePercent;
	int m_interval;
	float m_intervalHz;

	int m_phaseOne;
	int m_phaseTwo;
	bool m_isPhaseOne;
	double m_clock;
	bool m_edgeTriggered;
};

#endif