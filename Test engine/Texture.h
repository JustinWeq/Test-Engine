#pragma once
#include <D3D11.h>
#include <d3dx11tex.h>
namespace JR_Texture
{
	//Texture-- a class that contains methods and propertys for
	// a texture to be used on a model
	class Texture
	{
	public:
		//defualt constructor-- creates a new instance of Texture with defualt parameters
		Texture();
		//deconstructor-- cleans up dynamic memory for the Texture class
		~Texture();

		bool init();
		void shutdown();

		ID3D11ShaderResourceView* getTexture();
	private:
		ID3D11ShaderResourceView* m_texture;
	};
}