#include "GuiElement.h"
namespace JR_Gui
{
	//getDimensions-- returns the dimensions for the GuiElement;
	Dimensions GuiElement::getDimensions()
	{

		return *m_dimensions;
	}

	//setDimensions-- sets the dimensions for the gui element
	//dimensions- the new dimensions ot he gui element
	void GuiElement::setDimensions(Dimensions dimensions) 
	{
		*m_dimensions = dimensions;
	}

	//init-- intiailizes the gui element, must be overridden
	//x- the x coord of this gui component
	//y- the y coor dof this gui component
	//width- the width of this gui component
	//height- the height of this gui component
	void GuiElement::init(float x, float y, float width, float height)
	{
		setDimensions(Dimensions(x, y, width, height));
	}
}