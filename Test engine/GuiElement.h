#pragma once
#include "Shader.h"
using namespace JR_Shader;
namespace JR_Gui
{
	//UpdateInfo-- contains data for needed update information
	struct UpdateInfo
	{
		//mouseX- the x location of the mouse
		float mouseX;
		//mouseY- the y location of the mouse
		float mouseY;
		//mouseZ- the z location of the mouse otherwise known as the scroll wheel
		float mouseZ;
		//mouseButtons- the various mouse button states
		bool mouseButtons[8];
		//frameTime- the slice for the last frame
		float frameTime;
		//keys- the key presses for the keyboard during last frame
		char keys[128];
	};

	//Dimensions-- a struct that contains data for a GuiElements
	// Dimensions
	struct Dimensions
	{
		//x- the x coord
		float x;
		//y- the y coord
		float y;
		//width- the width
		float width;
		//height- the height
		float height;

		//overloaded constructor-- creates a new instance of  Dimensions with passed in parameters
		//x- the x coord
		//y- the y coord
		//width - the width
		//height- the height
		Dimensions(float x, float y, float width, float height)
		{
			(*this).x = x;
			(*this).y = y;
			(*this).width = width;
			(*this).height = height;
		}

	};

	//GuiElement-- an abstract class that contains methods and propertys
	// for a GuiElement
    class GuiElement
	{
	public:


	public:
		//init-- intiailizes the gui element, must be overridden
		//x- the x coord of this gui component
		//y- the y coor dof this gui component
		//width- the width of this gui component
		//height- the height of this gui component
		virtual void init(float x, float y, float width, float height);

		//update-- updates the GuiElement derived class
		//updateInfo- the update information for the last frame
		virtual void  update(const UpdateInfo* updateInfo);

		//shutdown-- cleans up dynamic memory for the Gui elemn
		virtual void shutdown();

		//draw-- draws the gui element, must be overridden
		//deviceContext- the device context to use for drawing
		//shader- the shader to use for drawing
		//view- the view matrix to use for drawing
		//ortho- the ortho matrix to use for drawing
		virtual void draw(ID3D11DeviceContext* deviceContext,Shader shader,D3DXMATRIX view,D3DXMATRIX ortho);

		//getDimensions-- returns the dimensions for the GuiElement;
		Dimensions getDimensions();

		//setDimensions-- sets the dimensions for the gui element
		//dimensions- the new dimensions ot he gui element
		void setDimensions(Dimensions dimensions);

		//setVisability- sets the visability state
		//isVisable- the state to set the visability to
		void setVisability(bool isVisable);
		
		//isVisable-- returns the visability of the gui element
		bool isVisable();

	private:
		//dimensions- the dimensions of this gui element, defualt = {0,0,0,0}
		Dimensions* m_dimensions;
		//isVisable- indicates whether the element is visable or not, defualt = false
		bool m_isVisable;
	};
}