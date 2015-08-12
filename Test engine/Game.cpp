#include "App.h"
#include <thread>
#include "Shader.h"
#include "Model.h"
#include <D3DX10math.h>
#include "Graphics.h"
#include <math.h>
#include "Input.h"
#include "Sound.h"
#include "Object.h"
#include "Bitmap.h"
using namespace Application;
using namespace std;
using namespace JR_Model;
using namespace JR_Shader;
using namespace JR_Graphics;
using namespace JR_Input;
using namespace JR_Sound;
using namespace JR_Object;
using namespace JR_Bitmap;

//prototypes
void init();
bool update();
void draw();
void updateViewMatrix();
App app;
Shader* shader;
float cubeRot;
D3DXMATRIX view;
Graphics* graphics;
//Model* model;
Object* object;
Input* input;
Sound* sound;
Bitmap* bitmap;
bool error = false;
struct camera
{
public:
	float x, y, z;
	float rotX, rotY, rotZ;
};
void init()
{
	app = App();

	app.init(800, 600, false , L"Test engine");

	//Create view matrix
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 10), &D3DXVECTOR3(0, 0, -9), &D3DXVECTOR3(0, 1, 0));

	//Create the graphics
	graphics = new Graphics();

	//Create the shader
	shader = new Shader();

	//Create the model

    //model = new Model();

	//Create the managed object
	object = new Object();

	//init the graphics
	error !=  graphics->init(app.getScreenWidth(), app.getScreenHeight(),true, app.getHWND(), false, 1000, 0.2);


	//init the shader
	error != shader->init(graphics->getDevice(), app.getHWND());

	//init the model
	//error != model->init(graphics->getDevice(), "cube.mdl", TEXT("texture.dds"));

	error  != object->init(graphics->getDevice(), "brick.obj", TEXT("texture.dds"));

	//init the input device
	input = new Input();

	 input->init(app.getHinstance(), app.getHWND(), app.getScreenWidth(), app.getScreenHeight());

	 //create and set up the sound
	 sound = new Sound();

	 error != sound->init(app.getHWND());

	 //create and set up the test bitmap
	 bitmap = new Bitmap();

	 //init bitmap
	 bitmap->init(graphics->getDevice(), app.getScreenWidth(), app.getScreenHeight(), TEXT("texture.dds"), 32, 32);
	 
}

bool update()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done && !error)
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
			cubeRot += 0.02;
			 //modf(3.149,&cubeRot);
			//now do the draw updates
			draw();
		}
		input->Frame();
		////check to see if the escape key is pressed
		if (input->IsKeyPressed(DIK_ESCAPE))
		{
			done = true;
		}

		//read input for player movement.
		float x = 0, y = 0, z = 0;
		
		//read left movement
		if (input->IsKeyPressed(DIK_A))
		{
			x += 1;
		}

		//read right movement
		if (input->IsKeyPressed(DIK_D))
		{
			x -= 1;
		}

		//read up movement
		if (input->IsKeyPressed(DIK_W))
		{
			z -= 1;
		}

		//read down movement
		if (input->IsKeyPressed(DIK_S))
		{
			z += 1;
		}

		//D3DXMATRIX matrix = object->getWorld();
		//D3DXMatrixRotationY(&matrix, cubeRot);
		//D3DXMatrixTranslation(&matrix, x, y, z);
		//(*object)*matrix;

	}
	return true;
}

void updateViewMatrix()
{
	//update the position of the camera;
}



void draw()
{
	D3DXMATRIX world,world2,projection,ortho;
	bool result;

	//Clear the buffers to begin the scene.
	graphics->beginDrawing(0, 1, 0, 1);

	graphics->getWorldMatrix(world);
	graphics->getWorldMatrix(world2);
	graphics->getProjectionMatrix(projection);
	graphics->getOrthoMatrix(ortho);
	D3DXMatrixRotationY(&world,cubeRot);
	(*object)*world;
	//model->render(graphics->getDeviceContext());
	object->render(graphics->getDeviceContext());

	//render the model using the defualt shader
	result = shader->render(graphics->getDeviceContext(), object->getIndexCount() , world, view,
		projection, object->getTexture(), D3DXVECTOR3(0, 0, 1), D3DXVECTOR4(1, 1, 1, 0.2),
		D3DXVECTOR4(1, 1, 1, 1), D3DXVECTOR3(0, 0, -10), D3DXVECTOR4(1, 1, 1, 1), 32);
	if (!result)
	{
		error = true;
	}

	//begin 2D drawing now
	//disable 2 buffer
	graphics->zBufferState(false);

	//prepare the bitmap for rendering
	result = bitmap->render(graphics->getDeviceContext(), 0, 0);
	if (!result)
	{
		error = true;
	}

	//render the bitmap with the texture shader
	result = shader->renderTexture(graphics->getDeviceContext(), bitmap->getIndexCount(), world2, view, ortho, bitmap->getTexture());
	if (!result)
	{
		error = true;;
	}

	//turn the z buffer back on now that 2D rendering is over
	graphics->zBufferState(true);
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