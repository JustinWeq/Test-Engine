#pragma once
#include "Font.h"
#include "Shader.h"
using namespace JR_Font;
using namespace JR_Shader;
namespace JR_Text
{
	//Text--A class with methods and propertys for displaying multiple lines of text
	class Text
	{
	private:
		//SentenceType-- a struct that contains propertys for a single sentence
		struct SentenceType
		{
			//vertexBuffer- the vertexBuffer for the sentences model
			ID3D11Buffer* vertexBuffer;
			//indexBuffer- the indexbuffer for this sentences model
			ID3D11Buffer *indexBuffer;
			//vertexCount- the number of vertices for this sentences model
			int vertexCount;
			//indexCount- the number of indices for this sentences model
			int indexCount;
			//maxLength- the maximum number of chars for this sentence
			int maxLength;
			//red- the red value for this sentences color
			float red;
			//green- the green value for this sentences color
			float green;
			//blue- the blue value for this sentences color
			float blue;
		};

		//VertexType-- a struct that contains the vertex data for this sentences model
		struct VertexType
		{
			//position- the position of this vertex
			D3DXVECTOR3 position;
			//texture- the uv coords for this vertex
			D3DXVECTOR2 texture;
		};

	public:
		//default constructor- creates a new instance of Text with defualt parameters
		Text();
		//decontructor-- deletes and cleans up the data for the Text class
		~Text();

		//init-- initializes the Text class data
		//device- the device to use for initialization
		//deviceContext- the device context to use for initialization
		//hwnd- the hwnd handle for the window
		//screenWidth- the width of the screen being rendered to
		//screenHeight- the height of the screen being rendered to
		//baseViewMatrix- the base view matrix.
		//numOfSentences- the number of sentences.
		bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
			D3DXMATRIX baseViewMatrix,int numberOfSentences);

		//shutdown-- cleans up memory for the text instance
		void shutdown();

		//render-- prepares this text for rendering
		//deviceContext- the device context to use for rendering
		//worldMatrix- the world mat
		//orthoMatrix- the ortho matrix to use for 2D rendering
		//shader- the shader to use for rendering
		bool render(ID3D11DeviceContext* deviceContext,D3DXMATRIX worldMatrix,D3DXMATRIX orthoMatrix,Shader* shader);


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
		bool setSentence(int index, char* text,int x,int y,float red,float green,float blue,ID3D11DeviceContext* deviceContext,ID3D11Device* device);
	private:
		//initSentence-- initializes the passed in sentence
		//sentence- the sentence to initialize
		//maxLength- the maximum length for the sentence
		//device- the device to use for initialization
		bool initSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);

		//updateSentence-- updates the passed in sentence
		//text- the text to update the sentence to
		//positionX- the x position of the sentence
		//positionY- the y position of the sentence
		//red- the red value of the sentence
		//blue- the blue value of the sentence
		//green the green value of the sentence
		//deviceContext- the device context to update the sentence on.
		bool updateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext);

		//releaseSentence-- releases the passed in sentence
		//sentence- the sentence to release
		void releaseSentence(SentenceType** sentence);

		//renderSentence--renders the passed in sentence
		//deviceContext- the device context to tuse for rendering
		//sentence- the sentence to render
		//worldMatrix- the world matrix to pass into the device context
		//orthoMatrix- the orth matrix to pass into the device context
		//shader- the shader to use for rendering
		bool renderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, Shader* shader);

	private:
		Font* m_Font;
		int m_screenWidth;
		int m_screenHeight;
		int m_numOfSentences;
		D3DXMATRIX m_baseViewMatrix;
		SentenceType** m_sentences;

		

	};
}