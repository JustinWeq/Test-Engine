#include "Shader.h"

namespace JR_Shader
{
	//defualt constructor-- creates a new Shader object with defualt parameters
	Shader::Shader()
	{
		//set all values to NULL
		m_cameraBuffer = NULL;
		m_layout = NULL;
		m_lightBuffer = NULL;
		m_matrixBuffer = NULL;
		m_pixelShader = NULL;
		m_sampleState = NULL;
		m_vertexShader = NULL;
	}

	Shader::~Shader()
	{
		shutdown();
	}

	//init-- initializes the shader
	//device- the ID3D11Deivce to use
	//hwnd- the hwnd(parent window)
	bool Shader::init(ID3D11Device* device, HWND hwnd)
	{
		bool result;

		//init the vertex and pixel shaders
		result = initShader(device, hwnd);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//shutdown-- Cleans up memory for the shader
	void Shader::shutdown()
	{
		shutdownShader();
	}

	//render-- renders using the shader and the passed in parameters
	//deviceContext- the device context to use for drawing
	//indexCount- the number of indicies
	//worldMatrix- the world matrix to use
	//viewMatrix- the view matrix to use
	//projectionMatrix- the projection matrix to use
	//texture- the texture to use for the model
	//lightDirection- the direction of the light
	//ambientColor- the color of the ambient light
	//diffuseColor- the color of the diffuse light
	//cameraPosition- the position of the camera
	//specularColor- the color of the specular light
	//specularPower- the power(reflectiveness) of the light
	bool Shader::render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
		D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
	{
		bool result;

		//set the shader parameters that it will use when drawing
		result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture,
			lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor, specularPower);
		if (!result)
		{
			return false;
		}

		//now render the prepared shader
		renderShader(deviceContext, indexCount);
	}

	//renderTexture-- renders using the shader and the passed in parameters
	//deviceContext- the device context to use for drawing
	//indexCount- the number of indicies
	//worldMatrix- the world matrix to use
	//viewMatrix- the view matrix to use
	//projectionMatrix- the projection matrix to use
	//texture- the texture to use for the model
	bool Shader::renderTexture(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
	{
		bool result;

		//set the shader parameters that it will use when drawing
		result = setTextureShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
		if (!result)
		{
			return false;
		}

		//now render the prepared shader
		renderTextureShader(deviceContext, indexCount);
	}

	//renderFont-- renders using the shader and the passed in parameters
	//deviceContext- the device context to use for drawing
	//indexCount- the number of indicies
	//worldMatrix- the world matrix to use
	//viewMatrix- the view matrix to use
	//projectionMatrix- the projection matrix to use
	//texture- the texture to use for the model
	//fontColor- the color of the font to use for rendering
	bool Shader::renderFont(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,D3DXVECTOR4 pixelColor)
	{
		bool result;

		//set the shader parameters that it will use when drawing
		result = setFontShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture,pixelColor);
		if (!result)
		{
			return false;
		}

		//now render the prepared shader
		renderFontShader(deviceContext, indexCount);
	}

