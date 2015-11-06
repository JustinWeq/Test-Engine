#pragma once
//includes
#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include <string>
#include <sstream>
#include "Texture.h"
#include "Rectangle.h"
using namespace std;
using namespace JR_Rectangle;
using namespace JR_Texture;
namespace JR_Font
{
	//A simple class that contains font data
	class Font
	{
		struct FontType
		{
			float left, right;
			int size;
		};

		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
		};

		struct CharDescriptor
		{
			unsigned short x, y;
			unsigned short width, height;
			float xoffset, yoffset;
			float xadvance;
			unsigned short page;
		};

		struct CharSet
		{
			unsigned long lineHeight;
			unsigned short base;
			unsigned short width, height;
			unsigned short pages;
			CharDescriptor Chars[256];
		};

	public:
		//defualt constructor-- creates anew instance of Font with defualt parameters
		Font();

		//deconstructor-- deletes dynamic memory and cleans up this instance of Font
		~Font();

		//init- initializes the font class
		//device- the device to use for initialization
		//fontFileName- the name of the font filw
		//textureFileName- the name of the texture to use for the font
		bool init(ID3D11Device* device, char* fontFileName, WCHAR* textureFileName);

		//cleans up memory for the font class
		void shutdown();

		//getTexture-- returns the texture for this font
		ID3D11ShaderResourceView* getTexture();

		//buildVertexArray-- builds the vertex buffer to use for drawing the sentence
		//vertices- the vertex array to build onto
		//sentence- the sentence that will be displayed
		//drawX- the x draw coord
		//drawY- the y draw coord
		void buildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

		//buildRectangleArray-- builds the array of rectangles to draw
		//rectangles- the array of rectangles to copy to
		//sentence- the sentence the array of rectangles is based on
		//drawX- the x coord of the sentence
		//drawY- the y coord of the sentence
		//color- the color of the sentence
		//size- the size to multiply the normal font size by
		//textureChannel- the id for the texture
		void buildRectangleArray(void* rectangles,const char* sentence, float drawX, float drawY, D3DXVECTOR4 color, float size,int textureChannel);

		float getStringWidth(char* text);

	private:
		//loadFontData-- loads the data for the font
		//fontFileName- the name of the font file
		bool loadFontData(char* fontFileName);

		//loadBetterFontData-- replaces the old load font data method
		//fontFileName- the name of the file to load
		bool loadBetterFontData(char* fontFileName);
		//releaseFontData-- releases the data for the font
		void releaseFontData();

		//loadTexture-- loads the texture for the font
		//device- the device to use to load the texture
		//textureFileName- the name of the textures file
		bool loadTexture(ID3D11Device* device, WCHAR* textureFileName);

		//releaseTexture-- releases the data for the texture
		void releaseTexture();

	private:
		//font- the font data for the sentence
		FontType* m_font;
		//texture- the texture for the font
		Texture* m_texture;

		CharDescriptor* m_charDescriptions;

		CharSet* m_charSet;
	};
}