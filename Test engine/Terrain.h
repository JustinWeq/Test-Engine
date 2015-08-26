#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

namespace JR_Terrain
{

	//Terrain- a class the contians methods and propertys for basic terrain
	class Terrain
	{
	private:
		//VertexType-a struct defining the vertex for the terrain
		struct VertexType
		{
			//position- the position of this vertex
			D3DXVECTOR3 position;
			//color- the color of this vertex
			D3DXVECTOR4 color;
		};

	public:
		//defualt constructor-- creates a new instance of Terrain with defualt parameters
		Terrain();

		//deconstructor-- cleans up memory for Terrain
		~Terrain();

		//init-- initalizes the Terrain class
		//device- the device to use for initializtion
		//width- the number of tiles for the terrain
		//height- the number of tiles for the height of the terrain
		bool init(ID3D11Device* device, int width, int height);

		//shutdown-- shutdown and cleans up memory for this Terrain instance
		void shutdown();

		//render-- puts the model for this terrain on the pipeline and prepares it for rendering
		//deviceContext- the device context that will be used for rendering
		void render(ID3D11DeviceContext* deviceContext);

		//getIndexCount- returns the number of indices for this Terrains model
		int getIndexCount();

	private:

		//initBuffers-- initializes the buffers for the terrain model
		//device- the device to use for intialization
		bool initBuffers(ID3D11Device* device);

		//shutdownBuffers-- shuts down the buffers for the Terrain class
		void shutdownBuffers();

		//renderBuffers-- preapres the buffers for rendering
		//deviceContext- the device context to use for rendering
		void renderBuffers(ID3D11DeviceContext* deviceContext);

	private:
		//terrainWidth- the number of tiles horizontally
		int m_terrainWidth;

		//terrainHeight- the number fo tiles vertically
		int m_terrainHeight;

		//vertexCount- the number of vertices for this terrain
		int m_vertexCount;

		//indexCount- the number of indices for this terrain
		int m_indexCount;

		//vertexbuffer- the vertices for this terrains model
		ID3D11Buffer* m_vertexBuffer;

		//indexBuffer- the index buffer for this terrains model
		ID3D11Buffer* m_indexbuffer;



	};
}