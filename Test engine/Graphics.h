#pragma once

//linkers
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dx10.lib")

//includes
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <D3DX10math.h>


namespace JR_Graphics
{
	//Graphics-- a class containing methods and propertys for 3D drawing
	class Graphics
	{
	public:
		//defualt constructor-- constructs a new instance of Graphics with defualt
		//parameters
		Graphics();
		//deconstructor-- deletes pointers and clears up memory
		~Graphics();

		//init-- inits and set ups the graphics object to get it ready for drawing
		//screenWidth-- the width of the screen the graphics device is drawing to
		//screenHeight-- the height of the screen this graphics device is drawing to
		//vsync-- a boolean that determines whether vsync is enabled or not
		//hwnd-- the hwnd of the parent window
		//fullscreen-- a boolean that determines whether the window is fullscreen or not
		//screenDepth-- the depth of the screen for FOV
		//screenNear-- the near plane of the screen for FOV
		bool init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
			float screenDepth, float screenNear);

		//beginDrawing-- clears the screen with the passed in color
		//red-- the red value of the color
		//green-- the green value of the color
		//blue-- the blue value of the color
		//alpha-- tha alpha value of the color
		void beginDrawing(float red, float green, float blue, float alpha);

		//endDrawing-- swaps the back buffer to the front buffer on the swapchain for 
		//double buffering
		void endDrawing();

		//close-- cleans up and releases memory
		//called by the deconstructor
		void close();

		//getDevice-- returns the device for this graphics object
		ID3D11Device* getDevice();

		//getDeviceContext-- returns the deivce context for this
		//graphics object
		ID3D11DeviceContext* getDeviceContext();

		//getProjectionMatrix--returns the projection matrix for this graphics object
		//projection-- the projection matrix to assign to
		void getProjectionMatrix(D3DXMATRIX& projection);
		//getWorldMatrix--returns the world matrix(identity of this graphics object)
		//world--the world matrix to assign to
		void getWorldMatrix(D3DXMATRIX& world);
		//getOrthoMatrix-- returns the ortho matrix of this graphics object
		//ortho--the ortho matrix to assign to
		void getOrthoMatrix(D3DXMATRIX& ortho);

		//getVideoCardInfo-- reutrns information on the video card
		//description--the description of the video card to assign to
		//vram-- the amount of vram to assign to
		void GetVideoCardInfo(char* description, int& vram);

		//zBufferState-- turns the z buffer on or off
		//state- the state to turn the zbuffer to
		void zBufferState(bool state);

		//alphaBlendingState-- turns alpha blending on or off
		//state- the state to turn alpha blending to
		void alphaBlendingState(bool state);

	private:
		//vsync-- vsync
		bool m_vsync;
		//videoCardMemory--the memory of the video card
		int m_videoCardMemory;
		//videoCarDescription--the description of the video card
		char m_videoCardDescription[128];
		//the graphics swap chain
		IDXGISwapChain* m_swapChain;
		//the graphics device
		ID3D11Device* m_device;
		//the graphics device context
		ID3D11DeviceContext* m_deviceContext;
		//the graphics render target
		ID3D11RenderTargetView* m_renderTarget;
		//the graphics depth stencil buffer
		ID3D11Texture2D* m_depthStencilBuffer;
		//the graphics actual depth stencil
		ID3D11DepthStencilState* m_depthStencil;
		//the depth stencil view
		ID3D11DepthStencilView* m_depthStencilView;
		//the graphics rasterizer state object
		ID3D11RasterizerState* m_rasterState;
		//the projection matrix for the graphics object
		D3DXMATRIX m_projectionMatrix;
		//the world matrix(identity) of the graphics object
		D3DXMATRIX m_worldMatrix;
		//the orth matrix of the graphics object, for use in 2D drawing
		D3DXMATRIX m_orthoMatrix;
		//a depth stencil state with the z buffer disabled.
		ID3D11DepthStencilState* m_depthStencilDiabledState;
	};
}