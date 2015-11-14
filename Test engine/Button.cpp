#include "Button.h"
namespace JR_Button
{
	//defualt constructor-- creates a new instance of button with defulat parameters
	Button::Button()
	{
		//set defulat values
		m_backColor = D3DXVECTOR4();
		m_clicked = 0;
		m_foreColor = D3DXVECTOR4();
		m_height = 0;
		m_hover = 0;
		m_label = 0;
		m_rectangle = 0;
		m_width = 0;
		m_x = 0;
		m_y = 0;
	}

	//deconstrucor-- cleans up memory for the button instance
	Button::~Button()
	{
		//call shutdown
		shutdown();
	}

	//init-- initializes the button
	//text- the text for the button
	//x- the x position of the button
	//y- the y position of the button
	//width- the width of the button
	//height- the height of the buton
	//foreColor- the forColor of the button
	//backColor- the back color of the button
	//parent- the information for the GuiManager
	bool Button::init(const char* text, int x, int y, int width, int height, D3DXVECTOR4 foreColor, D3DXVECTOR4 backColor,
		GuiManager* parent)
	{
		//store dimension info
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
		m_testX = x;
		m_testY = y;
		//set x and y accordin to screen dimensions
		m_x -= parent->getApp()->getScreenWidth() / 2- (m_height/2);
		m_y = m_y*-1 + (parent->getApp()->getScreenHeight() / 2)- (m_width/2);
		//set up the label
		m_label = new Label();

		//initialize the label
		m_label->init(m_x, m_y, 1, foreColor, 128,parent->getFont() );

		//store the fore and back color
		m_foreColor = foreColor;
		m_backColor = backColor;

		//set the text for the label
		m_label->setText(text);

		//set up the button
		m_rectangle = new JR_Rectangle::Rectangle();
		//initialize the rectanlge
		m_rectangle->init(m_x, m_y, width, height, 0, -1, D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), m_backColor);

		return true;
	}

	//setX-- sets the x coord of the button
	//x- the new x coord ofr the button
	void Button::setX(int x)
	{
		//set the x coord for the label and the button
		m_x = x;
		m_label->setX(x);
	}

	//getX-- returns the x coord of the button
	int Button::getX()
	{
		return m_x;
	}

	//setY-- set the y coord of the button
	//y- the new y coord for the button
	void Button::setY(int y)
	{
		//set the y coord on the button and the label
		m_y = y;
		m_label->setY(y);
	}

	//getY-- returns the y coord of the button
	int Button::getY()
	{
		return m_y;
	}

	//setWidth-- sets the width of the button
	//width- the new width for the button
	void Button::setWidth(int width)
	{
		m_width = width;
	}

	//getWidth-- returns the width of the button
	int Button::getWidth()
	{
		return m_width;
	}

	//setHeight-- sets the height for the button
	//height- the new height for the button
	void Button::setHeight(int height)
	{
		m_height = height;
	}

	//getHeight-- gets the height
	int Button::getHeight()
	{
		return m_height;
	}

	//setBackColor-- sets the back color for the button
	//backColor- the new back color
	void Button::setBackColor(D3DXVECTOR4 backColor)
	{
		m_backColor = backColor;
	}

	//getBackColor-- returns the back color for the button
	D3DXVECTOR4 Button::getBackColor()
	{
		return m_backColor;
	}

	//setForeColor-- sets the fore color for the button
	//forColor- the new for color
	void Button::setForeColor(D3DXVECTOR4 foreColor)
	{
		m_foreColor = foreColor;
	}

	//getForeColor-- returns the foreColor for the button
	D3DXVECTOR4 Button::getForeColor()
	{
		return m_foreColor;
	}

	//setText-- sets the text that is on the button
	//text- the new text for the button
	void Button::setText(const char* text)
	{
		m_label->setText(text);
	}

	//getText-- returns the text on the button
	const char* Button::getText()
	{
		return m_label->getText();
	}

	//shutdown-- cleans up dynamic memory for the button
	void Button::shutdown()
	{
		//clean up dynamic memory
		if (m_rectangle)
		{
			//clean up rectangle
			delete m_rectangle;
			m_rectangle = NULL;
		}
	}

	//update-- updates the button usng the passed in input device
	//input- the input device to use for updating
	bool Button::update(Input* input)
	{
		//check to see if the button is clicked on
		if (input->isLeftClicked())
		{
			m_clicked = true;
			//set the color of the rectangle
			m_rectangle->setColor(m_foreColor);

			//set the color on the label
			m_label->setColor(m_backColor);
		}
		else
		{
			m_clicked = false;
			//reset the color on the rectangl and the label
			m_rectangle->setColor(m_backColor);
			m_rectangle->setColor(m_foreColor);
		}


		if (mouseHover(input))
		{
			m_hover = true;
			//set the color of the rectangle
			m_rectangle->setColor(m_foreColor);

			//set the color on the label
			m_label->setColor(m_backColor);
		}
		else
		{
			m_hover = false;
			//reset the color on the rectangl and the label
			m_rectangle->setColor(m_backColor);
			m_rectangle->setColor(m_foreColor);
		}
		return true;
	}

	//draw-- prepares the button for rendering
	//renderer- the renderer to draw to
	void Button::draw(Renderer* renderer)
	{
		//draw the rectangle and the label;
		m_rectangle->draw(renderer);
		m_label->draw(renderer);
	}

	//isClickedOn-- returns wheather the button is clicked on or not
	bool Button::isClickedOn()
	{
		return m_clicked;
	}


	//mouseHover-- returns whether over the button or not
	bool Button::mouseHover(Input* input)
	{
		//get x and y location of the mouse on the screen
		int x, y;
		input->GetMouseLocation(x, y);

		//test to see if the mouse is inside the button
		if (x > m_testX && x < m_testX + m_width &&
			y > m_testY && y < m_testY + m_height)
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	
}