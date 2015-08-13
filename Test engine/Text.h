#pragma once
#include "Font.h"
using namespace JR_Font;
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
		bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
			D3DXMATRIX baseViewMatrix);

	};
}