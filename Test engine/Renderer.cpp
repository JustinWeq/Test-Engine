#include "Renderer.h"

namespace JR_Renderer
{

	//defualt constructor-- creates a new instance of Rednerer with defualt parameters
	Renderer::Renderer()
	{
		//initialize all pointer to NULL(0)
		m_indexBuffer = NULL;
		m_instanceBuffer = NULL;
		m_pixelShader = NULL;
		m_vertexBuffer = NULL;
		m_vertexShader = NULL;
	}

	//deconstructor- cleans up dynamic memory for
	Renderer::~Renderer()
	{
		//call shutdown to clean up memory
		shutdown();
	}

	//init-- initializes the Renderer
	//device- the device to use for intitialization
	//screenWidth- the width of the screen
	//screenHeight- the height of the screen
	//hwnd- the handle to the window, used for outputting debugging information
	bool Renderer::init(ID3D11Device* device,int screenWidth,int screenHeight,HWND hwnd)
	{
		bool result;
		//init the vertex buffers
		result = initBuffers(device);
		if (!result)
		{
			return false;
		}

		//init the shaders
		result = initShaders(device,hwnd);
		if (!result)
		{
			return false;
		}

		//create the matricies
		createMatricies(screenWidth,screenHeight);

		return true;
	}

	//draw-- adds the passed in vertices to the list to be drawn
	//matrix- the matrix to be used for drawing, this is calculated aginst the ortho and view matricies
	//textureID- the id of the texture to draw,leave this -1 to not use a text
	//color- the color to draw it
	//UVAdd- the moust to add to the 0 coords
	//UVMultiply- the amount to multiply the one coords with
	bool Renderer::draw(D3DXMATRIX matrix, int textureID, D3DXVECTOR4 color,D3DXVECTOR2 UVAdd,D3DXVECTOR2 UVMultiply)
	{
		InstanceVertex2 instance;
		instance.matrixInstance = matrix*m_view*m_ortho;
		instance.color = color;
		instance.textureID = textureID;
		instance.UVAdd = UVAdd;
		instance.UVMultiply = UVMultiply;
		//multiply the matrix bt the view and orth matricies to get the final matrix position;
		m_vertices.push_back(instance);

		return true;
	}

	//deviceContext- the device context to use for drawing
	//deviceContext
	bool Renderer::presentDraw(ID3D11DeviceContext* deviceContext)
	{
		bool result;
		//set up the buffers for drawing
		result = updateBuffers(deviceContext);
		if (!result)
		{
			return false;
		}

		//now render the buffers
		result = drawBuffers(deviceContext);
		if (!result)
		{
			return false;
		}

		//set the textures on the buffer
		ID3D11ShaderResourceView** textures = new ID3D11ShaderResourceView*[m_textures.size()];

		//get the textures into the new array
		for (int i = 0;i < m_textures.size();i++)
		{
			textures[i] = m_textures[i];
		}

		deviceContext->VSSetShader(m_vertexShader, NULL, 0);

		deviceContext->PSSetShader(m_pixelShader, NULL, 0);

		deviceContext->IASetInputLayout(m_layout);

		//now set the textures into the Input Assembler
		deviceContext->PSSetShaderResources(0, m_textures.size(), textures);

		deviceContext->PSSetSamplers(0, 1, &m_sampler);

		deviceContext->DrawIndexedInstanced(6, m_vertices.size(),0,0,0);

		//flush the vertex buffer
		m_vertices.clear();

		//return true since drawing was successful at this point
		return true;
	}

	//addTexture
	//texture- the texture to add, remember no more then 30 textures can be loaded at once,
	// returns false if the maximum number of textures has already been reached
	bool Renderer::addTexture(Texture texture)
	{
		//add the texture to the list of textures if the number of textures
		// in the list is not MAX_TEXTURES or greater
		if (m_textures.size() < MAX_TEXTURES)
		{
			//add the texture to the list
			m_textures.push_back(texture.getTexture());
			//return true since texture was added
			return true;
		}

		//return false since texture was not added
		return false;
	}

	//shutdown-- shutsdown and deletes dynamic memory for the renderer
	void Renderer::shutdown()
	{


		//delete buffers
		shutdownBuffers();

		//delete shaders 
		shutdownShaders();
	}

