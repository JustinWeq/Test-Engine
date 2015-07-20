#include "Input.h"

namespace JR_Input
{
	Input::Input()
	{
		m_directInput = NULL;
		m_keyboard = NULL;
		m_mouse = NULL;
		m_mouseX = 0;
		m_mouseY = 0;
	}

	Input::~Input()
	{
		shutdown();
	}

	bool Input::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
	{
		HRESULT result;

		//store the screen width and height
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		//init the mouse location
		m_mouseX = 0;
		m_mouseY = 0;

		//init the main direct input interface
		result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
		if (FAILED(result))
		{
			return false;
		}

		//init the direct input interface for the keyboard
		result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
		if (FAILED(result))
		{
			return false;
		}

		//set the data format, since it is a keyboard use the defualt keyboard format
		result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
		{
			return false;
		}

		//Set the cooperative level of the keyboard to not share with other programs
		result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))
		{
			return false;
		}

		//now aquire the keyboard
		result = m_keyboard->Acquire();
		if (FAILED(result))
		{
			return false;
		}

		//init the direct input interface for the mouse
		result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		if (FAILED(result))
		{
			return false;
		}

		//set the data format for th mouse to use
		result = m_mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result))
		{
			return false;
		}

		//Aquire the mouse
		result = m_mouse->Acquire();
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void Input::shutdown()
	{
		//release the mouse
		if (m_mouse)
		{
			m_mouse->Unacquire();
			m_mouse->Release();
			m_mouse = NULL;
		}

		//release the keyboard
		if (m_keyboard)
		{
			m_keyboard->Unacquire();
			m_keyboard->Release();
			m_keyboard = NULL;
		}

		//Release the main interface to direct input
		if (m_directInput)
		{
			m_directInput->Release();
			m_directInput = NULL;
		}
	}

	bool Input::Frame()
	{
		bool result;

		//Read the current state of the keyboard
		result = ReadKeyboard();
		if (!result)
		{
			return false;
		}

		//read the current state of the mouse.
		result = ReadMouse();
		if (!result)
		{
			return false;
		}

		//Process the changes in the mouse and keyboard.
		ProcessInput();

		return true;
	}


	bool Input::ReadKeyboard()
	{
		HRESULT result;

		result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
		if (FAILED(result))
		{
			//If the keyboard lost focus or was not aquired try to get it back
			if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			{
				m_keyboard->Acquire();
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	bool Input::ReadMouse()
	{
		HRESULT result;

		//Read the mouse device
		result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
		if (FAILED(result))
		{
			//if the mouse lost focus or was not aquired try to get control back
			if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			{
				m_mouse->Acquire();
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	void Input::ProcessInput()
	{
		//update the location of the mouse cursor based on the change of the mouse location
		m_mouseX += m_mouseState.lX;
		m_mouseY += m_mouseState.lY;

		//Ensure the mouse location does not exceed the screen width or height
		if (m_mouseX < 0) m_mouseX = 0;
		if (m_mouseY < 0) m_mouseY = 0;

		if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
		if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;

	}

	bool Input::IsKeyPressed(int key)
	{
		if (m_keyboardState[key] & 0x80)
		{
			return true;
		}
		return false;
	}

	void Input::GetMouseLocation(int& mouseX, int& mouseY)
	{
		mouseX = m_mouseX;
		mouseY = m_mouseY;
	}
}
