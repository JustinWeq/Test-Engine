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
Shader* shader;
float cubeRot;
D3DXMATRIX view;
Graphics* graphics;
Model* model;
bool error = false;

void init()
{
	app = App();

	app.init(640, 480, true , L"Test engine");

	//Create view matrix
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 10, 0), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));

	//Create the graphics
	graphics = new Graphics();

	//Create the shader
	shader = new Shader();

	//Create the model

    model = new Model();

	//init the graphics
	error =  graphics->init(app.getScreenWidth(), app.getScreenHeight(), true, app.getHWND(), true, 1000, 0.2);


	//init the shader
	error = shader->init(graphics->getDevice(), app.getHWND());

	//init the model
	error = model->init(graphics->getDevice(), "cube.mdl", TEXT("texture.dds"));
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

			//now do the draw updates
			draw();
		}

	}
	return true;
}



void draw()
{
	D3DXMATRIX world,projection;
	bool result;

	//Clear the buffers to begin the scene.
	graphics->beginDrawing(0, 0, 0, 1);

	graphics->getWorldMatrix(world);
	graphics->getProjectionMatrix(projection);

	model->render(graphics->getDeviceContext());

	//render the model using the defualt shader
	shader->render(graphics->getDeviceContext(), model->getIndexCount(), world, view,
		projection, model->getTexture(), D3DXVECTOR3(0, 0, 5), D3DXVECTOR4(1, 1, 1, 1),
		D3DXVECTOR4(1, 1, 1, 1), D3DXVECTOR3(0, 10, 0), D3DXVECTOR4(1, 1, 1, 1), 2);

	//present the rendered scene
	graphics->endDrawing();
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
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
	

	app.shutdown();
	return 0;
}