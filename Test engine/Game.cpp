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
#include "Text.h"
#include "Cpu.h"
#include "Fps.h"
#include "Timer.h"
#include "FlyCam.h"
#include "Terrain.h"
#include "Frustum.h"
#include "TerrainQuadTree.h"
#include "LineModel.h"
#include <string>
using namespace Application;
using namespace std;
using namespace JR_Model;
using namespace JR_Shader;
using namespace JR_Graphics;
using namespace JR_Input;
using namespace JR_Sound;
using namespace JR_Object;
using namespace JR_Bitmap;
using namespace JR_Text;
using namespace JR_Timer;
using namespace JR_Fps;
using namespace JR_CPU;
using namespace JR_FlyCam;
using namespace JR_Terrain;
using namespace JR_Frustum;
using namespace JR_TerrainQuadTree;

//prototypes
void init();
bool update();
void draw();
void updateViewMatrix();
void shutdown();
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
Text* text;
Fps* fps;
Timer* timer;
Cpu* CPU;
Terrain* terrain;
TerrainQuadTree* quadTree;
Frustum* frustum;
//line model
LineModel* lineModel;

bool error = false;
int cpuPercentage;
float frameTime;
FlyCam cam;
float dmx = 0, dmy = 0,dcx = 0,dcy = 0,dcz = 10;
int fpss;
int numTriangles;
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

	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 10), &D3DXVECTOR3(0, 0, -9), &D3DXVECTOR3(0, 1, 0));

	//Create view matrix
	//cam.setView(view);
	//cam.init(0, 0, 10, 0, 0, 0);

	//Create the graphics
	graphics = new Graphics();

	//Create the shader
	shader = new Shader();

	//Create the model

    //model = new Model();

	//Create the managed object
	object = new Object();

	//init the graphics
	error !=  graphics->init(app.getScreenWidth(), app.getScreenHeight(),false, app.getHWND(), false, 1000, 0.2);


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

	 //create text object
	 text = new Text();
	 D3DXMATRIX baseView;
	 D3DXMatrixTranslation(&baseView,0,0,-1.0f);
	 //init the text object
	 text->init(graphics->getDevice(), graphics->getDeviceContext(), app.getHWND(),
		 app.getScreenWidth(), app.getScreenHeight(), view, 5);

	 //set up sentence 1
	// text->setSentence(0, "Red", 0, 0, 1.0f, 0.0f, 0.0f, graphics->getDeviceContext(), graphics->getDevice());

	 //set up sentence 2
	// text->setSentence(1, "Green", 0, 32, 0.0f, 1.0f, 0.0f, graphics->getDeviceContext(), graphics->getDevice());

	 //set up sentence 3
	 //text->setSentence(2, "Blue", 0, 64, 0.0f, 0.0f, 1.0f, graphics->getDeviceContext(), graphics->getDevice());
	 
	 //create the cpu object
	 CPU = new Cpu();

	 //init the Cpu object
	 CPU->init();

	 //create the fps object
	 fps = new Fps();

	 //init the fps object
	 fps->init();

	 //create the timer object
	 timer = new Timer();

	 //init the timer object
	 timer->init();

	 //create the terrain object
	 terrain = new Terrain;


	 terrain->init(graphics->getDevice(), "heightmap01.bmp",TEXT("texture.dds"));


	 //setUp thefrustum
	 frustum = new Frustum;
	 
	 quadTree = new TerrainQuadTree;

	 quadTree->init(terrain,graphics->getDevice());

	 //set up line model
	 lineModel = new LineModel;

	 //set up and obtain model info to pass into the line model object
	 D3DXVECTOR3* vertices;

	 vertices = new D3DXVECTOR3[object->getModelVertexCount()];

	 D3DXVECTOR3 newVertices;

     object->getVertices(vertices);

	 //initialize line model

	 //vertices = new D3DXVECTOR3[2];

	 //vertices[0] = D3DXVECTOR3(0, 0, 0);

	 //vertices[1] = D3DXVECTOR3(0, 100, 0);

     lineModel->init(graphics->getDevice(), vertices, D3DXVECTOR4(1, 0, 0, 1),object->getIndexCount());
	 

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
			cubeRot += (0.02)* frameTime;
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
		float playSpeed = 1;
		//read left movement
		if (input->IsKeyPressed(DIK_A))
		{
			x += playSpeed*frameTime;
		}

		//read right movement
		if (input->IsKeyPressed(DIK_D))
		{
			x -= playSpeed*frameTime;
		}

		//read up movement
		if (input->IsKeyPressed(DIK_E))
		{
			y -= playSpeed*frameTime;
		}

		//read down movement
		if (input->IsKeyPressed(DIK_Q))
		{
			y += playSpeed*frameTime;
		}

		//read forward movement
		if (input->IsKeyPressed(DIK_W))
		{
			z -= playSpeed*frameTime;
		}

		//read backward movement
		if (input->IsKeyPressed(DIK_S))
		{
			z += playSpeed*frameTime;
		}

		//move the camera



		cam.setDeltaX(x);
		cam.setDeltaY(y);
		cam.setDeltaZ(z);
		dcx = cam.getPosX();
		dcy = cam.getPosY();
		dcz = cam.getPosZ();

		float height;

		if (quadTree->getHeightAtPosition(-dcx, -dcz, height))
		{
			cam.setPosY(-height - 2.0f);
		}

		//move view cam accordingly
		int mx, my;
		byte* buttons;
		input->GetRawLocation(mx,my);

		//move the view up and down
		dmx += mx*frameTime;
		dmy += my*frameTime;
		
		cam.setRotZ(0);

		cam.setRotX(dmy);

		cam.setRotY(dmx);

		cam.updateView();


		//do frame processing for the FPS CPU and timer objects
		fps->frame();
		timer->frame();
		CPU->frame();

		cpuPercentage = CPU->getCpuPercentage();
		if (timer->getTime() > 14)
			frameTime = 14 / timer->getTime();
		else
		    frameTime = timer->getTime()/14;
		fpss = fps->getFps();

		//D3DXMATRIX matrix = object->getWorld();
		//D3DXMatrixRotationY(&matrix, cubeRot);
		//D3DXMatrixTranslation(&matrix, x, y, z);
		//(*object)*matrix;
		//update the sentences
		char tempString[32];
		char cpuString[32];
		char fpsString[32];
		char drawsString[32];
		char frameTimeString[32];
		char mouseButtonString[32];
		bool clicked = input->isLeftClicked();
	    
		_itoa_s(cpuPercentage, tempString, 10);
		strcpy_s(cpuString, "Cpu: ");
		strcat_s(cpuString, tempString);
		strcat_s(cpuString, "%");
		text->setSentence(0, cpuString, 10, 0, 0, 1, 0, graphics->getDeviceContext(), graphics->getDevice());
		//set fps sentence
		_itoa_s(fpss, tempString, 10);
		strcpy_s(fpsString, "Fps: ");
		strcat_s(fpsString, tempString);
		text->setSentence(1, fpsString, 10, 32, 0, 1, 0, graphics->getDeviceContext(), graphics->getDevice());
		//set frametime sentence
		sprintf(tempString, "%f", frameTime);
		strcpy_s(frameTimeString, "FrameTime: ");
		strcat_s(frameTimeString, tempString);
		text->setSentence(2, frameTimeString, 20,64, 0, 1, 0, graphics->getDeviceContext(), graphics->getDevice());
		//set number of drawn triangles
		_itoa_s(numTriangles, tempString, 10);
		strcpy(drawsString, "Draws Triangles");
		strcat_s(drawsString, tempString);
		text->setSentence(3, drawsString, 20, 96, 0, 1, 0, graphics->getDeviceContext(), graphics->getDevice());
		//set mouse button
		//sprintf(tempString, "%b", clicked);
		//strcpy_s(mouseButtonString, "left button: ");
		//strcat_s(mouseButtonString, tempString);
		if (input->isLeftClicked())
		{
			text->setSentence(4,"mouse is clicked", 20, 128, 0, 1, 0, graphics->getDeviceContext(), graphics->getDevice());
		}
		else
		{
			text->setSentence(4, "mouse is not clicked", 20, 128, 0, 1, 0, graphics->getDeviceContext(), graphics->getDevice());
		}



	}

	return true;
}

