#include "Bitmap.h"

namespace JR_Bitmap
{
	//defualt constructor-- constructs a new instance of Bitmap with defualt parameters'
	Bitmap::Bitmap()
	{
		m_vertexBuffer = 0;
		m_indexBuffer = 0;
		m_texture = 0;
	}



	//decontructor-- deletes dynamic memory and cleans it up
	Bitmap::~Bitmap()
	{
		shutdown();
	}

	//init-- initializes the bitmap
	//device- the device to use for initiazation
	//screenWidth- the width of the screen that is being drawn to
	//screenHight- the height of the screen being drawn to
	//textureFileName- the file name of the texture to use(uncompresed formats only)
	//bitmapWidth- the width of the bitmap
	//bitmapHeight- the hight of the bitmap
	bool Bitmap::init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename,
		int bitmapWidth, int bitmapHeight)
	{
		bool result;

		//Store the screen size
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		//Store the size in pixels that this bitmap should be rendered at.
		m_bitmapWidth = bitmapWidth;
		m_bitmapHeight = bitmapHeight;

		//init the previous rendering position to negative one.
		m_previousPosX = -1;
		m_previousPosY = -1;

		// Initialize the vertex and index buffers.
		result = initBuffers(device);
		if (!result)
		{
			return false;
		}

		// Load the texture for this model.
		result = loadTexture(device, textureFilename);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//shutdown- shuts down and cleans up the memory for this bitmap
	void Bitmap::shutdown()
	{
		// Release the model texture.
		releaseTexture();

		// Shutdown the vertex and index buffers.
		shutdownBuffers();

		return;
	}

	//render-- renders the bitmap to the deviceContext
	//deviceContext- the device context to use for rendering
	//positionX- the x position to draw the bitmap to
	//positionY- the y position to draw the bitmap to
	bool Bitmap::render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
	{
		bool result;

		//Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
		result = updateBuffers(deviceContext, positionX, positionY);
		if (!result)
		{
			return false;
		}
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		renderBuffers(deviceContext);

		return true;
	}

	//getIndexCount--returns the number of indices for this bitmap(typicaly 6)
	int Bitmap::getIndexCount()
	{
		return m_indexCount;
	}

	//getTexture- returns the texture of this bitmap
	ID3D11ShaderResourceView* Bitmap::getTexture()
	{
		return m_texture->getTexture();
	}

	//initBuffers-- sets up the vertex and index buffers
	//device- the device to use for setting up the buffers
	bool Bitmap::initBuffers(ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;


		// Set the number of vertices in the vertex array.
		m_vertexCount = 6;

		// Set the number of indices in the index array.
		m_indexCount = m_vertexCount;

		// Create the vertex array.
		vertices = new VertexType[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return false;
		}

		//init the vertex array to zeros at first.
		memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

		//load the index array with data
		for (int i = 0; i < m_indexCount; i++)
		{
			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;

		return true;
	}

	//shutDownBuffers--shuts down the buffers
	void Bitmap::shutdownBuffers()
	{
		// Release the index buffer.
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = 0;
		}

		// Release the vertex buffer.
		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = 0;
		}

		return;
	}

	//updateBuffers-- updates the buffer information
	//deviceContext- the device context to send the buffers to
	//positionX- the x position to draw the bitmap to
	//positionY- the y position to draw the bitmap to
	bool Bitmap::updateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
	{
		float left, right, top, bottom;
		VertexType* vertices;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType* verticesPtr;
		HRESULT result;

		//if the position we are rendering this bitmap to has not changed then dont update the vertex buffer since it
		//currently has the correct parameters.
		if ((!positionX == m_previousPosX) && (positionY == m_previousPosY))
		{
			return true;
		}

		//If it has changed then update the position it is being rendered to
		m_previousPosX = positionX;
		m_previousPosY = positionY;

		// Calculate the screen coordinates of the left side of the bitmap.
		left = (float)((m_screenWidth / 2)*-1)+(float)positionX;

		// Calculate the screen coordinates of the right side of the bitmap.
		right = left + (float)m_bitmapWidth;

		// Calculate the screen coordinates of the top of the bitmap.
		top = (float)(m_screenHeight / 2) - (float)positionY;

		// Calculate the screen coordinates of the bottom of the bitmap.
		bottom = top - (float)m_bitmapHeight;

		//Create the vertex array
		vertices = new VertexType[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		//Load the vertex array with data
		//First triangle
		vertices[0].position = D3DXVECTOR3(left, top, 0.0f);//top left
		vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);//bottom right
		vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

		vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);//bottom left
		vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

		//Second triangle
		vertices[3].position = D3DXVECTOR3(left, top, 0.0f);//top left
		vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[4].position = D3DXVECTOR3(right, top, 0.0f);//top right
		vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

		vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);//bottom right
		vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

		//Lock the vertex buffer so it can be written to
		result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the vertex buffer.
		verticesPtr = (VertexType*)mappedResource.pData;

		//Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, sizeof(VertexType)*m_vertexCount);

		//Unlock the vertex buffer
		deviceContext->Unmap(m_vertexBuffer, 0);

		//Release the vertex array as it is no longer needed.
		delete[] vertices;
		vertices = NULL;

		return true;


	}

	//renderBuffers-- sends the index and vertex buffer to the device context
	//deviceContext- the device context to use for rendering
	bool Bitmap::renderBuffers(ID3D11DeviceContext* deviceContext)
	{
		unsigned int stride;
		unsigned int offset;


		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		return true;
	}

	//loadTexture-- loads the texture for the bitmap
	//device- the divice to use for loading the texture
	//name- the filename of the texture
	bool Bitmap::loadTexture(ID3D11Device* device, WCHAR* filename)
	{
		bool result;


		// Create the texture object.
		m_texture = new Texture();
		if (!m_texture)
		{
			return false;
		}

		// Initialize the texture object.
		result = m_texture->init(device, filename);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//releaseTexture-- cleans up memory for the texture
	void Bitmap::releaseTexture()
	{
		// Release the texture object.
		if (m_texture)
		{
			m_texture->shutdown();
			delete m_texture;
			m_texture = 0;
		}

		return;
	}
}