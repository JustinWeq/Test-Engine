//pre processors
#pragma once
//includes
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

namespace JR_Shader
{
	//Shader-- a class that contains properties and methods for 
	//a basic shader
	class Shader
	{
	private:
		//MatrixBuffer--A buffer that contains propertys for the world,
		// view and projection matrices to be passed into the shader
		struct MatrixBuffer
		{
			//world-- the world matrix
			D3DXMATRIX world;
			//view-- the view matrix
			D3DXMATRIX view;
			//projection-- the projection matrix
			D3DXMATRIX projection;
		};

		//CameraBuffer--Contains properties for the camera information
		// this is used mostly for lighting
		struct CameraBuffer
		{
			//cameraPosition--The x,y and z position of the camera
			D3DXVECTOR3 cameraPosition;
			//padding-- padding for the camera
			float padding;
		};

		//LightBuffer-- Contains information for the light to be
		//passed into the shader
		struct LightBuffer
		{
			//ambientColor--The color of the ambient light
			D3DXVECTOR4 ambientColor;
			//diffuseColor--The color of the diffusal light,
			//this is shown on faces facing towards the light
			D3DXVECTOR4 diffuseColor;
			//lightDirection--The direction the light is facing
			D3DXVECTOR3 lightDirection;
			//specularPower-- the relfective power to be passed into the shader
			float specularPower;
			//specularColor-- The color of the reflected light;
			D3DXVECTOR4 specularColor;
		};

		//PixelColorBuffer-- contains information for the pixelColor to be passed into the
		//pixel shader
		struct PixelColorBuffer
		{
			//pixelColor-- the color of the pixel to be multiplied with.
			D3DXVECTOR4 pixelColor;
		};

	public:
		//defualt constructor-- creates a new Shader object with defualt parameters
		Shader();
	    //deconstructor-- cleans up dynamic memory before the object is destroyed
		~Shader();

		//init-- initializes the shader
		//device- the ID3D11Deivce to use
		//hwnd- the hwnd(parent window)
		bool init(ID3D11Device* device, HWND hwnd);

		//shutdown-- Cleans up memory for the shader
		void shutdown();

		//render-- renders using the shader and the passed in parameters
		//deviceContext- the device context to use for drawing
		//indexCount- the number of indicies
		//worldMatrix- the world matrix to use
		//viewMatrix- the view matrix to use
		//projectionMatrix- the projection matrix to use
		//texture- the texture to use for the model
		//lightDirection- the direction of the light
		//ambientColor- the color of the ambient light
		//diffuseColor- the color of the diffuse light
		//cameraPosition- the position of the camera
		//specularColor- the color of the specular light
		//specularPower- the power(reflectiveness) of the light
		bool render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
			D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
			D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

		//renderTexture-- renders using the shader and the passed in parameters
		//deviceContext- the device context to use for drawing
		//indexCount- the number of indicies
		//worldMatrix- the world matrix to use
		//viewMatrix- the view matrix to use
		//projectionMatrix- the projection matrix to use
		//texture- the texture to use for the model
		bool renderTexture(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
			D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

	private:
		//initShader-- sets up the shader
		//device- The device to use to create the shader
		//hwnd- the parent window
		//vsFilename- the name of the vertex shader
		//psFilename- the name of the pixel shader
		bool initShader(ID3D11Device* device, HWND hwnd);

		//shutdownShader-- cleans up memory for the shader
		void shutdownShader();

		//outputShaderErrorMessage-- outputs an error message to a textfile in the root directory of the .exe
		//errorMessage- the error message to output
		//hwnd- the parent window
		//shaderFilename- the name of the file to output to
		void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

		//setShaderParameters-- sets the parameters for the shader
		//deviceContext- the device context to use for drawing
		//indexCount- the number of indicies
		//worldMatrix- the world matrix to use
		//viewMatrix- the view matrix to use
		//projectionMatrix- the projection matrix to use
		//texture- the texture to use for the model
		//lightDirection- the direction of the light
		//ambientColor- the color of the ambient light
		//diffuseColor- the color of the diffuse light
		//cameraPosition- the position of the camera
		//specularColor- the color of the specular light
		//specularPower- the power(reflectiveness) of the light
		bool setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
			D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection,
			D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPostition, D3DXVECTOR4 specularColor, float specularPower);
		
		//setTextureShaderParameters-- sets the parameters for the texture shader
		//deviceContext- the device context to use for drawing
		//indexCount- the number of indicies
		//worldMatrix- the world matrix to use
		//viewMatrix- the view matrix to use
		//projectionMatrix- the projection matrix to use
		//texture- the texture to use for the model
		bool setTextureShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
			D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

		//setFontShaderParameters-- sets the parameters for the texture shader
		//deviceContext- the device context to use for drawing
		//indexCount- the number of indicies
		//worldMatrix- the world matrix to use
		//viewMatrix- the view matrix to use
		//projectionMatrix- the projection matrix to use
		//texture- the texture to use for the model
		//fontColor- the color to drw the font with
		bool setFontShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
			D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,D3DXVECTOR4 fontColor);

		//renderShader-- renders the model currently in the device context
		//deviceContext- the device context to use for rendering
		//indexCount- the number of indices in the model
		void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);

		//renderTextureShader-- renders the model currently in the device context
		//deviceContext- the device context to use for rendering
		//indexCount- the number of indices in the model
		void renderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount);

		//renderFontShader-- renders the model currently in the device context
		//deviceContext- the device context to use for rendering
		//indexCount- the number of indices in the model
		void renderFontShader(ID3D11DeviceContext* deviceContext, int indexCount);


		private:
			//vertexShader-- the vertex shader
			ID3D11VertexShader* m_vertexShader;
			//pixelShader-- the pixel shader
			ID3D11PixelShader* m_pixelShader;
			//textureVertexShader-- the texture vertex shader
			ID3D11VertexShader* m_textureVertexShader;
			//texturePixelShader-- the texture pixel shader
			ID3D11PixelShader* m_texturePixelShader;
			//fontPixelShader-- the pixel shader for the font
			ID3D11PixelShader* m_fontPixelShader;
			//layout-- the layout of the shader
			ID3D11InputLayout* m_layout;
			//matrixBuffer-- the buffer for the matricies
			ID3D11Buffer* m_matrixBuffer;
			//sampleState-- the state of the sampler
			ID3D11SamplerState* m_sampleState;
			//cameraBuffer-- the buffer for the camera
			ID3D11Buffer* m_cameraBuffer;
			//lightBuffer-- the buffer for the light
			ID3D11Buffer* m_lightBuffer;
			//pixelColorBuffer-- the buffer for the pixel color
			ID3D11Buffer* m_pixelColorBuffer;
	};
}