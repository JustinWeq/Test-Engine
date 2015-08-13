#pragma once
//includes
#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include "Texture.h"
using namespace std;
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

		float getStringWidth(char* text);

	private:
		//loadFontData-- loads the data for the font
		//fontFileName- the name of the font file
		bool loadFontData(char* fontFileName);

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

	};
}