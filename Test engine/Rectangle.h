#pragma once
#include "Renderer.h"
namespace JR_Rectangle
{
	using namespace JR_Renderer;
	//Rectangle-- a simple class that contains methods and prpertys for drawing a simple 2d rectangle
	class Rectangle 
	{
	public:
		//defualt contructor-- creates a new instance of Rectangle with default parameters
		Rectangle();

		//destructor-- destructs the data for Rectangle
		~Rectangle();


		//init-- initializes the rectangle
		//x- the x coord of the rectangle
		//y- the y coord of the ractangle
		//width- the width of the rectangle
		//height- the height of the rectangle
		//rotation- the 360 rotation of the rectangle
		//textureID- the ID of the texture to use for drawing, put -1 for no texture
		//UVAdd- the 2D vactor to add to the 0 corner
		//UVMultiply- the value to multiply the 1 corner by
		//color- the color to set the rectangle, leave alpha 0 to not have color
		bool init(float x, float y, float width, float height , float rotation,
			int textureID , D3DXVECTOR2 UVAdd, D3DXVECTOR2 UVMultiply,D3DXVECTOR4 color);

		//draw- adds the rectangle to the renderer to be drawn
		//renderer- the renderer to draw the rectangle with
		bool draw(Renderer* renderer);

		//getX-- returns the x coord of the rectangle
		float getX();

		//getY-- returns the y coord dof the rectangle
		float getY();

		//getWidth-- returns the width of the rectangle
		float getWidth();

		//getHeight-- returns the height of the rectangle
		float getHeight();

		//getRotation-- returns the rotation of the rectangle
		float getRotation();

		//getTextureID-- returns the textureID of the rectangle
		int getTextureID();

		//getUVAdd-- returns the amount to add to the UV coords of the rectangle
		D3DXVECTOR2 getUVAdd();

		//getUVMultiply-- returns the amount to multiply the UV coords by
		D3DXVECTOR2 getUVMultiply();

		//getColor-- returns the color of the rectangle
		D3DXVECTOR4 getColor();

		//setX-- sets the x coord of the rectangle
		//x- the new x coord of the rectangle
		void setX(float x);

		//setY-- sets the y coord of the rectangle
		//y- the new y coord of the rectangle
		void setY(float y);

		//setWidth-- sets the width of the rectangle
		//width- the new width of the rectangle
		void setWidth(float width);

		//setHeight-- sets the height of the rectangle
		//height- the new height of the rectangle
		void setHeight(float height);

		//setRotation-- sets the rotation of the rectangle
		//rotation-the new rotation of the rectangle(in degrees)
		void setRotation(float rotation);

		//setTextureID-- sets the id of the texture
		//textureID- the new id for the texture(set this to anything lower then 0 or higher then 30 to not have a texture
		void setTextureID(int textureID);

		//setUVAdd-- sets the UVAdd for the rectangle
		//UVAdd- the new amount to add to the UV coords
		void setUVAdd(D3DXVECTOR2 UVAdd);

		//setUVMultiply-- sets the amount to multiply the uv coords by
		//UVMultiply- the new amount to multiply by
		void setUVMultiply(D3DXVECTOR2 UVMultiply);

		//setColor-- sets the color of the rectangle
		//color- the new color of the rectangle, set this to solid white(1,1,1,1) to not change the color at all
		void setColor(D3DXVECTOR4 color);
		

		//update-- updates the rectangle, this should be called after any new propertys are set only.
		void update();

	private:

		//createMatrix-- creates the matrix for the rectangle
		void createMatrix();

	private:
		//x- the x coord of the rectangle
		float m_x;
		//y- the y coord of the rectangle
		float m_y;
		//width- the width of the rectangle
		float m_width;
		//height the height of the rectangle
		float m_height;
		//rotation- the rotation of the rectangle
		float m_rotation;
		//textureID- the texture ID of the rectangle, set to -1 to have no texture
		int m_textureID;
		//UVAdd- the UV coords to add to the 0 corner
		D3DXVECTOR2 m_UVAdd;
		//UVMultiply- the UV coords to multiply the 1 corner by;
		D3DXVECTOR2 m_UVMultiply;
		//matrix- the world matrix of the rectangle
		D3DXMATRIX m_matrix;
		//color- the color of the rectangle, leave alpha at 0 to not add color
		D3DXVECTOR4 m_color;

	};
}