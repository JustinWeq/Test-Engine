#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

namespace JR_FBXModel
{
	class FBXModel
	{
	public:
		FBXModel();
		~FBXModel();

		//init-- sets up the model
		//device- the device to use for loading
		//modelName- the name of the model to use
		bool init(ID3D11Device device,char* modelName);

		//render-- prepares the vertex and index buffers on the device context
		//deviceContext- the device context to use for rendering
		void render(ID3D11DeviceContext deviceContext);

	private:

		//loadModel-- loads and sets up the model
		//device- the deivce to use for loading the model
		//modelName- the name of the model to load
		bool loadModel(ID3D11Device device,char* modelName);

		//initBuffers-- initializes the vertex and index buffers for the model
		//device- the device to use for initialization
		bool initBuffers(ID3D11Device device);


	private:
		//vertexBuffer-- the vertex buffer for the model
		ID3D11Buffer* m_vertexBuffer;
		//indexBuffer-- the index buffer for the model
		ID3D11Buffer* m_indexBuffer;
		//numOfVertices-- the number of vertices
		unsigned int m_numOfVerticies;
		//numOfIndicies-- the number of indicies
		unsigned int m_numOfIndicies;

	};
}
