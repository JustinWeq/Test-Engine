#include "Terrain.h"

namespace JR_Terrain
{
	//defualt constructor-- creates a new instance of Terrain with defualt parameters
	Terrain::Terrain()
	{
		m_indexbuffer = NULL;
		m_indexCount = 0;
		m_terrainHeight = 0;
		m_terrainWidth = 0;
		m_vertexBuffer = NULL;
		m_vertexCount = 0;
	}

	//deconstructor-- cleans up memory for Terrain
	Terrain::~Terrain()
	{
		//call shutdown
		shutdown();
	}

	//init-- initalizes the Terrain class
	//device- the device to use for initializtion
	//width- the number of tiles for the terrain
	//height- the number of tiles for the height of the terrain
	bool Terrain::init(ID3D11Device* device, int width, int height)
	{
		bool result;

		//store the width and height of the terrain
		m_terrainWidth = width;
		m_terrainHeight = height;

		//init the vertex and index buffer to hold the model for the terrain
		result = initBuffers(device);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//shutdown-- shutdown and cleans up memory for this Terrain instance
	void Terrain::shutdown()
	{
		//release the vertex and index buffers
		shutdownBuffers();
	}
	//render-- puts the model for this terrain on the pipeline and prepares it for rendering
	//deviceContext- the device context that will be used for rendering
	void Terrain::render(ID3D11DeviceContext* deviceContext)
	{
		//put the vertex and index buffers on the pipeline
		renderBuffers(deviceContext);
	}

	//getIndexCount- returns the number of indices for this Terrains model
	int Terrain::getIndexCount()
	{
		return m_indexCount;
	}

	//initBuffers-- initializes the buffers for the terrain model
	//device- the device to use for intialization
	bool Terrain::initBuffers(ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		int index, i, j;
		float positionX, positionZ;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Calculate the number of vertices in the terrain mesh.
		m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 8;

		// Set the index count to the same as the vertex count.
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

		// Initialize the index to the vertex array.
		index = 0;

		// Load the vertex and index arrays with the terrain data.
		for (j = 0; j<(m_terrainHeight - 1); j++)
		{
			for (i = 0; i<(m_terrainWidth - 1); i++)
			{
				// LINE 1
				// Upper left.
				positionX = (float)i;
				positionZ = (float)(j + 1);

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// Upper right.
				positionX = (float)(i + 1);
				positionZ = (float)(j + 1);

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// LINE 2
				// Upper right.
				positionX = (float)(i + 1);
				positionZ = (float)(j + 1);

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// Bottom right.
				positionX = (float)(i + 1);
				positionZ = (float)j;

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// LINE 3
				// Bottom right.
				positionX = (float)(i + 1);
				positionZ = (float)j;

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// Bottom left.
				positionX = (float)i;
				positionZ = (float)j;

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// LINE 4
				// Bottom left.
				positionX = (float)i;
				positionZ = (float)j;

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;

				// Upper left.
				positionX = (float)i;
				positionZ = (float)(j + 1);

				vertices[index].position = D3DXVECTOR3(positionX, 0.0f, positionZ);
				vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				indices[index] = index;
				index++;
			}
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
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
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexbuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Release the arrays now that the buffers have been created and loaded.
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;

		return true;
	}

	void Terrain::shutdownBuffers()
	{
		//Release the index buffer
		if (m_indexbuffer)
		{
			m_indexbuffer->Release();
			m_indexbuffer = 0;
		}

		//release the vertex buffer
		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = NULL;
		}
	}

	//renderBuffers-- preapres the buffers for rendering
	//deviceContext- the device context to use for rendering
	void Terrain::renderBuffers(ID3D11DeviceContext* deviceContext)
	{
		unsigned int stride;
		unsigned int offset;

		//Set vertex buffer stride and offset
		stride = sizeof(VertexType);
		offset = 0;

		//Set the vertex buffer to active in the input assembler so it can be rendererd
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		//Set the index buffer to active int the input assembler so it can be rendered to.
		deviceContext->IASetIndexBuffer(m_indexbuffer, DXGI_FORMAT_R32_UINT, 0);

		//Set the type of primitive layout
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
}