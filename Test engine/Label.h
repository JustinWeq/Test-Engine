#pragma once
#include "GuiElement.h"
#include "Text.h"
namespace JR_Label
{
	using namespace JR_Text;
	using namespace JR_Gui;
	//Label- a class that contains methods and propertys for a label
	class Label : GuiElement
	{
	public:
		//defualt constructor-- creates a new instance of Label with defualt parameters
		Label();

		virtual void init(float x,float y,float width,float height) override;

		

	protected:

	};
}