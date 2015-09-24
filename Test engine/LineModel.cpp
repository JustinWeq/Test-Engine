#include "LineModel.h"


	//defualt constructor-- creates a new instance of LineModel with defualt parameters
	LineModel::LineModel()
	{
		m_vertexBuffer = NULL;
	}

	//deconstructor-- destroys and cleans up ememory for the LineModel
	LineModel::~LineModel()
	{
		shutDown();
	}

	//init-- sets up the LineModel instance
	//model- the vertices for this model
	//color- the color for the model
	//vertexCount- the number of vertices for the passed in model
	bool LineModel::init(ID3D11Device* device, D3DXVECTOR3* model, D3DXVECTOR4 color, int vertexCount)
	{
		Vertex* nmodel;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexDesc;
		D3D11_SUBRESOURCE_DATA vertexData;
		HRESULT result;
		m_indexCount = vertexCount;
		indices = new unsigned long[m_indexCount];
		if (!indices)
		{
			return false;
		}

		nmodel = new Vertex[vertexCount];
		if (!nmodel)
		{
			return false;
		}



		//copy passed in information to the new model
		for (int i = 0;i < vertexCount;i++)
		{
			nmodel[i].position = model[i];
			nmodel[i].color = color;

			int stop = 0;
		}

		//set up the decription of the vertex buffer
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(Vertex)*vertexCount;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		//fill the subresource data with information
		vertexData.pSysMem = nmodel;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Now create the vertex buffer for the model
		result = device->CreateBuffer(&vertexDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		
		

		delete[] nmodel;

		return true;

	}

	//shutDown()-- cleans up memory for the line model
	void LineModel::shutDown()
	{
		//release buffers
		releaseBuffers();
	}

	//render-- prepares the model for rendering
	//deviceContext- the deivce context to use for rednering
	void LineModel::render(ID3D11DeviceContext* deviceContext)
	{
		//call render bufers method
		// to prepare the buffers for rendering
		renderBuffers(deviceContext);
	}

	//getIndexCount- returns the index count
	int LineModel::getIndexCount()
	{
		return m_indexCount;
	}

	//releaseBuffers-- releases the memory for the buffer of this model
	void LineModel::releaseBuffers()
	{
		//release the buffers
		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			delete m_vertexBuffer;
			m_vertexBuffer = NULL;
		}
		
	}

	//renderBuffers-- sets up this model instance to be rendered on the passed in
	// device context
	//deviceContext- the device context to use for rendering
	void LineModel::renderBuffers(ID3D11DeviceContext* deviceContext)
	{
		//set the primitive topology on the device context
		//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
		//set the vertex buffer on the device context
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer,&stride ,& offset);


		//set the primitive topology on the device context
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
