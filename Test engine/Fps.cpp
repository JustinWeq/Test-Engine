#include "Fps.h"

namespace JR_Fps
{
	//defualt constructor-- creates a new instance of Fps with defualt parameters
	Fps::Fps()
	{
		m_count = 0;
		m_fps = 0;
		m_startTime = 0;
	}

	//deconstructor-- cleans up memory for this Fps instance
	Fps::~Fps()
	{
		//no dynamic memory so theres nothing to delete
	}

	//init-- initializes the Fps class
	void Fps::init()
	{
		m_fps = 0;
		m_count = 0;
		m_startTime = timeGetTime();
	}

	//frame-- updates the Fps class
	void Fps::frame()
	{
		m_count++;
		if (timeGetTime() >= (m_startTime + 1000))
		{
			m_fps = m_count;
			m_count = 0;

			m_startTime = timeGetTime();
		}
	}

	//getFps-- returns 
	int Fps::getFps()
	{
		return m_fps;
	}
}