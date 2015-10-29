#include "Label.h"

namespace JR_Label
{
	//defualt constructor-- creates a new instance of Label with defualt parameters
	Label::Label()
	{
		//set the defualts for the propertys
		m_color = D3DXVECTOR4();
		m_font = NULL;
		m_maxNumChars = 0;
		m_rectangles = 0;
		m_size = 0;
		m_text = 0;
		m_x = 0;
		m_y = 0;
	}

	//deconstructor-- deletes and cleans up any dynamic memory for label
	Label::~Label()
	{
		//clean up label
		shutdown();
	}

	//init-- initializes the label
	//x- the x coord for the label
	//y- the y coord for the label
	//size- the size of the font for the label
	//color- the color of the labels text
	//maxNumChars- the maximum number of characters the label can hold
	//font- the font for the label to use
	bool Label::init(float x, float y, float size, D3DXVECTOR4 color, int maxNumOfChars, JR_Font::Font* font)
	{
		m_x = x;
		m_y = y;
		m_size = size;
		m_color = color;
		m_maxNumChars = maxNumOfChars;
		m_font = font;
		//initialize the array for the letters
		m_rectangles = new JR_Rectangle::Rectangle[maxNumOfChars];

		return true;
	}

	//setText-- sets the text for the label
	//text- the text to set the label to
	bool Label::setText(char* text)
	{
		if (strlen(text) > m_maxNumChars)
		{
			//return false since the max number of characters was exceeded
			return false;
		}
		delete[] m_text;
		//set the text
		m_text = text;

		//update the rectangles array
		m_font->buildRectangleArray(m_rectangles, m_text, m_x, m_y, m_color, m_size);

		//return true since it was successfull
		return true;
	}

	//setX-- sets the x coord of the label
	//x - the new x coord
	void Label::setX(float x)
	{
		m_x = x;
	}

	//setY-- sets the y coord of the label
	//y- the new y coord for the label
	void Label::setY(float y)
	{
		m_y = y;
	}

	//setSize-- sets the size for the labels font
	//size- the size of the labels font
	void Label::setSize(float size)
	{
		m_size = size;
	}

	//setColor-- sets the color of the labels text
	//color- the new color for the label
	void Label::setColor(D3DXVECTOR4 color)
	{
		m_color = color;
	}

	//setFont-- sets the font for the label
	//font- the new font for the label
	void Label::setFont(JR_Font::Font* font)
	{
		m_font = font;
	}

	//setMaxNumChars-- sets the max number of characters for the label
	//maxNumChars- the maximum number of characters this label can hold(note this wipes the text that is in the label
	// currently.
	void Label::setMaxNumChars(int maxNumChars)
	{
		m_maxNumChars = maxNumChars;

		//update the letter size
		delete[] m_rectangles;
		
		m_rectangles = new JR_Rectangle::Rectangle[m_maxNumChars];

		//rebuild the text
		setText(m_text);
	}

	//getText-- retusnt eh text for the label
	char* Label::getText()
	{
		return m_text;
	}

	//getX-- retusn the x coord of the label
	float Label::getX()
	{
		return m_x;
	}


	//getY-- returns the y coord of the label
	float Label::getY()
	{
		return m_y;
	}

	//getSize-- returns the size of the label
	float Label::getSize()
	{
		return m_size;
	}

	//getColor-- returns the color fo the label
	D3DXVECTOR4 Label::getColor()
	{
		return m_color;
	}

	//getFont-- returns the font for the label
	JR_Font::Font* Label::getFont()
	{
		return m_font;
	}

	//getMaxNumChars-- returns the maximum number of characters for the label
	int Label::getMaxNumChars()
	{
		return m_maxNumChars;
	}

	//shutdown-- cleans up memory for the label
	void Label::shutdown()
	{
		//delete the text and the letter array
		if (m_text)
		{
			delete[] m_text;
		}

		if (m_rectangles)
		{
			delete[] m_rectangles;
		}
	}

	//draw-- draws the text
	void Label::draw(Renderer* renderer)
	{
		//draw each of the letters
		int count = strlen(m_text);

		for (int i = 0;i < count;i++)
		{
			//draw each of the characters
			m_rectangles[i].draw(renderer);
		}
	}
}