	//initShader-- sets up the shader
	//device- The device to use to create the shader
	//hwnd- the parent window
	//vsFilename- the name of the vertex shader
	//psFilename- the name of the pixel shader
	bool Shader::initShader(ID3D11Device* device, HWND hwnd)
	{
		HRESULT result;
		ID3D10Blob* errorMessage;
		ID3D10Blob* vertexShaderBuffer;
		ID3D10Blob* pixelShaderBuffer;
		ID3D10Blob* textureVertexShaderBuffer;
		ID3D10Blob* texturePixelShaderBuffer;
		ID3D10Blob* fontPixelShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
		D3D11_INPUT_ELEMENT_DESC texturePolygonLayout[2];
		unsigned int numElements;
		D3D11_BUFFER_DESC matrixBufferDesc;
		D3D11_SAMPLER_DESC samplerDesc;
		D3D11_BUFFER_DESC cameraBufferDesc;
		D3D11_BUFFER_DESC lightBufferDesc;
		D3D11_BUFFER_DESC pixelColorBufferDesc;

		//init the pointers this method will use to null
		errorMessage = NULL;
		vertexShaderBuffer = NULL;
		pixelShaderBuffer = NULL;

		//compile the vertex shader code
		result = D3DX11CompileFromFile( L"defualt.vs", NULL, NULL, "DefualtVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&vertexShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//If the shader failed to compile it should have written something tot he error message
			if (errorMessage)
			{
				outputShaderErrorMessage(errorMessage, hwnd, L"defualt.vs");
			}
			else
			{
				//theres nothing in the error message
				// so it could not find the shader file itself
				MessageBox(hwnd, L"defualt.vs",  L"Could not find the file for the shader", MB_OK);
			}

			return false;
		}

		//compile the texture vertex shader code
		result = D3DX11CompileFromFile(L"defualt.vs", NULL, NULL, "textureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&textureVertexShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//If the shader failed to compile it should have written something tot he error message
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

		//Compile the pixel shader code
		result = D3DX11CompileFromFile(L"defualt.ps", NULL, NULL, "defualtPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&pixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//if the shader failed to sompile it should have written something to the error message
			if (errorMessage)
			{
				outputShaderErrorMessage(errorMessage, hwnd, L"defualt.ps");
			}
			else
			{
				//the was no error message so the program could not find the shader itself
				MessageBox(hwnd, L"defualt.ps", L"Missing Shader File", MB_OK);
			}
			return false;
		}

		//Compile the texture pixel shader code
		result = D3DX11CompileFromFile(L"defualt.ps", NULL, NULL, "texturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&texturePixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//if the shader failed to sompile it should have written something to the error message
			if (errorMessage)
			{
				outputShaderErrorMessage(errorMessage, hwnd, L"defualt.ps");
			}
			else
			{
				//the was no error message so the program could not find the shader itself
				MessageBox(hwnd, L"defualt.ps", L"Missing Shader File", MB_OK);
			}
			return false;
		}

		//Compile the font pixel shader code
		result = D3DX11CompileFromFile(L"defualt.ps", NULL, NULL, "fontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&fontPixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			//if the shader failed to sompile it should have written something to the error message
			if (errorMessage)
			{
				outputShaderErrorMessage(errorMessage, hwnd, L"defualt.ps");
			}
			else
			{
				//the was no error message so the program could not find the shader itself
				MessageBox(hwnd, L"defualt.ps", L"Missing Shader File", MB_OK);
			}
			return false;
		}

		//Create the vertex shader from the buffer
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
		if (FAILED(result))
		{
			return false;
		}

		//Create the texture vertex shader frome the buffer
		result = device->CreateVertexShader(textureVertexShaderBuffer->GetBufferPointer(), textureVertexShaderBuffer->GetBufferSize(), NULL, &m_textureVertexShader);
		if (FAILED(result))
		{
			return false;
		}

		//Create the pixel shader from the buffer
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,& m_pixelShader);
		if (FAILED(result))
		{
			return false;
		}

		//Create the texture pixel shader from the buffer
		result = device->CreatePixelShader(texturePixelShaderBuffer->GetBufferPointer(), texturePixelShaderBuffer->GetBufferSize(), NULL, &m_texturePixelShader);
		if (FAILED(result))
		{
			return false;
		}

		//Create the font pixel shader from the buffer
		result = device->CreatePixelShader(fontPixelShaderBuffer->GetBufferPointer(), fontPixelShaderBuffer->GetBufferSize(), NULL, &m_fontPixelShader);
		if (FAILED(result))
		{
			return false;
		}

		//Create the vertex input layout description
		//This setup needs to match the vertex type for the model and the vertex shader exactly
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		polygonLayout[2].SemanticName = "NORMAL";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		//get a count of the number of elemements in the layout
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		//Create the vertex input layout
		result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
			&m_layout);
		if (FAILED(result))
		{
			return false;
		}

