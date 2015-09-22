#pragma once

namespace JR_LineModel
{
#include <D3D11.h>
#include <D3DX10math.h>
	//LineModel-- a simple class containing propertys and moethods for a model
	// that consists of lines
	class LineModel
	{

	private:
		//Vertex- defines a simple struct that contains propertys for a single vertex of the model
		struct Vertex
		{
			//position- the position of the vertex
			D3DXVECTOR3 position;
			//color- the color of the vertex
			D3DXVECTOR4 color;
		};

	public:
		//defualt constructor-- creates a new instance of LineModel with defualt parameters
		LineModel();

		//deconstructor-- destroys and cleans up ememory for the LineModel
		~LineModel();

		//init-- sets up the LineModel instance
		//model- the vertices for this model
		//color- the color for the model
		bool init(D3DXVECTOR3* model,D3DXVECTOR4 color);

		//shutDown()-- cleans up memory for the line model
		void shutDown();

		//render-- prepares the model for rendering
		//deviceContext- the deivce context to use for rednering
		void render(ID3D11DeviceContext deviceContext);

	private:

		//releaseBuffers-- releases the memory for the buffer of this model
		void releaseBuffers();

		//renderBuffers-- sets up this model instance to be rendered on the passed in
		// device context
		//deviceContext- the device context to use for rendering
		void renderBuffers(ID3D11DeviceContext deviceContext);
	private:

		ID3D11Buffer* m_indexBuffer;
		ID3D11Buffer* m_vertexBuffer;

	};
}

