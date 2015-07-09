#include "Texture.h"
namespace JR_Texture
{
	//defualt constructor-- creates a new instance of Texture with defualt parameters
	Texture::Texture()
	{
		m_texture = NULL;
	}

	//deconstructor-- cleans up dynamic memory for the Texture class
	Texture::~Texture()
	{
		shutdown();
	}

	//init-- initializes the texture
	//device- the device to use for initialization
	//filename- the name of the file to load the texture from
	bool Texture::init(ID3D11Device* device, WCHAR* filename)
	{
		HRESULT result;

		//load in the texture
		result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture,NULL);
		if (FAILED(result))
		{
			return false;
		}
	}

	//shutdown-- cleans up and releases the dynamic memory for the texture
	void Texture::shutdown()
	{
		if (m_texture)
		{
			m_texture->Release();
			m_texture = NULL;
		}
	}


	//getTexture-- returns the texture
	ID3D11ShaderResourceView* Texture::getTexture()
	{
		return m_texture;
	}
}