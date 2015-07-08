//pre processor
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
namespace Application
{


	/*
	Windows application class
	defines and maintans a win32 window
	*/
	class App
	{
	public:
		App();
		~App();

		bool init(int screenWidth,int screenHeight,bool fullscreen,LPCWSTR applicationName);
		void shutdown();
		LRESULT CALLBACK MessageHandler(HWND parent, UINT command, WPARAM wparam, LPARAM lparam);
        bool frame();
	private:
	    
		void initWindows();
		void closeWindows();
	private:
		//the name of the win32 application
		LPCWSTR m_applicationName;
		//the hinstance of the window
		HINSTANCE m_hinstance;
		//the hwnd of the window
		HWND m_hwnd;
		//the width of the window
		int m_width;
		//the height of the window
		int m_height;
		//the x position of the window
		int m_posX;
		//the y position of the window
		int m_posY;
		//fullscreen
		bool m_fullscreen;
		//the finished bool
		bool m_finished;
		

	};

	//windows proc
	static LRESULT CALLBACK WndProc(HWND parent, UINT command, WPARAM wparam, LPARAM lparam);

	static bool finished = false;

	//ApplicationHandle-- A pointer to the application
	static App* ApplicationHandle = NULL;
}