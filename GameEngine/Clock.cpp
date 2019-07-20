#include "Clock.h"


Clock::Clock()
{
	begin = std::chrono::steady_clock::now();
}


Clock::~Clock()
{
}

double Clock::UpdateTime()
{
	time_t timer;
	struct tm y2k = { 0 };

	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */

	return difftime(timer, mktime(&y2k));
}

void Clock::Update()
{
	end = std::chrono::steady_clock::now();
	frames++;

	last_time = UpdateTime();
	m_dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0f;
	m_time_passed += m_dt;

	begin = std::chrono::steady_clock::now();

	if (last_time - start_time > 6.25 && frames > 10)
	{
		fps = (double)frames / (last_time - start_time);
		start_time = last_time;
		frames = 0;
	}
}
