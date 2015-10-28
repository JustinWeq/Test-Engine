#include "App.h"
namespace Application
{
	//defualt constructor-- creates a new App class with defualt parameters
	App::App()
	{
		m_applicationName = L"Application";
		m_hinstance = NULL;
		m_hwnd = NULL;
		m_finished = m_width = m_height = m_fullscreen = 0;
	}

	//deconstructor-- deconstructs and cleans up the App
	App::~App()
	{
		closeWindows();
	}


	//init-- inits and then creates the window with the passed in parameters
	//screenWidth- the width of the application
	//screenHeight- the height of the application
	//fullscreen- boolean parameter indicating whether the app is full screen or not
	//applicationName- the name of the application in both windows and the form
	bool App::init(int screenWidth,int screenHeight,bool fullscreen,LPCSTR applicationName)
	{
		//set the screen width and height
		m_width = screenWidth;
		m_height = screenHeight;

		//set fullscreen
		m_fullscreen = fullscreen;

		//set the applications name
		m_applicationName = applicationName;

		//init windows
		initWindows();
		return true;
	}

	//initWindows-- starts up the windows form
	void App::initWindows()
	{
		//desc of windows class
		WNDCLASSEX windowsClass;
		
		//the screen settings
		DEVMODE screenSettings;
;

		//set up the hinstance
		m_hinstance = GetModuleHandle(NULL);

		//set up the windows class desc with defualt settings.
		windowsClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowsClass.lpfnWndProc = WndProc;
		windowsClass.cbClsExtra = NULL;
		windowsClass.cbWndExtra = NULL;
		windowsClass.hInstance = m_hinstance;
		windowsClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowsClass.hIconSm = windowsClass.hIcon;
		windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowsClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0xFF,0x00,0x00));
		windowsClass.lpszMenuName = m_applicationName;
		windowsClass.lpszClassName = m_applicationName;
		windowsClass.cbSize = sizeof(WNDCLASSEX);

		//register the windows class
		RegisterClassEx(&windowsClass);

		//get the resolution of the clients monitor
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (m_fullscreen)
		{
			//fullscreen was selected so set the screen accordingly
			//clear memory on the screen settings
			memset(&screenSettings, 0, sizeof(screenSettings));
			screenSettings.dmSize = sizeof(screenSettings);
			screenSettings.dmPelsWidth = (unsigned long)screenWidth;
			screenSettings.dmPelsHeight = (unsigned long)screenHeight;
			screenSettings.dmBitsPerPel = 32;
			screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			m_width = screenWidth;
			m_height = screenHeight;

			//change the display settings to fullscreen.
			ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

			//set the position of the window to the top left corner.
			m_posX = m_posY = 0;
		}
		else
		{
			//place the window in the center of the screen
			m_posX = screenWidth/2 - (m_width / 2);
				m_posY = screenHeight/2 - (m_height / 2);
		}

		//create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			m_posX, m_posY, m_width, m_height, NULL, NULL, m_hinstance, NULL);

		//bring the window up to the screen and request the focus
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);

		//hide the mouse
		ShowCursor(false);
	}

	//closeWindows--exits the windows form
	void App::closeWindows()
	{
		//show the mouse cursor
		ShowCursor(true);

		//fix the display settings if leaving fullscreen mode
		if (m_fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		//remove the window
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		//remove the instance of this application
		UnregisterClass(m_applicationName, m_hinstance);
		m_hinstance = NULL;

		//realese the pointer to this class.
		ApplicationHandle = NULL;
	}

	bool App::frame()
	{
		if (finished)
		{
			m_finished = true;
		}
		return m_finished;
	}

	//WndProc--window proc
	LRESULT CALLBACK WndProc(HWND hwnd, UINT command, WPARAM wparam,LPARAM lparam)
	{
		switch (command)
		{
			//check to see if the window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//check to see if the window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		//every other message gets passed in to the message handler
	    default:
		{
			return ApplicationHandle->MessageHandler(hwnd, command, wparam, lparam);
		}
		}
	}

	LRESULT CALLBACK App::MessageHandler(HWND hwnd, UINT command, WPARAM wparam, LPARAM lparam)
	{
		switch (command)
		{
		case WM_KEYDOWN:
		{
			//check to see if its the escape key if it is quit
			if ((unsigned int)wparam == VK_ESCAPE)
			{
				//finished = true;
			}
			return 0;
		}
	default:
		{
			return DefWindowProc(hwnd, command, wparam, lparam);
		}
		}
	}

	void App::shutdown()
	{
		closeWindows();
	}

	//getHWND-- returns the HWND of this app
	HWND App::getHWND()
	{
		return m_hwnd;
	}

	//getScreenWidth-- reutrns the width of the screen
	int App::getScreenWidth()
	{
		return m_width;
	}

	//getScreenHeight-- returns the height of the screen
	int App::getScreenHeight()
	{
		return m_height;
	}

	//getHinstance-- returns the hinstance of this app
	HINSTANCE App::getHinstance()
	{
		return m_hinstance;
	}

	
}