	//initBuffers-- initializes the buffers
	//device- the device to use for initialization
	bool Renderer::initBuffers(ID3D11Device* device)
	{
		InstanceVertex1* vertices = new InstanceVertex1[6];
		unsigned long indices[6];
		D3D11_BUFFER_DESC vertexDesc, indexDesc,instanceDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData,instanceData;
		HRESULT result;

		//set up the texture list
		m_textures =  vector<ID3D11ShaderResourceView*>();

		//set up the secondary instance buffer
		m_vertices =  vector<InstanceVertex2>();

		//fill the main vertex buffer with information

		//First triangle
		vertices[0].position = D3DXVECTOR3(0, 0, 0.0f);//top left
		vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[1].position = D3DXVECTOR3(0,1, 0.0f);//bottom left
		vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

		vertices[2].position = D3DXVECTOR3(1, 1, 0.0f);//bottom right
		vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

		//Second triangle
		vertices[3].position = D3DXVECTOR3(1, 0, 0.0f);//top left
		vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[4].position = D3DXVECTOR3(0, 0, 0.0f);//top right
		vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

		vertices[5].position = D3DXVECTOR3(1, 1, 0.0f);//bottom right
		vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

		//set the indices
		for (int i = 0;i < 6;i++)
		{
			indices[i] = i;
		}

		//set up the vertex buffer description
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(InstanceVertex1) * 6;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		//set up the vertex data structure
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//create the vertex buffer
		result = device->CreateBuffer(&vertexDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//set up the index buffer description
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.ByteWidth = sizeof(unsigned long) * 6;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		//set up the index data structure
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&indexDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		instanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceDesc.ByteWidth = sizeof(InstanceVertex2) * MAX_INSTANCES;
		instanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		instanceDesc.MiscFlags = 0;
		instanceDesc.StructureByteStride = 0;
		instanceDesc.Usage = D3D11_USAGE_DYNAMIC;

		InstanceVertex2* instanceVertex = new InstanceVertex2[MAX_INSTANCES];

			instanceVertex[0].color = D3DXVECTOR4(1, 1, 0, 1);
		
		instanceData.pSysMem = instanceVertex;
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;
		
		//create the instance buffer
		result = device->CreateBuffer(&instanceDesc, &instanceData, &m_instanceBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//delete vertex buffer since it is no longer being used
		return true;
	}

	//initShaders-- initializes the shaders
	//device- the device to use for initialization
	//hwnd- the handle to the window used for debugging
	bool Renderer::initShaders(ID3D11Device* device,HWND hwnd)
	{

		HRESULT result;
		ID3D10Blob* errorMessage;
		ID3D10Blob* pixelShaderBuffer;
		ID3D10Blob* vertexShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[10];
		D3D11_SAMPLER_DESC samplerDesc;
		unsigned int numElements;

		//init the pointers this method will use to null
		errorMessage = NULL;
		vertexShaderBuffer = NULL;
		pixelShaderBuffer = NULL;

		//compile the vertex shader code
		result = D3DX11CompileFromFile(L"defualt.vs", NULL, NULL, "instancedVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&vertexShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//if the shader faild to compile display error message to the user
			if (errorMessage)
			{
				outputShaderErrorMessage(errorMessage, hwnd, L"defualt.vs");
			}
			else
			{
				//theres nothing in the error message
				// so it could not find the shader file itself
				MessageBox(hwnd, L"defualt.vs", L"Could not find the file for the shader", MB_OK);
			}

			return false;
		}

		//create the vertex shader
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
		if (FAILED(result))
		{
			return false;
		}

		//compile the pixel shader code
		result = D3DX11CompileFromFile(L"defualt.ps", NULL, NULL, "instancedPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&pixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//if the shader faild to compile display error message to the user
			if (errorMessage)
			{
				outputShaderErrorMessage(errorMessage, hwnd, L"defualt.ps");
			}
			else
			{
				//theres nothing in the error message
				// so it could not find the shader file itself
				MessageBox(hwnd, L"defualt.ps", L"Could not find the file for the shader", MB_OK);
			}

			return false;
		}

		//create the pixel shader
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
		if (FAILED(result))
		{
			return false;
		}

		//vertex position, by vertex
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].SemanticName = "POSITION";

		//uv coords, by vertex
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].SemanticName = "TEXCOORD";

		//texture ID, by instance
		polygonLayout[2].AlignedByteOffset = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32_SINT;
		polygonLayout[2].InputSlot = 1;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[2].InstanceDataStepRate = 1;
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].SemanticName = "TEXTUREID";

		//color, by instance
		polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[3].InputSlot = 1;
		polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[3].InstanceDataStepRate = 1;
		polygonLayout[3].SemanticIndex = 0;
		polygonLayout[3].SemanticName = "COLOR";
		
		//UVAdd , by instance
		polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[4].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[4].InputSlot = 1;
		polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[4].InstanceDataStepRate = 1;
		polygonLayout[4].SemanticIndex = 0;
		polygonLayout[4].SemanticName = "UVADD";

