#pragma once

#include <chrono>

class Clock
{
public:
	Clock();
	~Clock();

	double UpdateTime();

	void Update();

	double Get_DT() { return m_dt; }
	double Get_Time_Passed() { return m_time_passed; }
	double GetFPS() { return fps; }

private:
	double m_time_passed = 0;
	double m_dt = 0;
	double last_time;
	double start_time;
	double fps;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	int frames;
};

