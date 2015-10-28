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

		//init-- initializes the texture
		//device- the device to use for initialization
		//filename- the name of the file to load the texture from
		bool init(ID3D11Device* device, LPCSTR filename);
		//shutdown-- cleans up and releases the dynamic memory for the texture
		void shutdown();

		//getTexture-- returns the texture
		ID3D11ShaderResourceView* getTexture();
	private:
		//texture-- the actual texture data for this texture class
		ID3D11ShaderResourceView* m_texture;
	};
}