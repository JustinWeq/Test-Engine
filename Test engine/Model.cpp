#include "Model.h"
namespace JR_Model
{
	//defualt constructor--creates a new Model instance with defualt parameters
	Model::Model()
	{
		m_indexBuffer = NULL;
		m_indexCount = NULL;
		m_texture = NULL;
		m_vertexBuffer = NULL;
		m_vertexCount = NULL;
	}

	//deconstructor--  cleans up dynamic memory of the model object
	Model::~Model()
	{
		shutdown();
	}

	//init-- initializes the model
	//device- the device to use for initialization
	//modelFileName- the name of the model to load
	//textureFilename-- the name of the texture to load
	bool Model::init(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
	{
		bool result;

		//load in the model data
		result = loadModel(modelFilename);
		if (!result)
		{
			return false;
		}

		//init the vertex and index buffers
		result = initBuffers(device);
		if (!result)
		{
			return false;
		}

		//Load the texture for this model
		result = loadTexture(device, textureFilename);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//shutdown-- releases all dynamic memory
	void Model::shutdown()
	{
		//release the models texture
		releaseTexture();
		//Shutdown the vertex and index buffers.
		shutdownBuffers();
		//Release the model data
		releaseModel();
	}

	//render-- renders the model
	//deviceContext- the device context to use for rendering
	void Model::render(ID3D11DeviceContext* deviceContext)
	{
		//Put the vertex and index buffers into the device context to preparre them for rendering
		renderBuffers(deviceContext);
	}

	//getIndexCount-- returns the number of indices for this model
	int Model::getIndexCount()
	{
		return m_indexCount;
	}

	//getTexture-- returns the texture for this model
	ID3D11ShaderResourceView* Model::getTexture()
	{
		return  m_texture->getTexture();
	}

	//initBuffers-- initializes the vertex and index buffer for this model
	//device- the device to use for initialization
	bool Model::initBuffers(ID3D11Device* device)
	{
		Vertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		//create the vertex array
		vertices = new Vertex[m_vertexCount];
		if (!vertices)
		{
			return false;
		}

		//Create the index array
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return  false;
		}

		//Load the vertex array and index array with data
		for (int i = 0; i < m_vertexCount; i++)
		{
			vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].z, m_model[i].y);
			vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
			vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

			indices[i] = i;
		}

		//Set up the description of the static vertex buffer
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex)* m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		//Give the subresource structure a pointer to the vertex data
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Now create a vertex buffer
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Set up the description of the static index buffer
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		//give the subresouce data for the index buffer a pointer to the indicies
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;


		//Create hte index buffer
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Release the arrays now that the vertex and index arrays are no longer needed
		delete[] vertices;
		vertices = NULL;

		delete[] indices;
		indices = NULL;

		return true;
	}

	//shutdownBuffers-- cleans up memory for the buffers
	void Model::shutdownBuffers()
	{
		//release the buffers for the verticies and indices
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = NULL;
		}

		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = NULL;
		}

		return;
	}

	//renderBuffers-- renders the buffers
	//deviceContext- the device context to put the index and vertex buffers on
	void Model::renderBuffers(ID3D11DeviceContext* deviceContext)
	{
		unsigned int stride;
		unsigned int offset;

		//set the vertex buffer stride and offset
		stride = sizeof(Vertex);
		offset = 0;

		//Set the vertex buffer to be active on the device context
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		//Set the index buffer to active in the input assembler so it can be renderered
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//set the type of primitive topology to use for rendering
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}