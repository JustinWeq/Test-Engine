#include "App.h"
#include <thread>
#include "Shader.h"
#include "Model.h"
#include <D3DX10math.h>
#include "Graphics.h"
using namespace Application;
using namespace std;
using namespace JR_Model;
using namespace JR_Shader;
using namespace JR_Graphics;

//prototypes
void init();
bool update();
void draw();
App app;
Shader shader;
float cubeRot;
D3DXMATRIX view;
Graphics graphics;
Model model;
bool error = false;

void init()
{
	app = App();

	app.init(480, 640, true , L"Test engine");

	//Create view matrix
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 10, 0), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));

	//Create the graphics
	graphics = Graphics();

	//Create the shader
	shader = Shader();

	//Create the model

	model = Model();

	//init the graphics

}

bool update()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = app.frame();
			if (result)
			{
				done = true;
			}
			if (finished)
			{
				done = true;
			}
		}

	}
	return true;
}



void draw()
{

}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	try
	{
		init();
		if (!error)
		{
			update();
		}
		else
		{
			//there was a problem setting up so tell the user that
			MessageBox(app.getHWND(), L"There was an error setting up D3D, closing application", L"Error", MB_OK);
		}
	}
	catch (exception e)
	{
		const char* error = e.what();

		MessageBox(NULL, L"There was an unknown exception", TEXT("Error"), MB_OK);
	}

	app.shutdown();
	return 0;
}