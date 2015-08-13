#include "Text.h"

namespace JR_Text
{
	//default constructor- creates a new instance of Text with defualt parameters
	Text::Text()
	{
		m_Font = NULL;
		m_screenWidth = NULL;
		m_screenHeight = NULL;
		m_sentences = NULL;
	}

	//decontructor-- deletes and cleans up the data for the Text class
	Text::~Text()
	{
		shutdown();
	}

	//init-- initializes the Text class data
	//device- the device to use for initialization
	//deviceContext- the device context to use for initialization
	//hwnd- the hwnd handle for the window
	//screenWidth- the width of the screen being rendered to
	//screenHeight- the height of the screen being rendered to
	//baseViewMatrix- the base view matrix.
	//numOfSentences- the number of sentences.
	bool Text::init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
		D3DXMATRIX baseViewMatrix, int numberOfSentences)
	{
		bool result;

		//store the screen width and height
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		//store the base view matrix
		m_baseViewMatrix = baseViewMatrix;

		//store the number of sentences
		m_numOfSentences = numberOfSentences;

		//Create and init the font object
		m_Font = new Font;
		if (!m_Font)
		{
			return false;
		}

		//init the font object
		result = m_Font->init(device, "fontdata.txt", L"font.dds");
		if (!result)
		{
			return false;
		}

		//set up the sentences
		m_sentences = new SentenceType*[numberOfSentences];
		if (!m_sentences)
		{
			return false;
		}

		//init each of the sentences
		for (int i = 0; i < m_numOfSentences; i++)
		{
			initSentence(&m_sentences[i], 256, device);
		}


	}

	//shutdown-- cleans up memory for the text instance
	void Text::shutdown()
	{
		//release the sentences
		for (int i = 0; i < m_numOfSentences; i++)
		{
			if (m_sentences[i])
			{
				releaseSentence(&m_sentences[i]);
			}

		}

		//release the font
		if (m_Font)
		{
			m_Font->shutdown();
			delete m_Font;
			m_Font = NULL;
		}

	}

	//render-- prepares this text for rendering
	//deviceContext- the device context to use for rendering
	//worldMatrix- the world mat
	//orthoMatrix- the ortho matrix to use for 2D rendering
	//shader- the shader to use for rendering
	bool Text::render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, Shader* shader)
	{
		bool result;

		//draw each of the sentences that are not blank
		for (int i = 0; i < m_numOfSentences; i++)
		{
			if (m_sentences[i])
			{
				//sentence exists so render it
				result = renderSentence(deviceContext, m_sentences[i], worldMatrix, orthoMatrix, shader);
				if (!result)
				{
					return false;
				}
			}
		}

		return true;
	}

	//setSentence- sets the sentence at the selected index.
	//index- the sentence to change
	//text- the text to update the sentence to
	//x- the x location of the sentence
	//y the y location of the sentence
	//red- the red value of the sentence
	//green- the gren value of the sentence
	//blue- the blue value of the sentence
	//deviceContext- the device context to update the sentence to
	//device- the device context to use for initialization if the sentence does not already exist
	bool Text::setSentence(int index, char* text, int x, int y, float red, float green, float blue, ID3D11DeviceContext* deviceContext, ID3D11Device* device)
	{
		bool result;
		//check to see if the selected sentence exists and if it does not initialize it
		//if (!m_sentences[index])
		//{
		//	//create new sentence
		//	m_sentences[index] = new SentenceType;
		//	if (!m_sentences[index])
		//	{
		//		return false;
		//	}

		//	//init the new sentence
		//	result = initSentence(&m_sentences[index], 256, device);
		//	if (!result)
		//	{
		//		return false;
		//	}
		//}

		//update the sentence
		result = updateSentence(m_sentences[index], text, x, y, red, green, blue, deviceContext);
		if (!result)
		{
			return false;
		}


	}

	//initSentence-- initializes the passed in sentence
	//sentence- the sentence to initialize
	//maxLength- the maximum length for the sentence
	//device- the device to use for initialization
	bool Text::initSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
	{
		VertexType*  vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		//Create a new sentence object.
		*sentence = new SentenceType;
		if (!*sentence)
		{
			return false;
		}

		//init the sentence buffers to null
		(*sentence)->vertexBuffer = 0;
		(*sentence)->indexBuffer = 0;

		//Set teh maximum length of the sentence.
		(*sentence)->maxLength = maxLength;

		//set the number of vertices in the vertex array
		(*sentence)->vertexCount = 6 * maxLength;
		//Set the number of indexes in the index array.
		(*sentence)->indexCount = (*sentence)->vertexCount;

		//Create the vertex array.
		vertices = new VertexType[(*sentence)->vertexCount];
		if (!vertices)
		{
			return false;
		}

		//Create the index array
		indices = new unsigned long[(*sentence)->vertexCount];
		if (!indices)
		{
			return false;
		}

		//init the vertex array to zeros at first.
		memset(vertices, 0, (sizeof(VertexType)*(*sentence)->vertexCount));

		//init the index array
		for (int i = 0; i < (*sentence)->indexCount; i++)
		{
			indices[i] = i;
		}

		//Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType)*(*sentence)->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		//Give the subresource structure a pointer tot he vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		//Give the sub resource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		//Create the index buffer
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		//Release the vertex array as it is no longer needed.
		delete[] vertices;
		vertices = NULL;

		//Release the index array as it is no longer needed.
		delete[] indices;
		indices = NULL;

		return true;
	}

	//updateSentence-- updates the passed in sentence
	//text- the text to update the sentence to
	//positionX- the x position of the sentence
	//positionY- the y position of the sentence
	//red- the red value of the sentence
	//blue- the blue value of the sentence
	//green the green value of the sentence
	//deviceContext- the device context to update the sentence on.
	bool Text::updateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
	{
		int numLetters;
		VertexType* vertices;
		float drawX, drawY;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType* verticesPtr;

		//Store the color of the sentence
		sentence->red = red;
		sentence->green = green;
		sentence->blue = blue;

		//Get teh number of letters in the sentence
		numLetters = (int)strlen(text);

		//Check for possible buffer overflow
		if (numLetters > sentence->maxLength)
		{
			return false;
		}

		//Create the vertex array
		vertices = new VertexType[sentence->vertexCount];
		if (!vertices)
		{
			return false;
		}

		//init the vertex array to zeros at first
		memset(vertices, 0, (sizeof(VertexType)*sentence->vertexCount));

		float length = m_Font->getStringWidth(text);
		//Calculate the X and Y pixel position on the screen to start drawing to.
		drawX = (float)(((m_screenWidth/2)*-1)+m_screenWidth - positionX-length-5);
		drawY = (float)(((m_screenHeight/2)*-1)+m_screenHeight - positionY-3);

		//Use the font class to build the vertex array from the sentence text and sentence draw location.
		m_Font->buildVertexArray((void*)vertices, text, drawX, drawY);

		//Lock the vertex buffer so it can be written to.
		result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		//Get a pointer to the data in the vertex buffer.
		verticesPtr = (VertexType*)mappedResource.pData;

		//Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));


		//Unlock the vertex buffer
		deviceContext->Unmap(sentence->vertexBuffer, 0);

		//Release the vertex array as it is no longer needed.
		delete[] vertices;
		vertices = NULL;

		return true;
	}

	//releaseSentence-- releases the passed in sentence
	//sentence- the sentence to release
	void Text::releaseSentence(SentenceType** sentence)
	{
		if (*sentence)
		{
			//Release the sentence vertex buffer.
			if ((*sentence)->vertexBuffer)
			{
				(*sentence)->vertexBuffer->Release();
				(*sentence)->vertexBuffer = NULL;
			}

			//Release teh sentence index buffer
			if ((*sentence)->indexBuffer)
			{
				(*sentence)->indexBuffer->Release();
				(*sentence)->indexBuffer = NULL;
			}

			//Release teh sentence.
			delete *sentence;
			*sentence = NULL;
		}
	}

	//renderSentence--renders the passed in sentence
	//deviceContext- the device context to tuse for rendering
	//sentence- the sentence to render
	//worldMatrix- the world matrix to pass into the device context
	//orthoMatrix- the orth matrix to pass into the device context
	//shader- the shader to use for rendering
	bool Text::renderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, Shader* shader)
	{
		unsigned int stride, offset;
		D3DXVECTOR4 pixelColor;
		bool result;

		//set the vertex buffer stride and offset
		stride = sizeof(VertexType);
		offset = 0;

		//Set the vertex buffer to active in the input assembler so it can be rendered
		deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

		//set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//set the type of primitive to render to
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//create a pixel color vector with the input sentence volor
		pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

		//set the texture


		//render the text using the shaders renderFont
		result = shader->renderFont(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix,
			m_Font->getTexture(),pixelColor);
		if (!result)
		{
			return false;
		}

		return true;
	}
}