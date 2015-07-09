#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include "Texture.h"
using namespace std;
using namespace JR_Texture;
namespace JR_Model
{
	//Model-- a class that contains the vertices and other information for a model
	class Model
	{
	private:
		//Vertex-- contains the information for a single vertex
		struct Vertex {
			//Vertex--The position of the vertex(x,y,z)
			D3DXVECTOR3 position;
			//Texture--The tex coords(U,V)
			D3DXVECTOR2 texture;
			//Normal--the normal of this vertex
			D3DXVECTOR3 normal;
		};

		//ModelType--The model type information
		struct ModelType
		{
			//x-- The x coord
			float x,
				//y-- the y coord
				y,
				//z-- the z coord
				z;
			//tu-- the textures u coord
			float tu,
				//tv-- the textures v coord
				tv;
			//nx-- the normal x coord
			float nx,
				//ny-- the normals y coord
				ny,
				//nz-- the normals z coord
				nz;
		};
	public:
		//defualt constructor--creates a new Model instance with defualt parameters
		Model();
		//deconstructor--  cleans up dynamic memory of the model object
		~Model();

		//init-- initializes the model
		//device- the device to use for initialization
		//modelFileName- the name of the model to load
		//textureFilename-- the name of the texture to load
		bool init(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename);
		//shutdown-- releases all dynamic memory
		void shutdown();
		//render-- renders the model
		//deviceContext- the device context to use for rendering
		void render(ID3D11DeviceContext* deviceContext);

		//getIndexCount-- returns the number of indices for this model
		int getIndexCount();
		//getTexture-- returns the texture for this model
		ID3D11ShaderResourceView* getTexture();
	private:

		//initBuffers-- initializes the vertex and index buffer for this model
		//device- the device to use for initialization
		bool initBuffers(ID3D11Device* device);
		//shutdownBuffers-- cleans up memory for the buffers
		void shutdownBuffers();
		//renderBuffers-- renders the buffers
		//deviceContext- the device context to put the index and vertex buffers on
		void renderBuffers(ID3D11DeviceContext* deviceContext);

		//loadTexture-- loads the texture for this model
		//device- the device to use for loading
		//filename- the name of the file the texture is in
		bool loadTexture(ID3D11Device* device, WCHAR* filename);
		//releaseTexture-- cleans up memory for the texture
		void releaseTexture();

		//loadModel-- loads the model
		//filename- the name of the file to load the model from
		bool loadModel(char* filename);
		//releaseModel-- releases the memory for the model
		void releaseModel();


	private:
		//vertexBuffer-- the vertex buffer
		ID3D11Buffer * m_vertexBuffer,
			//indexBuffer- the index buffer
			*m_indexBuffer;
		//vertexCount-- the number of verticies
		int m_vertexCount,
			//indexCount-- the number of indices
			m_indexCount;
		//texture-- the texture for this model
		Texture* m_texture;
		//model-- the model information
		ModelType* m_model;
	};
}