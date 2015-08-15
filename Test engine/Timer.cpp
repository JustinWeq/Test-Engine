#include "Timer.h"
namespace JR_Timer
{
	//defualt constructor-- creates a new instance of Timer with defualt parameters
	Timer::Timer()
	{
		m_frameTime = 0;
		m_frequency = 0;
		m_startTime = 0;
		m_ticksPerMs = 0;
	}
	//deconstructor-- cleans up any dynamic memory that the Timer class uses
	Timer::~Timer()
	{
		//no dyna,ic memory to clean up
	}

	//init-- initializes this instance of the timer class
	bool Timer::init()
	{
		//Check to see if this system supports high frequency timers
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
		if (m_frequency == 0)
		{
			return false;
		}

		//Find out how many times the frequency counter ticks every second
		m_ticksPerMs = (float)(m_frequency / 1000);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

		return true;
	}

	//frame-- updates and processes this Timer instance
	void Timer::frame()
	{
		INT64 currentTime;
		float timeDifference;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		timeDifference = (float)(currentTime - m_startTime);
		m_frameTime = timeDifference / m_ticksPerMs;
		m_startTime = currentTime;
	}

	//getTimer-- returns the current amount of time progressed
	float Timer::getTime()
	{
		return m_frameTime;
	}
}