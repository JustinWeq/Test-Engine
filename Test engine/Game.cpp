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
#include "Rectangle.h"
#include "Renderer.h"
#include "Math.h"
#include "Label.h"
#include <string>
using namespace Application;
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
using namespace JR_Renderer;
using namespace JR_Label;
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
Texture* rectTexture;
JR_Rectangle::Rectangle* rectangle;
Renderer* renderer;
//line model
LineModel* lineModel;
Texture* texture;
Label* label;

JR_Rectangle::Rectangle* rectangle2;

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

	bool result;
	app = App();

	app.init(1920, 1080, false , L"Test engine");

	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, -10.0f), &D3DXVECTOR3(0, 0, -9.0f), &D3DXVECTOR3(0, 1, 0));

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
	error !=  graphics->init(app.getScreenWidth(), app.getScreenHeight(),false, app.getHWND(), false, 1000, 0.1);


	//init the shader
	error != shader->init(graphics->getDevice(), app.getHWND());

	//init the model
	//error != model->init(graphics->getDevice(), "cube.mdl", TEXT("texture.dds"));

	error  != object->init(graphics->getDevice(), "test.fbx", TEXT("texture.dds"));

	//init the input device
	input = new Input();

	 input->init(app.getHinstance(), app.getHWND(), app.getScreenWidth(), app.getScreenHeight());

	 //create and set up the sound
	 sound = new Sound();

	 error != sound->init(app.getHWND());

	 //create and set up the test bitmap
	 bitmap = new Bitmap();

	 //init bitmap
	 error !=  bitmap->init(graphics->getDevice(), app.getScreenWidth(), app.getScreenHeight(), TEXT("texture.dds"), 256, 256);

	 //create text object
	 text = new Text();
	 D3DXMATRIX baseView;
	 D3DXMatrixTranslation(&baseView,0,0,-1.0f);
	 //init the text object
	 error != text->init(graphics->getDevice(), graphics->getDeviceContext(), app.getHWND(),
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
	 error != timer->init();

	 //create the terrain object
	 terrain = new Terrain;


	 error != terrain->init(graphics->getDevice(), "heightmap01.bmp",TEXT("texture.dds"));


	 //setUp thefrustum
	 frustum = new Frustum;
	 
	 quadTree = new TerrainQuadTree;

	 error != quadTree->init(terrain,graphics->getDevice());

	 //set up line model
	 lineModel = new LineModel;

	 //set up and obtain model info to pass into the line model object
	 D3DXVECTOR3* vertices;

	 vertices = new D3DXVECTOR3[object->getModelVertexCount()];

	 D3DXVECTOR3* newVertices;

	 newVertices = new D3DXVECTOR3[4];
      object->getVertices(vertices);

	 D3DXVECTOR3 vertex = vertices[object->getModelVertexCount()];
	 int vertexCount;
		 //get the starting simplex for the convex hull

	quickHull(vertices, newVertices, object->getModelVertexCount(), vertexCount);

	 //initialize line model

	 //vertices = new D3DXVECTOR3[2];

	 //vertices[0] = D3DXVECTOR3(0, 0, 0);

	 //vertices[1] = D3DXVECTOR3(0, 100, 0);

    error !=  lineModel->init(graphics->getDevice(), newVertices, D3DXVECTOR4(1, 0, 0, 1),4);
	 
	 //set up the rectangle
	 rectangle = new JR_Rectangle::Rectangle();

	 //init the rectangle
	 error != rectangle->init(0, 0, 256,32, 132, 0, D3DXVECTOR2(0,0), D3DXVECTOR2(1, 1), D3DXVECTOR4(0, 0, 1, 1));

	 //set up the renderer
	 renderer = new Renderer();

	 //init the renderer
	 error = !renderer->init(graphics->getDevice(), app.getScreenWidth(), app.getScreenHeight(), app.getHWND());

	 //load the texture
	 rectTexture = new Texture();

	 error != rectTexture->init(graphics->getDevice(), TEXT("texture.dds"));
	 if (error)
	 {
		 int test = 0;
	 }

	  texture = new Texture();

	 error != texture->init(graphics->getDevice(), TEXT("Aerial.png"));

	 renderer->addTexture(*rectTexture);
	 renderer->addTexture(*texture);
	 int test = 0;

	 rectangle2 = new JR_Rectangle::Rectangle();

	 rectangle2->init(256, 0, 256, 256, 236, 0, D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), D3DXVECTOR4(1, 1, 1, 1));

	 //set up the label

	 label = new Label();

	 label->init(0,-100, 0.5, D3DXVECTOR4(0, 1, 0, 1), 256, text->getFont());

	 label->setTextureChannel(1);

	 //set the label sentence
	 label->setText("i\nnew line\nnewline\nnewline");
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


		string test = "Fps: " + to_string(fpss) + "\n" +
			"Cpu: " + to_string(cpuPercentage) + "\nFrametime: " +
			to_string(frameTime);

		//set the sentence
		label->setText(test.c_str());


	}

	return true;
}

void updateViewMatrix()
{
	//update the position of the camera;
}



void draw()
{
	D3DXMATRIX world, world2, world3, projection, ortho;
	bool result;

	//Clear the buffers to begin the scene.
	graphics->beginDrawing(1, 1, 0, 1);

	graphics->getWorldMatrix(world);
	world3 = world;
	graphics->getWorldMatrix(world2);
	graphics->getProjectionMatrix(projection);
	graphics->getOrthoMatrix(ortho);
	//D3DXMatrixRotationY(&world,cubeRot);
	//(*object)*world;
	//model->render(graphics->getDeviceContext());
	object->render(graphics->getDeviceContext());

	shader->render(graphics->getDeviceContext(), object->getIndexCount(), object->getWorld(), cam.getViewMatrix(), projection, rectTexture->getTexture(), D3DXVECTOR3(0, 0, 0), D3DXVECTOR4(0.2, 0.2, 0.2, 1),
		D3DXVECTOR4(1, 1, 1, 1), D3DXVECTOR3(cam.getPosX(), cam.getPosY(), cam.getPosZ()), D3DXVECTOR4(1, 0, 0, 1), 1.0);
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

shader->setTerrainShaderParameters(graphics->getDeviceContext(),object->getWorld(),cam.getViewMatrix(),projection,
		D3DXVECTOR3(0, 0, 0.75), D3DXVECTOR4(0.5, 0.5, 0.5, 1), D3DXVECTOR4(1, 1, 1, 1), terrain->getTexture());



	//render the terrain using the quad tree
	quadTree->render(frustum, graphics->getDeviceContext(), shader);

	//set the number of triangles since some where culled
	numTriangles = quadTree->getDrawCount();

	



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
	
	//prepare the Rectangle for redering
	result = rectangle->draw(renderer);
	if (!result)
	{
		error = true;
	}

	

	result = rectangle2->draw(renderer);

	//prepare the text for rendering
	label->draw(renderer);



	//now begin rednering text

	//turn on alpha blending
	graphics->alphaBlendingState(true);
	result = text->render(graphics->getDeviceContext(), world2, ortho, shader);

	//draw everything in the renderer
	result = renderer->presentDraw(graphics->getDeviceContext());
	if (!result)
	{
		error = true;
	}

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