		//UVMultiply, by instance
		polygonLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[5].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[5].InputSlot = 1;
		polygonLayout[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[5].InstanceDataStepRate = 1;
		polygonLayout[5].SemanticIndex = 0;
		polygonLayout[5].SemanticName = "UVMULTIPLY";

		//matrix row 1
		polygonLayout[6].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[6].InputSlot = 1;
		polygonLayout[6].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[6].InstanceDataStepRate = 1;
		polygonLayout[6].SemanticIndex = 0;
		polygonLayout[6].SemanticName = "MATRIX";

		//matrix row 2
		polygonLayout[7].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[7].InputSlot = 1;
		polygonLayout[7].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[7].InstanceDataStepRate = 1;
		polygonLayout[7].SemanticIndex = 1;
		polygonLayout[7].SemanticName = "MATRIX";

		//matrix row 3
		polygonLayout[8].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[8].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[8].InputSlot = 1;
		polygonLayout[8].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[8].InstanceDataStepRate = 1;
		polygonLayout[8].SemanticIndex = 2;
		polygonLayout[8].SemanticName = "MATRIX";

		//matrix row 4
		polygonLayout[9].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[9].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[9].InputSlot = 1;
		polygonLayout[9].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		polygonLayout[9].InstanceDataStepRate = 1;
		polygonLayout[9].SemanticIndex = 3;
		polygonLayout[9].SemanticName = "MATRIX";

		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		//create the input layout
		result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
		if (FAILED(result))
		{
			MessageBox(hwnd, TEXT("Failed to create the input layout"), TEXT("Error initializaing shader"), MB_OK);
			return false;
		}

		//create the sampler description
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		result = device->CreateSamplerState(&samplerDesc, &m_sampler);
		if (FAILED(result))
		{
			return false;
		}

		return true;
		
	}

	//shutdownBuffers-- shutsdown and deletes the buffers for the renderer
	void Renderer::shutdownBuffers()
	{
		//shutdown the buffers
		if (m_instanceBuffer)
		{
			m_instanceBuffer->Release();
			delete m_instanceBuffer;
			m_instanceBuffer = NULL;
		}

		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			delete m_indexBuffer;
			m_indexBuffer = NULL;
		}

		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			delete m_vertexBuffer;
			m_vertexBuffer = NULL;
		}
	}

	//shutdownShaders-- shutdown the shaders
	void Renderer::shutdownShaders()
	{
		//shutdown the vertex and pixel shaders
		if (m_vertexShader)
		{
			m_vertexShader->Release();
			delete m_vertexShader;
			m_vertexShader = NULL;
		}

		if (m_pixelShader)
		{
			m_pixelShader->Release();
			delete m_pixelShader;
			m_pixelShader = NULL;
		}
	}

	//updateBuffers-- updates the instance buffer with new information
	//deviceContext- the device context to update the buffers to
	bool Renderer::updateBuffers(ID3D11DeviceContext* deviceContext)
	{
		//update the buffer for the instances
		D3D11_MAPPED_SUBRESOURCE data;
		InstanceVertex2* instancesPtr = NULL;
		HRESULT result;


		result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		if (FAILED(result))
		{
			return false;
		}

		instancesPtr = (InstanceVertex2*)data.pData;
		for (int i = 0;i < m_vertices.size();i++)
		{
			instancesPtr[i].color = m_vertices[i].color;
			instancesPtr[i].matrixInstance = m_vertices[i].matrixInstance;
			instancesPtr[i].textureID = m_vertices[i].textureID;
			instancesPtr[i].UVAdd = m_vertices[i].UVAdd;
			instancesPtr[i].UVMultiply = m_vertices[i].UVMultiply;
		}

		//memcpy(instancesPtr, (void*)instances, sizeof(&m_vertices[0]));

		//now un map

         deviceContext->Unmap(m_instanceBuffer, 0);

		return true;

	}

	//drawBuffers-- renders the buffers 
	//deviceContext- the device context to use for rendering
	bool Renderer::drawBuffers(ID3D11DeviceContext* deviceContext)
	{
		unsigned int offsets[2];
		unsigned int strides[2];
		ID3D11Buffer* buffers[2];
		//set the buffers
		buffers[0] = m_vertexBuffer;
		buffers[1] = m_instanceBuffer;

		//set the strides
		strides[0] = sizeof(InstanceVertex1);
		strides[1] = sizeof(InstanceVertex2);

		//set the offsets
		offsets[0] = 0;
		offsets[1] = 0;
		//set the vertex buffers
		deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);

		//set the index buffers
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//buffers have been set return true
		return true;
	}

	//outputShaderErrorMessage-- outputs an error message to a textfile in the root directory of the .exe
	//errorMessage- the error message to output
	//hwnd- the parent window
	//shaderFilename- the name of the file to output to
	void Renderer::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
	{
		char* compileErrors;
		unsigned long bufferSize, i;
		ofstream fout;

		//get a pointer to the error message text buffer
		compileErrors = (char*)(errorMessage->GetBufferPointer());

		//Gett he length of the message
		bufferSize = errorMessage->GetBufferSize();

		//Open a file to write the error message to
		fout.open("shader-error.txt");

		//Write out the error message.
		for (int i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		//Close the file
		fout.close();

		//Release the error message
		errorMessage->Release();
		errorMessage = NULL;

		//pop up a message saying to check the text file for errors
		MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for details", shaderFilename, MB_OK);
	}

	//createMatricies
	void Renderer::createMatricies(int screenWidth, int screenHeight)
	{
		//create the orthographic matrix
		D3DXMatrixOrthoLH(&m_ortho, (float)screenWidth, (float)screenHeight, 0.001, 1000.0f);

		//create the view matrix
		D3DXMatrixLookAtLH(&m_view, &D3DXVECTOR3(0, 0, -10.0f), &D3DXVECTOR3(0, 0, -9.0f), &D3DXVECTOR3(0, 1, 0));
	}
}