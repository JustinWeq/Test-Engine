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