#pragma once

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <dinput.h>
namespace JR_Input
{
	class Input
	{
	public:
		Input();
		~Input();

		bool init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeght);
		void shutdown();
		bool Frame();

		bool IsKeyPressed(int key);
		void GetMouseLocation(int& x, int& y);

	private:
		bool ReadKeyboard();
		bool ReadMouse();
		void ProcessInput();
	private:
		IDirectInput8* m_directInput;
		IDirectInputDevice8* m_keyboard;
		IDirectInputDevice8* m_mouse;

		unsigned char m_keyboardState[256];
		DIMOUSESTATE2 m_mouseState;

		int m_screenWidth, m_screenHeight;
		int m_mouseX, m_mouseY;

	};
}