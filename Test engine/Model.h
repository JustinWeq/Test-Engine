#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;
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

		bool init();
		void shutdown();
		void render();

		int getIndexCount();
		ID3D11ShaderResourceView* getTexture();
	private:

		bool initBuffers();
		void shutdownBuffers();
		void renderBuffers();

		bool loadTexture();
		void releaseTexture();

		bool loadModel();
		void releaseModel();


	private:
		ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
		int m_vertexCount, m_indexCount;
		//Texture* m_texture;
		ModelType m_model;
	};
}