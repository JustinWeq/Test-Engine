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
		m_depthStencilDiabledState = 0;
		m_alphaBlendingStateDisabled = NULL;
		m_alphaBlendingStateEnabled = NULL;
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
		D3D11_DEPTH_STENCIL_DESC depthDisabledDesc;
		D3D11_BLEND_DESC  blendStateDescription;


		// Store the vsync setting.
		m_vsync = vsync;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i<numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)screenWidth)
			{
				if (displayModeList[i].Height == (unsigned int)screenHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		/*error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}*/

		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
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

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (fullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,NULL , &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
		if (FAILED(result))
		{
			return false;
		}

		// Get the pointer to the back buffer.
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTarget);
		if (FAILED(result))
		{
			return false;
		}

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
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

		// Create the texture for the depth buffer using the filled out description.
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencil);
		if (FAILED(result))
		{
			return false;
		}

		// Set the depth stencil state.
		m_deviceContext->OMSetDepthStencilState(m_depthStencil, 1);

		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_deviceContext->OMSetRenderTargets(1, &m_renderTarget, m_depthStencilView);

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState);

		// Setup the viewport for rendering.
		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		m_deviceContext->RSSetViewports(1, &viewport);

		// Setup the projection matrix.
		fieldOfView = (float)D3DX_PI / 4.0f;
		screenAspect = (float)screenWidth / (float)screenHeight;

		// Create the projection matrix for 3D rendering.
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

		// Initialize the world matrix to the identity matrix.
		D3DXMatrixIdentity(&m_worldMatrix);

		// Create an orthographic projection matrix for 2D rendering.
		D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

		//Clear the second depth stencil state before setting the parameters.
		ZeroMemory(&depthDisabledDesc, sizeof(depthDisabledDesc));

		//Now create a second depth stencil state which turns off the z buffer for 2D rendering.
		depthDisabledDesc.DepthEnable = false;
		depthDisabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//create the state using the device
		result = m_device->CreateDepthStencilState(&depthDisabledDesc, &m_depthStencilDiabledState);
		if (FAILED(result))
		{
			return false;
		}

		//Clear the blend state desription
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		//Create an a;pha eneabled blend state description
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		//Create the blend state using the description
		result = m_device->CreateBlendState(&blendStateDescription, &m_alphaBlendingStateEnabled);
		if (FAILED(result))
		{
			return false;
		}

		//Modify the description to create an alpha disabled blend state description
		blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

		//Create the blend state using the description
		result = m_device->CreateBlendState(&blendStateDescription, &m_alphaBlendingStateDisabled);
		if (FAILED(result))
		{
			return false;
		}

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

		if (m_depthStencilDiabledState)
		{
			m_depthStencilDiabledState->Release();
			m_depthStencilDiabledState = NULL;
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

		if (m_alphaBlendingStateDisabled)
		{
			m_alphaBlendingStateDisabled->Release();
			m_alphaBlendingStateDisabled = NULL;
		}

		if (m_alphaBlendingStateEnabled)
		{
			m_alphaBlendingStateEnabled->Release();
			m_alphaBlendingStateEnabled = NULL;
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

	//zBufferState-- turns the z buffer on or off
	//state- the state to turn the zbuffer to
	void Graphics::zBufferState(bool state)
	{
		switch(state)
		{
		case true: 
		{
			m_deviceContext->OMSetDepthStencilState(m_depthStencil, 1);
			break;
		}
		case false:
		{
			m_deviceContext->OMSetDepthStencilState(m_depthStencilDiabledState, 1);
			break;
		}
		}
	}

	//alphaBlendingState-- turns alpha blending on or off
	//state- the state to turn alpha blending to
	void Graphics::alphaBlendingState(bool state)
	{
		switch (state)
		{
		case true:
		{
			float blendFactor[4];

			//Setup the blend factor
			blendFactor[0] = 0.0f;
			blendFactor[1] = 0.0f;
			blendFactor[2] = 0.0f;
			blendFactor[3] = 0.0f;

			//Turn on alpha blending
			m_deviceContext->OMSetBlendState(m_alphaBlendingStateEnabled, blendFactor, 0xffffffff);
			break;
		}
		case false:
		{

			float blendFactor[4];

			//Setup the blend factor
			blendFactor[0] = 0.0f;
			blendFactor[1] = 0.0f;
			blendFactor[2] = 0.0f;
			blendFactor[3] = 0.0f;

			//Turn on alpha blending
			m_deviceContext->OMSetBlendState(m_alphaBlendingStateDisabled, blendFactor, 0xffffffff);
			break;
		}

		}
	}
}