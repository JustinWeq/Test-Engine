#pragma once
#pragma comment(lib,"winmm.lib")

//includes
#include <Windows.h>
#include <mmsystem.h>

namespace JR_Fps
{
	//Fps-- a simple class that is used for counting the Fps
	class Fps
	{
	public:
		//defualt constructor-- creates a new instance of Fps with defualt parameters
		Fps();
		//deconstructor-- cleans up memory for this Fps instance
		~Fps();

		//init-- initializes the Fps class
		void init();
		//frame-- updates the Fps class
		void frame();
		//getFps-- returns 
		int getFps();

	private:
		//fps- the fps for the last frame
		int m_fps;
		//count- the number of frames
		int m_count;
		//startTime- the time the timer started
		unsigned long m_startTime;
	}
}