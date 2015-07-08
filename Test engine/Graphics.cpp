#include "Graphics.h"
namespace JR_Graphics
{
	Graphics::Graphics()
	{
		m_depthStencil = NULL;
		m_depthStencilBuffer = NULL;
		m_depthStencilView = NULL;
		m_device = NULL;
		m_deviceContext = NULL;
		m_rasterState = NULL;
		m_renderTarget = NULL;
		m_swapChain = NULL;
		m_vsync = false;
		m_videoCardMemory = 0;
	}

	Graphics::~Graphics()
	{
		close();
	}

	//init-- inits and set ups the graphics object to get it ready for drawing
	//screenWidth-- the width of the screen the graphics device is drawing to
	//screenHeight-- the height of the screen this graphics device is drawing to
	//vsync-- a boolean that determines whether vsync is enabled or not
	//hwnd-- the hwnd of the parent window
	//fullscreen-- a boolean that determines whether the window is fullscreen or not
	//screenDepth-- the depth of the screen for FOV
	//screenNear-- the near plane of the screen for FOV
	//returns false if something went wrong, true otherwise
	bool Graphics::init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
		float screenDepth, float screenNear)
	{
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator, stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;
		float fieldOfView, screenAspect;

		//store the setting for vsync
		m_vsync = vsync;

		//create the graphics device factory
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
			return false;

		//use the factor to create the adapter for the primary graphics interface(video card)
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
			return false;

		//enumerate the primary adapter output(monitor)
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
			return false;

		//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
			return false;

		//create a list to hold all of the different disply modes
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
			return false;

		//now fill the list of display modes
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
			return false;

		//do a linear search and find the display mode that matches the screen height and width
		for(int i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)screenWidth &&
				displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}

		//get the adapter(video card) description
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
			return false;


		//store the dedicated video card memory in mega bytes
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		//convert the name of the video card to a character array and store it.
		error = wcstombs_s((size_t*)(&stringLength), m_videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		//Release the display mode list.
		delete[] displayModeList;
		displayModeList = NULL;

		//release the adpater output.
		adapterOutput->Release();
		adapterOutput = NULL;

		//release the adapter
		adapter->Release();
		adapter = NULL;

		//release the factory
		factory->Release();
		factory = 0;

		//zero out the swap chain description
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		//Set ot to a single back buffer
		swapChainDesc.BufferCount = 1;

		//set the width and height of the back buffer
		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		//set regular 32 bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (m_vsync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		//set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		//set the handle for the window to render to.
		swapChainDesc.OutputWindow = hwnd;

		//turn multi sampling off
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		//Set to fullscreen if fullscreen windowed otherwise
		if (fullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		//set the scan line odering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//discard the back buffer contents after presenting
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//donw set the advanced flags
		swapChainDesc.Flags = 0;

		//set the feature level to directX 11
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		//create the swap chain,Direct3D, and the driect3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
		if (FAILED(result))
			return false;

		//Get the pointer to the back buffer
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
			return false;

		//create the render target view with the back buffer pointer
		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTarget);
		if (FAILED(result))
			return false;

		//releas the pointer to the back buffer
		backBufferPtr->Release();
		backBufferPtr = 0;

		//init the description of the depth buffer
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		//set up the description of the depth buffer
		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		//Create the texture for the depth buffer using the filled out description
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(result))
			return false;

		//init the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		//Set up the description of the stencil state
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		//Stencil operations of pixel is front facing
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//Stencil operations of i=pixel is back facing
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//create the depth stencil state
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencil);
		if (FAILED(result))
			return false;

		//set the depth stencil state
		m_deviceContext->OMSetDepthStencilState(m_depthStencil, 1);

		//init the depth stencil view
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		//Set up the depth stencil view description
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil view
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc,&m_depthStencilView);
		if (FAILED(result))
			return false;

		//Bind the render target view and depth stencil buffer to the output render pipeline
		m_deviceContext->OMSetRenderTargets(1, &m_renderTarget, m_depthStencilView);

		//Setup the raster description which determines how and what polygons get drawn
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		//create the rasterizer state from the description
		result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
		if (FAILED(result))
			return false;

		// Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState);

		//Setup the viewport for rendering
		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the view por
		m_deviceContext->RSSetViewports(1, &viewport);

		//set up the projection matrix
		fieldOfView = (float)D3DX_PI / 4.0f;
		screenAspect = (float)screenWidth / (float)screenHeight;

		//create the projection matrix for 3d rendering
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

		//init the world matrix to the indentity matrix
		D3DXMatrixIdentity(&m_worldMatrix);

		//Create an orth projection matrix for 2D rendering
		D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

		return true;
	}

	//releases all hel memory
	void Graphics::close()
	{
		//check to make sure each object is not null before releasing
		if (m_swapChain)
		{
			m_swapChain->SetFullscreenState(false, NULL);
		}

		if (m_rasterState)
		{
			m_rasterState->Release();
			m_rasterState = NULL;
		}

		if (m_depthStencilView)
		{
			m_depthStencilView->Release();
			m_depthStencilView = NULL;
		}

		if (m_depthStencil)
		{
			m_depthStencil->Release();
			m_depthStencil = NULL;
		}

		if (m_depthStencilBuffer)
		{
			m_depthStencilBuffer->Release();
			m_depthStencilBuffer = NULL;
		}

		if (m_renderTarget)
		{
			m_renderTarget->Release();
			m_deviceContext = NULL;
		}

		if (m_deviceContext)
		{
			m_deviceContext->Release();
			m_deviceContext = NULL;
		}

		if (m_device)
		{
			m_device->Release();
			m_device = NULL;
		}
	}

	//beginDrawing-- clears the screen with the passed in color
	//red-- the red value of the color
	//green-- the green value of the color
	//blue-- the blue value of the color
	//alpha-- tha alpha value of the color
	void Graphics::beginDrawing(float red, float green, float blue, float alpha)
	{
		float color[4];

		//set up the color to clear the buffer to
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		//clear the back buffer
		m_deviceContext->ClearRenderTargetView(m_renderTarget, color);

		//clear the depth buffer
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f,0);
	}

	//endDrawing-- swaps the back buffer to the front buffer on the swapchain for 
	//double buffering
	void Graphics::endDrawing()
	{
		//present the back buffer to the screen now that rendering is complete
		m_swapChain->Present(m_vsync, 0);
	}

	//getDevice-- returns the device for this graphics object
	ID3D11Device* Graphics::getDevice()
	{
		return m_device;
	}

	//getDeviceContext-- returns the deivce context for this
	//graphics object
	ID3D11DeviceContext* Graphics::getDeviceContext()
	{
		return m_deviceContext;
	}

	//getProjectionMatrix--returns the projection matrix for this graphics object
	//projection-- the projection matrix to assign to
	void Graphics::getProjectionMatrix(D3DXMATRIX& projection)
	{
		projection = m_projectionMatrix;
	}

	//getWorldMatrix--returns the world matrix(identity of this graphics object)
	//world--the world matrix to assign to
	void Graphics::getWorldMatrix(D3DXMATRIX& world)
	{
		world = m_worldMatrix;
	}

	//getOrthoMatrix-- returns the ortho matrix of this graphics object
	//ortho--the ortho matrix to assign to
	void Graphics::getOrthoMatrix(D3DXMATRIX& ortho)
	{
		ortho = m_orthoMatrix;

	}

	//getVideoCardInfo-- reutrns information on the video card
	//description--the description of the video card to assign to
	//vram-- the amount of vram to assign to
	void Graphics::GetVideoCardInfo(char* description, int& vram)
	{
		strcpy_s(description, 128, m_videoCardDescription);
		vram = m_videoCardMemory;
	}
}