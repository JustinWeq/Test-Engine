#include "Rectangle.h"

namespace JR_Rectangle
{

	//defualt contructor-- creates a new instance of Rectangle with default parameters
	Rectangle::Rectangle()
	{
		//zero memory to get it ready for initialization
		m_height = 0;
		m_rotation = 0;
		m_UVAdd = { 0,0 };
		m_UVMultiply = { 0,0 };
		m_width = 0;
		m_x = 0;
		m_y = 0;
	}

	//destructor-- destructs the data for Rectangle
	Rectangle::~Rectangle()
	{
		//do nothing since Rectangle has no dynamic memory
	}


	//init-- initializes the rectangle
	//x- the x coord of the rectangle
	//y- the y coord of the rectangle
	//width- the width of the rectangle
	//height- the height of the rectangle
	//rotation- the 360 rotation of the rectangle
	//textureID- the ID of the texture to use for drawing, put -1 for no texture
	//UVAdd- the 2D vactor to add to the 0 corner
	//UVMultiply- the value to multiply the 1 corner by
	//UVColor- the color of the rectangle, leave alpha 0 to not add color
	bool Rectangle::init(float x, float y, float width, float height, float rotation,
		int textureID, D3DXVECTOR2 UVAdd, D3DXVECTOR2 UVMultiply,D3DXVECTOR4 color)
	{
		//store the x and y values
		m_x = x;
		m_y = y;

		//store the width and height
		m_width = width;
		m_height = height;

		//store the rotation of the rectangle
		m_rotation = rotation;

		//store the texture ID of the rectangle
		m_textureID = textureID;

		//store the UV add of the rectangle
		m_UVAdd = UVAdd;

		//store the UV multiply of the rectangle
		m_UVMultiply = UVMultiply;

		//store the color
		m_color = color;

		//build the matrix

		createMatrix();

		return true;
	}

	bool Rectangle::draw(Renderer* renderer)
	{
		//set the parameters on the renderer to prepare this recangle for drawing
		return renderer->draw(m_matrix,m_textureID,m_color,m_UVAdd,m_UVMultiply);
	}

	//createMatrix-- creates the matrix for the rectangle
	void Rectangle::createMatrix()
	{
		//create the world matrix
		D3DXMATRIX scale;
		D3DXMATRIX rotation;
		D3DXMATRIX translation;

		//create the scale
		D3DXMatrixScaling(&scale, m_width, m_height, 0);

		//convert the rotation to radians
		float radRot = m_rotation* 0.0174532925f;
		//Create the rotation
		D3DXMatrixRotationYawPitchRoll(&rotation, 0,0,radRot);

		//create the translation
		D3DXMatrixTranslation(&translation, m_x, m_y, 0);

		

		m_matrix = scale*rotation*translation;
	}
}