#pragma once
#include "Renderer.h"
#include "Font.h"
namespace JR_Label
{
 //label- a simple class that contains methods and propertys for displaying and drawing a label in 2D
 class Label
 {
 public:
	 //defualt constructor-- creates a new instance of Label with defualt parameters
	 Label();

	 //deconstructor-- deletes and cleans up any dynamic memory for label
	 ~Label();

	 //init-- initializes the label
	 //x- the x coord for the label
	 //y- the y coord for the label
	 //size- the size of the font for the label
	 //color- the color of the labels text
	 //maxNumChars- the maximum number of characters the label can hold
	 //font- the font for the label to use
	 bool init(float x, float y, float size,D3DXVECTOR4 color,int maxNumOfChars,JR_Font::Font* font);

	 //setText-- sets the text for the label
	 //text- the text to set the label to
	 bool setText(char* text);

	 //setX-- sets the x coord of the label
	 void setX(float x);

	 //setY-- sets the y coord of the label
	 //y- the new y coord for the label
	 void setY(float y);

	 //setSize-- sets the size for the labels font
	 //size- the size of the labels font
	 void setSize(float size);

	 //setColor-- sets the color of the labels text
	 //color- the new color for the label
	 void setColor(D3DXVECTOR4 color);

	 //setFont-- sets the font for the label
	 //font- the new font for the label
	 void setFont(JR_Font::Font* font);

	 //setMaxNumChars-- sets the max number of characters for the label
	 //maxNumChars- the maximum number of characters this label can hold(note this wipes the text that is in the label
	 // currently.
	 void setMaxNumChars(int maxNumChars);

	 //getText-- retusnt eh text for the label
	 char* getText();

	 //getX-- retusn the x coord of the label
	 float getX();


	 //getY-- returns the y coord of the label
	 float getY();

	 //getSize-- returns the size of the label
	 float getSize();

	 //getColor-- returns the color fo the label
	 D3DXVECTOR4 getColor();

	 //getFont-- returns the font for the label
	 JR_Font::Font* getFont();

	 //getMaxNumChars-- returns the maximum number of characters for the label
	 int getMaxNumChars();

	 //shutdown-- cleans up memory for the label
	 void shutdown();

	 void draw(Renderer* renderer);
 private:
	 //text- the text of the label
	 char* m_text;

	 //rectangles- the rectangle list for the label
	 JR_Rectangle::Rectangle* m_rectangles;

	 //x- the x coord of the label
	 float m_x;

	 //y- the y coord of the label
	 float m_y;

	 //size- the size to multiply the font by
	 float m_size;

	 //color- the color of the text
	 D3DXVECTOR4 m_color;

	 //font- the font of the label
	 JR_Font::Font* m_font;

	 //maxNumChars- the maximum number of characters the label can hold
	 int m_maxNumChars;
 };
}
