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

	//getX-- returns the x coord of the rectangle
	float Rectangle::getX()
	{
		return m_x;
	}

	//getY-- returns the y coord dof the rectangle
	float Rectangle::getY()
	{
		return m_y;
	}

	//getWidth-- returns the width of the rectangle
	float Rectangle::getWidth()
	{
		return m_width;
	}

	//getHeight-- returns the height of the rectangle
	float Rectangle::getHeight()
	{
		return m_height;
	}

	//getRotation-- returns the rotation of the rectangle
	float Rectangle::getRotation()
	{
		return m_rotation;
	}

	//getTextureID-- returns the textureID of the rectangle
	int Rectangle::getTextureID()
	{
		return m_textureID;
	}

	//getUVAdd-- returns the amount to add to the UV coords of the rectangle
	D3DXVECTOR2 Rectangle::getUVAdd()
	{
		return m_UVAdd;
	}

	//getUVMultiply-- returns the amount to multiply the UV coords by
	D3DXVECTOR2 Rectangle::getUVMultiply()
	{
		return m_UVMultiply;
	}

	//getColor-- returns the color of the rectangle
	D3DXVECTOR4 Rectangle::getColor()
	{
		return m_color;
	}

	//setX-- sets the x coord of the rectangle
	//x- the new x coord of the rectangle
	void Rectangle::setX(float x)
	{
		m_x = x;
	}

	//setY-- sets the y coord of the rectangle
	//y- the new y coord of the rectangle
	void Rectangle::setY(float y)
	{
		m_y = y;
	}

	//setWidth-- sets the width of the rectangle
	//width- the new width of the rectangle
	void Rectangle::setWidth(float width)
	{
		m_width = width;
	}

	//setHeight-- sets the height of the rectangle
	//height- the new height of the rectangle
	void Rectangle::setHeight(float height)
	{
		m_height = height;
	}

	//setRotation-- sets the rotation of the rectangle
	//rotation-the new rotation of the rectangle(in degrees)
	void Rectangle::setRotation(float rotation)
	{
		m_rotation = rotation;
	}

	//setTextureID-- sets the id of the texture
	//textureID- the new id for the texture(set this to anything lower then 0 or higher then 30 to not have a texture
	void Rectangle::setTextureID(int textureID)
	{
		m_textureID = textureID;
	}

	//setUVAdd-- sets the UVAdd for the rectangle
	//UVAdd- the new amount to add to the UV coords
	void Rectangle::setUVAdd(D3DXVECTOR2 UVAdd)
	{
		m_UVAdd = UVAdd;
	}

	//setUVMultiply-- sets the amount to multiply the uv coords by
	//UVMultiply- the new amount to multiply by
	void Rectangle::setUVMultiply(D3DXVECTOR2 UVMultiply)
	{
		m_UVMultiply = UVMultiply;
	}

	//setColor-- sets the color of the rectangle
	//color- the new color of the rectangle, set this to solid white(1,1,1,1) to not change the color at all
	void Rectangle::setColor(D3DXVECTOR4 color)
	{
		m_color = color;
	}


	//update-- updates the rectangle, this should be called after any new propertys are set only.
	void Rectangle::update()
	{
		//call create matrix to recreate the world matrix with new parameters
		createMatrix();
	}
}