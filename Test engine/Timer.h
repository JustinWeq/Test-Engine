#pragma once
#include <Windows.h>

namespace JR_Timer
{
	//Timer-- a class that querys the Cpu to get the current time progressed
	class Timer
	{
	public:
		//defualt constructor-- creates a new instance of Timer with defualt parameters
		Timer();
		//deconstructor-- cleans up any dynamic memory that the Timer class uses
		~Timer();

		//init-- initializes this instance of the timer class
		bool init();

		//frame-- updates and processes this Timer instance
		void frame();

		//getTimer-- returns the current amount of time progressed
		float getTime();
	private:
		//frequency-- the frequency of ticks for this cpu
		INT64 m_frequency;
		//ticksPerMS- the amount fo ticks per Milli second for this CPU
		float m_ticksPerMs;
		//startTime- the time this timer started at
		INT64 m_startTime;
		//frameTime-- the amount of time this frame took
		float m_frameTime;

	};
}