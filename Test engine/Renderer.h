#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <vector>
#include <fstream>
#include "Texture.h"
namespace JR_Renderer
{
	//InstanceVertex1- the first part of the vertex
	struct InstanceVertex1
	{
		//position- the position of the vertex
		D3DXVECTOR3 position;
		//texture- the UV coords of this vertex
		D3DXVECTOR2 texture;
	};
	//InstancedVertex2- a vertex containg date for instanced drawing
	struct InstanceVertex2 {
		//textureID- the ID for the texture, remmeber there can be no more then 30 at once.
		// set this to > 0 to not draw with a texture
		int textureID;
		//color- the color of the model
		D3DXVECTOR4 color;
		//UVAdd- the value to add to the UV coords for instanced animations
		D3DXVECTOR2 UVAdd;
		//UVMultiply- the value that is mutiplied if the coord is not equal to 0
		D3DXVECTOR2 UVMultiply;
		//matrixInstance- the WVP matrix for the entire model
		D3DXMATRIX matrixInstance;


	};
	using namespace JR_Texture;
	using namespace std;
	//Renderer- a simple class that contains methods and propertys for 2D rendering using batching.
	class Renderer
	{
	public:
		//defualt constructor-- creates a new instance of Rednerer with defualt parameters
		Renderer();

		//deconstructor- cleans up dynamic memory for
		~Renderer();

		//init-- initializes the Renderer
		//device- the device to use for intitialization
		//screenWidth- the width of the screen
		//screenHeight- the height of the screen
		//hwnd- the handle to the window, used for outputting debugging information
		bool init(ID3D11Device* device,int screenWidth,int screenHeight,HWND hwnd);

		//draw-- adds the passed in vertices to the list to be drawn
		//matrix- the matrix to be used for drawing, this is calculated aginst the ortho and view matricies
		//textureID- the id of the texture to draw,leave this -1 to not use a texture
		//color- the color to draw it
		//UVAdd- the amount to add to the 0 coords
		//UVMultiply- the amout to multiply the one coords with
		bool draw(D3DXMATRIX matrix, int textureID, D3DXVECTOR4 color,D3DXVECTOR2 UVAdd,D3DXVECTOR2 UVMultiply);

		//deviceContext- the device context to use for drawing
		//deviceContext
		bool presentDraw(ID3D11DeviceContext* deviceContext);

		//addTexture
		//texture- the texture to add, remember no more then 30 textures can be loaded at once
		bool addTexture(Texture texture);
		
		//shutdown-- shutsdown and deletes dynamic memory for the renderer
		void shutdown();
		
	private:

		//initBuffers-- initializes the buffers
		//device- the device to use for initialization
		bool initBuffers(ID3D11Device* device);

		//initShaders-- initializes the shaders
		//device- the device to use for initialization
		//hwnd- the handle to the window, used for debugging messages
		bool initShaders(ID3D11Device* device,HWND hwnd);

		//shutdownBuffers-- shutsdown and deletes the buffers for the renderer
		void shutdownBuffers();

		//shutdownShaders-- shutdown the shaders
		void shutdownShaders();

		//updateBuffers-- updates the instance buffer with new information
		//deviceContext- the device context to update the buffers to
		bool updateBuffers(ID3D11DeviceContext* deviceContext);

		//drawBuffers-- renders the buffers 
		//deviceContext- the device context to use for rendering
		bool drawBuffers(ID3D11DeviceContext* deviceContext);
		
		//createMatricies
		void createMatricies(int screenWidth, int screenHeight);

		//outputShaderErrorMessage-- outputs an error message to a textfile in the root directory of the .exe
		//errorMessage- the error message to output
		//hwnd- the parent window
		//shaderFilename- the name of the file to output to
		void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	private:
		//MAX_TEXTURES- the maximum number of textures for the renderer to contain
		const int MAX_TEXTURES = 30;
		//MAX_INSTANCES- the maximum amount of instances to draw at one time
		const int MAX_INSTANCES = 10000;
		//vertexBuffer- the vertex buffer
		ID3D11Buffer *m_vertexBuffer;
		//instanceBuffer- the instance buffer
		ID3D11Buffer *m_instanceBuffer;
		//indexbuffer- the indexBuffer
		ID3D11Buffer *m_indexBuffer;
		//vertexShader- the vertexShader
		ID3D11VertexShader* m_vertexShader;
		//pixelShader- the pixel shader
		ID3D11PixelShader* m_pixelShader;
		//layout- the layout of the vertex for the shader
		ID3D11InputLayout* m_layout;
		//vertices-the list of vertices for the instance
		vector<InstanceVertex2> m_vertices;
		//textures-the list of texture for the instsance
		vector<ID3D11ShaderResourceView*> m_textures;
		//sampler- the sampler state for the shader resources
		ID3D11SamplerState* m_sampler;
		//view- the view matrix
		D3DXMATRIX m_view;
		//ortho- the ortho matrix
		D3DXMATRIX m_ortho;
		//numOfInstances- the number of instances to draw
		int m_numOfInstances;
	};
}