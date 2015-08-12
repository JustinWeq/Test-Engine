#pragma once

//includes
#include <d3d11.h>
#include <D3DX10math.h>

#include "Texture.h"
using namespace JR_Texture;
namespace JR_Bitmap
{
	//Bitmap- a class that contains methods and properties for drawing a 2D bitmap
	class Bitmap
	{
	private:
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
		};
	public:

		//defualt constructor-- constructs a new instance of Bitmap with defualt parameters'
		Bitmap();

		//decontructor-- deletes dynamic memory and cleans it up
		~Bitmap();

		//init-- initializes the bitmap
		//device- the device to use for initiazation
		//screenWidth- the width of the screen that is being drawn to
		//screenHight- the height of the screen being drawn to
		//textureFileName- the file name of the texture to use(uncompresed formats only)
		//bitmapWidth- the width of the bitmap
		//bitmapHeight- the hight of the bitmap
		bool init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename,
			int bitmapWidth, int bitmapHeight);

		//shutdown- shuts down and cleans up the memory for this bitmap
		void shutdown();

		//render-- renders the bitmap to the deviceContext
		//deviceContext- the device context to use for rendering
		//positionX- the x position to draw the bitmap to
		//positionY- the y position to draw the bitmap to
		bool render(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

		//getIndexCount--returns the number of indices for this bitmap(typicaly 6)
		int getIndexCount();

		//getTexture- returns the texture of this bitmap
		ID3D11ShaderResourceView* getTexture();

	private:
		//initBuffers-- sets up the vertex and index buffers
		//device- the device to use for setting up the buffers
		bool initBuffers(ID3D11Device* device);

		//shutDownBuffers--shuts down the buffers
		void shutdownBuffers();

		//updateBuffers-- updates the buffer information
		//deviceContext- the device context to send the buffers to
		//positionX- the x position to draw the bitmap to
		//positionY- the y position to draw the bitmap to
		bool updateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

		//renderBuffers-- sends the index and vertex buffer to the device context
		//deviceContext- the device context to use for rendering
		bool renderBuffers(ID3D11DeviceContext* deviceContext);

		//loadTexture-- loads the texture for the bitmap
		//device- the divice to use for loading the texture
		//name- the filename of the texture
		bool loadTexture(ID3D11Device* device, WCHAR* name);

		//releaseTexture-- cleans up memory for the texture
		void releaseTexture();

	private:
		//vertexBuffer- the vertex buffer for this model
		ID3D11Buffer *m_vertexBuffer;
		//indexBuffer- the index buffer for this model
		ID3D11Buffer *m_indexBuffer;
		//vertexCount- the number of vertices for this model
		int m_vertexCount;
		//indexCount- the number of indices for this model
		int m_indexCount;
		//texture- the texture for this model
		Texture* m_texture;

		//screenWidth- the width of the screen
		int m_screenWidth;
		//screenHeight the hight of the screen
		int m_screenHeight;
		//bitmapWidth- the width of this bitmap
		int m_bitmapWidth;
		//bitmapHeight- the height of this bitmap
		int m_bitmapHeight;
		// previousPosX- the last x position of this bitmap
		int m_previousPosX;
		//previousPosY- the last y position of this bitmap
		int m_previousPosY;

	};
}