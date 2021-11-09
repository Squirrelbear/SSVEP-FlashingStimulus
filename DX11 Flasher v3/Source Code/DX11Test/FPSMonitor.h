#ifndef _FPSMonitor_H_
#define _FPSMonitor_H_

class FpsMonitor
{
public:
	FpsMonitor();
	~FpsMonitor();

	void Update(double time);
	void frameOccurence();

	void SetFPS(double);
	double GetFPS();

	void SetFrameTime(double);
	double GetFrameTime();

private:
	double m_clock;
	int m_frames;
	double m_value;
	double m_frameTime;
};

#endif