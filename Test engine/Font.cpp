#include "Font.h"

namespace JR_Font
{
	//defualt constructor-- creates anew instance of Font with defualt parameters
	Font::Font()
	{
		m_font = NULL;
		m_texture = NULL;
	}

	//deconstructor-- deletes dynamic memory and cleans up this instance of Font
	Font::~Font()
	{
		shutdown();
	}

	//init- initializes the font class
	//device- the device to use for initialization
	//fontFileName- the name of the font filw
	//textureFileName- the name of the texture to use for the font
	bool Font::init(ID3D11Device* device, char* fontFileName, WCHAR* textureFileName)
	{
		bool result;
		m_charSet = new CharSet();
		//load in the text file contianing the font data
		result = loadFontData(fontFileName);
		if (!result)
		{
			return false;
		}

		//loadbetterfont data
		//result = loadBetterFontData("testfont.fnt");
		if (!result)
		{
			return false;
		}

		//load the texture that has the font characters it it.
		result = loadTexture(device, textureFileName);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//cleans up memory for the font class
	void Font::shutdown()
	{
		releaseFontData();
		releaseTexture();
	}

	//getTexture-- returns the texture for this font
	ID3D11ShaderResourceView* Font::getTexture()
	{
		return m_texture->getTexture();
	}

	float Font::getStringWidth(char* text)
	{
		int numLetters = (int)strlen(text);
		float length = 0;
		for (int i = 0; i < numLetters; i++)
		{
			int letter = ((int)text[i]) - 32;
			length += m_font[letter].size;
		}
		return length;
	}

	//buildVertexArray-- builds the vertex buffer to use for drawing the sentence
	//vertices- the vertex array to build onto
	//sentence- the sentence that will be displayed
	//drawX- the x draw coord
	//drawY- the y draw coord
	void Font::buildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
	{
		VertexType* vertexPtr;
		int numLetters, index, letter;

		//Coerce the input vertices into a VertexType structure.
		vertexPtr = (VertexType*)vertices;

		//Get the numbers of letters in the sentence
		numLetters = (int)strlen(sentence);

		//init the index to the vertex array
		index = 0;

		//Draw each letter onto a quad.
		for (int i =0; i < numLetters; i++)
		{
			letter = ((int)sentence[i]) - 32;

			//If the letter is a space then just move over three pixels;
			if (letter == 0)
			{
				drawX += 3.0f;
			}
			else
			{
				// First triangle in quad.
				vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
				vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
				vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
				index++;

				// Second triangle in quad.
				vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3(drawX + m_font[letter].size, drawY, 0.0f);  // Top right.
				vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
				vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
				index++;

				// Update the x location for drawing by the size of the letter and one pixel.
				drawX = drawX + m_font[letter].size + 1.0f;
			}
		}
	}

	//buildRectangleArray-- builds the array of rectangles to draw
	//rectangles- the array of rectangles to copy to
	//sentence- the sentence the array of rectangles is based on
	//drawX- the x coord of the sentence
	//drawY- the y coord of the sentence
	//color- the color of the sentence
	//size- the size to multiply the font by
	//textureChannel- the id for the texture
	void Font::buildRectangleArray(void* rectangles,const char* sentence, float drawX, float drawY, D3DXVECTOR4 color,float size,int textureChannel)
	{
		JR_Rectangle::Rectangle* rectanglesPtr;
		int numLetters, letter;
		float oriX = drawX;
		//cast void rectangles to rectanglesPtr
		rectanglesPtr = (JR_Rectangle::Rectangle*)rectangles;

		//get the number of letters in the sentence
		numLetters = strlen(sentence);


		//create each letter
		for (int i = 0;i < numLetters;i++)
		{
			letter = ((int)sentence[i]) - 32;

			if (letter+32 == 0)
			{
				//it was a space so just add to the x draw location
				drawX += 3.0f* size;
			}
			else
				if (letter + 32 == '\n')
				{
					//character was a new line so make a new line for the characters
					drawX = oriX;
					//make the line move down one
					drawY -= 16 * size;
				}
				else
				{
					//create the new rectangle to be added
					JR_Rectangle::Rectangle rect = JR_Rectangle::Rectangle();

					//set the propertys for the rectangle and add it to the rectangles array

					rect.init(drawX, drawY, m_font[letter].size*size, 16 * size, 0, textureChannel, D3DXVECTOR2(m_font[letter].left, 0), D3DXVECTOR2(m_font[letter].right, 1), color);

					//put the rectangle into the rectangles list

					rectanglesPtr[i] = rect;
					//update the x draw location
					drawX = drawX + ((m_font[letter].size + 3)*size);
				}



		}

	}

	//loadFontData-- loads the data for the font
	//fontFileName- the name of the font file
	bool Font::loadFontData(char* fontFileName)
	{
		ifstream fin;
		int i;
		char temp;

		//Create the font spacing buffer
		m_font = new FontType[95];
		if (!m_font)
		{
			fin.close();
			return false;
		}

		//Read in the font size and spacing between chars.
		fin.open(fontFileName);
		if (fin.fail())
		{
			fin.close();
			return false;
		}

		//read in the 95 used ascii characters for text
		for (int i = 0; i < 95; i++)
		{
			fin.get(temp);
			while (temp != ' ')
			{
				fin.get(temp);
			}
			fin.get(temp);
			while (temp != ' ')
			{
				fin.get(temp);
			}

			char c = i + 32;

			c;
			fin >> m_font[i].left;
			fin >> m_font[i].right;
			fin >> m_font[i].size;

			int test = 0;
		}

		//close the file
		fin.close();

		return true;
	}

	//releaseFontData-- releases the data for the font
	void Font::releaseFontData()
	{
		if (m_font)
		{
			delete[] m_font;
			m_font = 0;
		}
	}

	//loadTexture-- loads the texture for the font
	//device- the device to use to load the texture
	//textureFileName- the name of the textures file
	bool Font::loadTexture(ID3D11Device* device, WCHAR* textureFileName)
	{
		bool result;

		//Create the texture object.
		m_texture = new Texture;
		if (!m_texture)
		{
			return false;
		}

		//init the texture object
		result = m_texture->init(device, textureFileName);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//releaseTexture-- releases the data for the texture
	void Font::releaseTexture()
	{
		if (m_texture)
		{
			m_texture->shutdown();
			delete m_texture;
			m_texture = NULL;
		}
	}

	//loadBetterFontData-- replaces the old load font data method
	//fontFileName- the name of the file to load
	bool Font::loadBetterFontData(char* fontFileName)
	{
		string line;
		string read, key, value;
		size_t i;
		fstream stream;

		//open stream
		stream.open(fontFileName);

		if (stream.fail())
		{
			//return false, file could not be found or was bad
			return false;
		}

		//continue reading the file
			while (!stream.eof())
			{
				stringstream iss;
				getline(stream, line);
				iss << line;
				//read the lines type
				iss >> read;
				if (read == "common")
				{
					while (!iss.eof())
					{
						stringstream converter;
						iss >> read;
						i = read.find('=');
						key = read.substr(0, i);
						value = read.substr(i + 1);

						//assign the correct value
						converter << value;
						//choose the destination for the key
						if (key == "lineHeight")
						{
							converter >> m_charSet->lineHeight;
						}
						else
							if (key == "base")
							{
								converter >> m_charSet->base;
							}
							else if (key == "scaleW")
							{
								converter >> m_charSet->width;
							}
							else if (key == "scaleH")
							{
								converter >> m_charSet->height;
							}
							else if (key == "pages")
							{
								converter >> m_charSet->pages;
							}
							
					}

				}
				else if (read == "char")
				{
					//this is data for a specific char
					unsigned short charID = 0;

					while (!iss.eof())
					{
						stringstream converter;
						iss >> read;
						i = read.find('=');
						key = read.substr(0, 1);
						value = read.substr(i + 1);
					    //assign the correct value
					    converter << value;
					    if (key == "id")
					    {
							converter >> charID;
					    }
						else if (key == "x")
						{
							converter >> m_charSet->Chars[charID].x;
						}
						else if (key == "y")
						{
							converter >> m_charSet->Chars[charID].y;
						}
						else if (key == "width")
						{
							converter >> m_charSet->Chars[charID].width;
						}
						else if (key == "height")
						{
							converter >> m_charSet->Chars[charID].height;
						}
						else if (key == "xoffset")
						{
							converter >> m_charSet->Chars[charID].xoffset;
						}
						else if (key == "yoffset")
						{
							converter >> m_charSet->Chars[charID].yoffset;
						}
						else if (key == "xadvance")
						{
							converter >> m_charSet->Chars[charID].xadvance;
						}
						else if (key == "page")
						{
							converter >> m_charSet->Chars[charID].page;
						}
					}


				}
			


		}
		return true;
	}
}