void updateViewMatrix()
{
	//update the position of the camera;
}



void draw()
{
	D3DXMATRIX world,world2,world3,projection,ortho;
	bool result;

	//Clear the buffers to begin the scene.
	graphics->beginDrawing(0, 0, 0, 1);

	graphics->getWorldMatrix(world);
	world3 = world;
	graphics->getWorldMatrix(world2);
	graphics->getProjectionMatrix(projection);
	graphics->getOrthoMatrix(ortho);
	//D3DXMatrixRotationY(&world,cubeRot);
	//(*object)*world;
	//model->render(graphics->getDeviceContext());
	//object->render(graphics->getDeviceContext());
	//prepare the line model for rendering
	lineModel->render(graphics->getDeviceContext());

	//render the line model instead of the regular one using the color shader
	result = shader->renderLine(graphics->getDeviceContext(), lineModel->getIndexCount(), object->getWorld(), cam.getViewMatrix(), projection);
	if (!result)
	{
		error = true;
	}

	//render the model using the defualt shader
	result = shader->render(graphics->getDeviceContext(), object->getIndexCount() ,object->getWorld(), cam.getViewMatrix(),
		projection, object->getTexture(), D3DXVECTOR3(0, 0, 1), D3DXVECTOR4(0.5, 0.5, 0.5, 1),
		D3DXVECTOR4(1, 1, 1, 1), D3DXVECTOR3(dcx, dcy, dcz), D3DXVECTOR4(1, 1, 1, 1), 32);
	if (!result)
	{
		error = true;
	}

	//now draw the terrain

	//construct the frustum
	frustum->constructFrustum(1000, projection, cam.getViewMatrix());

	//render the terrain using the quad tree
	quadTree->render(frustum, graphics->getDeviceContext(), shader);

	//set the number of triangles since some where culled
	numTriangles = quadTree->getDrawCount();

	shader->setTerrainShaderParameters(graphics->getDeviceContext(),object->getWorld(),cam.getViewMatrix(),projection,
		D3DXVECTOR3(0, 0, 0.75), D3DXVECTOR4(0.5, 0.5, 0.5, 1), D3DXVECTOR4(1, 1, 1, 1), terrain->getTexture());



	//result = shader->
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
		error = true;
	}

	//now begin rednering text

	//turn on alpha blending
	graphics->alphaBlendingState(true);
	result = text->render(graphics->getDeviceContext(), world2, ortho, shader);

	//disable alpha blending
	graphics->alphaBlendingState(false);

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

		//now shutdown everything
		shutdown();

	app.shutdown();
	return 0;
}

void shutdown()
{
	if (shader)
	{
		shader->shutdown();
		shader = NULL;
	}
	if (graphics)
	{
		graphics->close();
		graphics = NULL;
	}
	if (object)
	{
		object->shutdown();
		object = NULL;
	}
	if (input)
	{
		input->shutdown();
		input = NULL;
	}
	if (sound)
	{
		sound->shutdown();
		sound = NULL;
	}
	if (bitmap)
	{
		bitmap->shutdown();
		bitmap = NULL;
	}
	if (text)
	{
		text->shutdown();
		text = NULL;
	}
}