#pragma once
#include "Label.h"
#include "Input.h"
#include "Gui.h"
using namespace JR_Label;
using namespace JR_Input;
using namespace JR_GuiManager;
namespace JR_Button
{
	//Button-- a simple class that contains methods and propertys for a button
	class Button
	{

	public:
		//defualt constructor-- creates a new instance of button with defulat parameters
		Button();

		//deconstrucor-- cleans up memory for the button instance
		~Button();

		//init-- initializes the button
		//text- the text for the button
		//x- the x position of the button
		//y- the y position of the button
		//width- the width of the button
		//height- the height of the buton
		//foreColor- the forColor of the button
		//backColor- the back color of the button
		//parent- the information for the GuiManager
		bool init(const char* text, int x, int y, int width, int height, D3DXVECTOR4 foreColor, D3DXVECTOR4 backColor,GuiManager* parent);

		//setX-- sets the x coord of the button
		//x- the new x coord ofr the button
		void setX(int x);

		//getX-- returns the x coord of the button
		int getX();

		//setY-- set the y coord of the button
		//y- the new y coord for the button
		void setY(int y);

		//getY-- returns the y coord of the button
		int getY();

		//setWidth-- sets the width of the button
		//width- the new width for the button
		void setWidth(int width);

		//getWidth-- returns the width of the button
		int getWidth();

		//setHeight-- sets the height for the button
		//height- the new height for the button
		void setHeight(int height);

		//getHeight-- gets the height
		int getHeight();

		//setBackColor-- sets the back color for the button
		//backColor- the new back color
		void setBackColor(D3DXVECTOR4 backColor);

		//getBackColor-- returns the back color for the button
		D3DXVECTOR4 getBackColor();

		//setForeColor-- sets the fore color for the button
		//forColor- the new for color
		void setForeColor(D3DXVECTOR4 foreColor);

		//getForeColor-- returns the foreColor for the button
		D3DXVECTOR4 getForeColor();

		//setText-- sets the text that is on the button
		//text- the new text for the button
		void setText(const char* text);

		//getText-- returns the text on the button
		const char* getText();

		//shutdown-- cleans up dynamic memory for the button
		void shutdown();

		//update-- updates the button usng the passed in input device
		//input- the input device to use for updating
		bool update(Input* input);

		//draw-- prepares the button for rendering
		//renderer- the renderer to draw to
		void draw(Renderer* renderer);

		//isClickedOn-- returns wheather the button is clicked on or not
		bool isClickedOn();

	private:

		//mouseHover-- returns whether over the button or not
		bool mouseHover(Input* input);
	private:
		//width- the width of the button
		int m_width;
		//height- the height of the button
		int m_height;
		//x- the x coord of the button
		int m_x;
		//y- the y coord of the button
		int m_y;
		//clicked- indicates whether the button is clicked on or not
		bool m_clicked;
		//hover- indicates whether the mouse is over the button or not
		bool m_hover;
		//foreColor- the for color of the Button
		D3DXVECTOR4 m_foreColor;
		//backColor- the back color of the Button
		D3DXVECTOR4 m_backColor;
		//label- the label that displays the text for the button
		Label* m_label;
		//rectangle- the area that is drawn on the button other then the label
		JR_Rectangle::Rectangle* m_rectangle;
	};
}