		//Create the texture vertex input layout description
		//This setup needs to match the vertex type for the model and the texture vertex shader exactly
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		//get a count of the number of elements in the texture layout
		numElements = sizeof(texturePolygonLayout) / sizeof(texturePolygonLayout[0]);
		
		//Create the texture vertex input layout
		//result = device->CreateInputLayout(texturePolygonLayout, numElements, textureVertexShaderBuffer->GetBufferPointer(), textureVertexShaderBuffer->GetBufferSize(),
		//	&m_textureLayout);
		//if (FAILED(result))
		//{
		//	return false;
		//}

		//Release the texture vertex shader buffer and pixel shader buffer since they are no longer needed
		textureVertexShaderBuffer->Release();
		textureVertexShaderBuffer = NULL;

		texturePixelShaderBuffer->Release();
		texturePixelShaderBuffer = NULL;

		//Release the vertex shader buffer and pixel shader buffer since they are no longer needed
		vertexShaderBuffer->Release();
		vertexShaderBuffer = NULL;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = NULL;

		//Release the pixel shader for the font pixel shader since it is no longer needed
		fontPixelShaderBuffer->Release();
		fontPixelShaderBuffer = NULL;

		//Set up the description of the dynamic matrix constant buffer pointer so that
		//it is in the vertex shader
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		//Create the constant buffer so we can access the vertex shader from this class
		result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Setup the description of the camera dynamic constant buffer so that it is in the vertex shader
		cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
		cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cameraBufferDesc.MiscFlags = 0;
		cameraBufferDesc.StructureByteStride = 0;

		//create the constant camera buffer so that we can have access to it form within this class
		result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Create a texture sampler state description.
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

		//create the texture sampler state
		result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
		if (FAILED(result))
		{
			return false;
		}

