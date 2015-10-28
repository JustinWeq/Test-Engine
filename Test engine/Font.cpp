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

		//load in the text file contianing the font data
		result = loadFontData(fontFileName);
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

			fin >> m_font[i].left;
			fin >> m_font[i].right;
			fin >> m_font[i].size;
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
}