#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <stdio.h>
#include "Texture.h"

using namespace JR_Texture;
using namespace std;

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
			//texture- the texture coords
			D3DXVECTOR2 texture;
			//normal- normal of this vertex
			D3DXVECTOR3 normal;
		};

		//HeightMapType- a struct that contains information for a heghtmap
		struct HeightMapType
		{
			//x- the x coord
			float x;
			//y- the y coord
			float y;
			//z- the z coord
			float z;
			//tu- the textures u coord
			float tu;
			//tv the texture v coord
			float tv;
			//nx- the x normal 
			float nx;
			//ny- the y normal
			float ny;
			//nz- the z normal
			float nz;
		};

		//Vector-- a simple struct defining a simple 3D vector
		struct Vector
		{
			//x- the x coord 
			float x;
			//y- the y coord
			float y;
			//z- the z coord
			float z;
		};

	public:
		//defualt constructor-- creates a new instance of Terrain with defualt parameters
		Terrain();

		//deconstructor-- cleans up memory for Terrain
		~Terrain();

		//init-- initalizes the Terrain class
		//device- the device to use for initializtion
		//mapAddress- the address of the heightmap
		//textureAddress- the texture address
		bool init(ID3D11Device* device,char* mapAddress,WCHAR* textureAddress);

		//shutdown-- shutdown and cleans up memory for this Terrain instance
		void shutdown();

		//setTextureRepeat- sets the amount of times the texture should repeat itself
		//repeat- the amount of times to repeat
		void setTextureRepeat(int repeat);

		//getTexture-- returns the texture for this terrain
		ID3D11ShaderResourceView* getTexture();

		//getVertexCount-- returns the number of vertices for this terrain
		int getVertexCount();
		
		//copyVertexArray- copys the vertex data over to the passed in array
		//vertexList- the list to copy the array to
		void copyVertexArray(void* vertexList);

	private:

		//initBuffers-- initializes the buffers for the terrain model
		//device- the device to use for intialization
		bool initBuffers(ID3D11Device* device);

		//calculateNormals- calculates the normals for the terrain
		bool calculateNormals();
		//shutdownBuffers-- shuts down the buffers for the Terrain class
		void shutdownBuffers();

		//loadHeightMap-- loads the height map using the passed in address
		bool loadHeightMap(char* mapAddress);

		//normalizeHeightMap-- normalizes the heightmap
		void normalizeHeightMap();

		//shutdownHeightMap()-- cleans up memory for the heightmap
		void shutdownHeightMap();

		//calculateTextureCoords-- calculates the texture coordinates for this terrain
		void calculateTextureCoords();

		//loadTexture-- loads the texture for this terrain
		//device- the device to load the texture with
		//textureAddress- the address of the texture to use
		bool loadTexture(ID3D11Device* device, WCHAR* textureAddress);
		
		//releaseTexture-- cleans up memory for the texture
		void releaseTexture();

	private:
		//terrainWidth- the number of tiles horizontally
		int m_terrainWidth;

		//terrainHeight- the number fo tiles vertically
		int m_terrainHeight;

		//vertexCount- the number of vertices for this terrain
		int m_vertexCount;

		//heightMap- the array containing information for a height map
		HeightMapType* m_heightMap;

		//textureRepeat- the amount of times the texture repeats itself
		int m_textureRepeat;

		//texture- the texture for the terrain
		Texture* m_texture;

		//vertices- the vertices in the terrain
		VertexType* m_vertices;

	};
}