		//Setup the description of the light dynamic constant buffer so that it is in the pixel shader
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBuffer);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		//create the light buffer so this class will have access to it
		result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
		if (FAILED(result))
		{
			return false;
		}
		
		//create the pixel constant buffer
		pixelColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		pixelColorBufferDesc.ByteWidth = sizeof(PixelColorBuffer);
		pixelColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		pixelColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pixelColorBufferDesc.MiscFlags = 0;
		pixelColorBufferDesc.StructureByteStride = 0;

		//create the pixel color buffer so this class will have access to it
		result = device->CreateBuffer(&pixelColorBufferDesc, NULL, &m_pixelColorBuffer);
		if (FAILED(result))
		{
			return false;
		}



		return true;
	}

	//shutdownShader-- cleans up memory for the shader
	void Shader::shutdownShader()
	{
		//release resources for then shader
		if (m_cameraBuffer)
		{
			m_cameraBuffer->Release();
			m_cameraBuffer = NULL;
		}

		if (m_layout)
		{
			m_layout->Release();
			m_layout = NULL;
		}

		if (m_lightBuffer)
		{
			m_lightBuffer->Release();
			m_lightBuffer = NULL;
		}

		if (m_matrixBuffer)
		{
			m_matrixBuffer->Release();
			m_matrixBuffer = NULL;
		}

		if (m_pixelShader)
		{
			m_pixelShader->Release();
			m_pixelShader = NULL;
		}

		if (m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = NULL;
		}

		if (m_vertexShader)
		{
			m_vertexShader->Release();
			m_vertexShader = NULL;
		}

		if (m_fontPixelShader)
		{
			m_fontPixelShader->Release();
			m_fontPixelShader = NULL;
		}

		if (m_pixelColorBuffer)
		{
			m_pixelColorBuffer->Release();
			m_pixelColorBuffer = NULL;
		}

		if (m_texturePixelShader)
		{
			m_texturePixelShader->Release();
			m_texturePixelShader = NULL;
		}

		if (m_textureVertexShader)
		{
			m_textureVertexShader->Release();
			m_textureVertexShader = NULL;
		}
	}

	//outputShaderErrorMessage-- outputs an error message to a textfile in the root directory of the .exe
	//errorMessage- the error message to output
	//hwnd- the parent window
	//shaderFilename- the name of the file to output to
	void Shader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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

	//setShaderParameters-- sets the parameters for the shader
	//deviceContext- the device context to use for drawing
	//indexCount- the number of indicies
	//worldMatrix- the world matrix to use
	//viewMatrix- the view matrix to use
	//projectionMatrix- the projection matrix to use
	//texture- the texture to use for the model
	//lightDirection- the direction of the light
	//ambientColor- the color of the ambient light
	//diffuseColor- the color of the diffuse light
	//cameraPosition- the position of the camera
	//specularColor- the color of the specular light
	//specularPower- the power(reflectiveness) of the light
	bool Shader::setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPostition, D3DXVECTOR4 specularColor, float specularPower)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBuffer* dataPtr;
		LightBuffer* dataPtr2;
		CameraBuffer* dataPtr3;
		unsigned int bufferNumber;

		//Transpose the matricies to prepare them for the shader
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

		//lock the constant buffer so that it can be written to
		result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the constant buffer
		dataPtr = (MatrixBuffer*)mappedResource.pData;

		//Copy the matrices into the constant buffer
		dataPtr->world = worldMatrix;
		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;

		//unlock the constant buffer
		deviceContext->Unmap(m_matrixBuffer, 0);

		//set the position of the constant buffer in the vertex shader
		bufferNumber = 0;
		//Now set the constant buffer in the vertex shader with the updated values
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

		//Lock the camera constant buffer so it can be written to.
		result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the constant buffer
		dataPtr3 = (CameraBuffer*)mappedResource.pData;

		//Copy the camera position into the constant buffer.
		dataPtr3->cameraPosition = cameraPostition;
		dataPtr3->padding = 0.0f;

		//Unlock the camera constant buffer
		deviceContext->Unmap(m_cameraBuffer, 0);

		//Set the position of the camera constant buffer in the vertex shader
		bufferNumber = 1;

		//Now set the camera constant buffer in the vertex shader with the updated values
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

		//Set shader texture resource in the pixel shader
		deviceContext->PSSetShaderResources(0, 1, &texture);

		//Lock the light constant buffer so it can be written 10.
		result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the constant buffer
		dataPtr2 = (LightBuffer*)mappedResource.pData;

		//Copy the lighting vars into the constant buffer
		dataPtr2->ambientColor = ambientColor;
		dataPtr2->diffuseColor = diffuseColor;
		dataPtr2->lightDirection = lightDirection;
		dataPtr2->specularColor = specularColor;
		dataPtr2->specularPower = specularPower;

		//Unlock the constant buffer
		deviceContext->Unmap(m_lightBuffer, 0);

		//Set the position of the light constant buffer in the pixel shader
		bufferNumber = 0;
		//Set the light constant buffer in the pixel shader with the updated values
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

		return true;
	}

	//renderShader-- renders the model currently in the device context
	//deviceContext- the device context to use for rendering
	//indexCount- the number of indices in the model
	void Shader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
	{
		//Set the vertex input layout
		deviceContext->IASetInputLayout(m_layout);

		//Set the vertex and pixel shaders that will be used to render this triangle.
		deviceContext->VSSetShader(m_vertexShader, NULL, 0);
		deviceContext->PSSetShader(m_pixelShader, NULL, 0);

		//Set the sampler state in the pixel shader.
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);

		//Render the model
		deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	//setShaderParameters-- sets the parameters for the texture shader
	//deviceContext- the device context to use for drawing
	//indexCount- the number of indicies
	//worldMatrix- the world matrix to use
	//viewMatrix- the view matrix to use
	//projectionMatrix- the projection matrix to use
	//texture- the texture to use for the model
	bool Shader::setTextureShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBuffer* dataPtr;
		unsigned int bufferNumber;

		//Transpose the matricies to prepare them for the shader
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

		//lock the constant buffer so that it can be written to
		result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the constant buffer
		dataPtr = (MatrixBuffer*)mappedResource.pData;

		//Copy the matrices into the constant buffer
		dataPtr->world = worldMatrix;
		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;

		//unlock the constant buffer
		deviceContext->Unmap(m_matrixBuffer, 0);

		//set the position of the constant buffer in the vertex shader
		bufferNumber = 0;
		//Now set the constant buffer in the vertex shader with the updated values
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

		//Set shader texture resource in the pixel shader
		deviceContext->PSSetShaderResources(0, 1, &texture);
		
		return true;
	}

	//setFontShaderParameters-- sets the parameters for the texture shader
	//deviceContext- the device context to use for drawing
	//indexCount- the number of indicies
	//worldMatrix- the world matrix to use
	//viewMatrix- the view matrix to use
	//projectionMatrix- the projection matrix to use
	//texture- the texture to use for the model
	//fontColor- the color to drw the font with
	bool Shader::setFontShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 fontColor)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBuffer* dataPtr;
		PixelColorBuffer* dataPtr2;
		unsigned int bufferNumber;

		//Transpose the matricies to prepare them for the shader
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

		//lock the constant buffer so that it can be written to
		result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the constant buffer
		dataPtr = (MatrixBuffer*)mappedResource.pData;

		//Copy the matrices into the constant buffer
		dataPtr->world = worldMatrix;
		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;

		//unlock the constant buffer
		deviceContext->Unmap(m_matrixBuffer, 0);

		//set the position of the constant buffer in the vertex shader
		bufferNumber = 0;
		//Now set the constant buffer in the vertex shader with the updated values
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

		//now update the color

		//lock the constant buffer so it can be written to
		result = deviceContext->Map(m_pixelColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the constant buffer
		dataPtr2 = (PixelColorBuffer*)mappedResource.pData;
		//copy the font color into the constant buffer
		dataPtr2->pixelColor = fontColor;

		//unlock the constant buffer
		deviceContext->Unmap(m_pixelColorBuffer, 0);

		//now set the position of the constant buffer in the pixel shader
		bufferNumber = 1;
		//now set the constant buffer in the pixel shader with the updated values
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

		//Set shader texture resource in the pixel shader
		deviceContext->PSSetShaderResources(0, 1, &texture);


	}

	//renderTextureShader-- renders the model currently in the device context
	//deviceContext- the device context to use for rendering
	//indexCount- the number of indices in the model
	void Shader::renderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount)
	{
		//Set the vertex input layout
		deviceContext->IASetInputLayout(m_layout);

		//Set the vertex and pixel shaders that will be used to render this triangle.
		deviceContext->VSSetShader(m_textureVertexShader, NULL, 0);
		deviceContext->PSSetShader(m_texturePixelShader, NULL, 0);

		//Set the sampler state in the pixel shader.
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);

		//Render the model
		deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	//renderFontShader-- renders the model currently in the device context
	//deviceContext- the device context to use for rendering
	//indexCount- the number of indices in the model
	void Shader::renderFontShader(ID3D11DeviceContext* deviceContext, int indexCount)
	{
		//Set the vertex input layout
		deviceContext->IASetInputLayout(m_layout);

		//Set the vertex and pixel shaders that will be used to render the triangles in the model
		deviceContext->VSSetShader(m_textureVertexShader, NULL, 0);
		deviceContext->PSSetShader(m_fontPixelShader, NULL, 0);

		//Set the sampler state in the pixel shader.
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);

		//redner the model
		deviceContext->DrawIndexed(indexCount, 0, 0);
	}

}
