////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	m_Cube = 0;
	m_TextureShader = 0;
	
	// Post Processing
	m_BasicShader = 0;
	m_ConvolutionShader = 0;

	m_SmallWindow = 0;
	m_FullScreenWindow = 0;

	m_RenderTexture = 0;
	m_DownSampleTexture = 0;
	m_ConvolutionTexture = 0;
	m_UpSampleTexture = 0;

	// Sky Dome
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	// m_NoiseTexture = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	// Set the size to sample down to.
	int downSampleWidth = screenWidth / 2;
	int downSampleHeight = screenHeight / 2;

	// Create the input object. The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 50.0f;
	cameraY = 2.0f;
	cameraZ = -7.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->InitializeTerrain(m_Direct3D->GetDevice(), 128,128, L"../Engine/data/SandTexture.png", L"../Engine/data/SlopeTexture.png");   //initialise the flat terrain.
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 0.0f);

	// Create the cube model
	m_Cube = new ModelClass;
	if (!m_Cube)
	{
		return false;
	}

	// Initialise the cube object.
	result = m_Cube->Initialize(m_Direct3D->GetDevice(), "../Engine/data/cylinder.txt", L"../Engine/data/SandTexture.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialise the texture shader.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	////////// Post Processing ////////////////////
	// Create the convolution shader object
	m_ConvolutionShader = new ConvolutionShaderClass;
	if (!m_ConvolutionShader)
	{
		return false;
	}

	// Initialise the convolution shader
	result = m_ConvolutionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the convolution shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the Small Window
	m_SmallWindow = new OrthoWindowClass();
	if (!m_SmallWindow)
	{
		return false;
	}

	// Initialise the Big Window object.
	result = m_SmallWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho mesh object.", L"Error", MB_OK);
		return false;
	}

	// Create the Big Window
	m_FullScreenWindow = new OrthoWindowClass();
	if (!m_FullScreenWindow)
	{
		return false;
	}

	// Initialise the Big Window object.
	result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho mesh object.", L"Error", MB_OK);
		return false;
	}

	// Create the render texture
	m_RenderTexture = new RenderTextureClass();
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialise the render texture object.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the Down Sample Texture
	m_DownSampleTexture = new RenderTextureClass();
	if (!m_DownSampleTexture)
	{
		return false;
	}

	// Initialise the Down Sample Texture
	result = m_DownSampleTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the Basic Shader
	m_BasicShader = new BasicShaderClass();
	if (!m_BasicShader)
	{
		return false;
	}

	result = m_BasicShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the basic shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the Post Processing Texture.
	m_ConvolutionTexture = new RenderTextureClass();
	if (!m_ConvolutionTexture)
	{
		return false;
	}

	// Initialise the Post Processing Texture object.
	result = m_ConvolutionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight/*, SCREEN_DEPTH, SCREEN_NEAR*/);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the convolution texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the  Up Sample Texture
	m_UpSampleTexture = new RenderTextureClass();
	if (!m_UpSampleTexture)
	{
		return false;
	}

	// Initialise the Up Sample Texture object.
	result = m_UpSampleTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight/*, SCREEN_DEPTH, SCREEN_NEAR*/);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample texture object.", L"Error", MB_OK);
		return false;
	}
	///////////////////////////////////////////////
	// Sky Dome

	// Initialise the Sky Dome Object
	m_SkyDome = new SkyDomeClass();
	if (!m_SkyDome)
	{
		return false;
	}

	result = m_SkyDome->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Initialise the Sky Dome Shader Object
	m_SkyDomeShader = new SkyDomeShaderClass();
	if (!m_SkyDomeShader)
	{
		return false;
	}

	result = m_SkyDomeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	//// Initialise the Noise Texture Object.
	//m_NoiseTexture = new NoiseTextureClass;
	//if (!m_NoiseTexture)
	//{
	//	return false;
	//}

	//result = m_NoiseTexture->Initialize(m_Direct3D->GetDevice());
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the noise texture` object.", L"Error", MB_OK);
	//	return false;
	//}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Release the cube object.
	if (m_Cube)
	{
		m_Cube->Shutdown();
		delete m_Cube;
		m_Cube = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}


	//////////////////// PostProcessing ////////////////////
	// Release the basic shader object.
	if (m_BasicShader)
	{
		m_BasicShader->Shutdown();
		delete m_BasicShader;
		m_BasicShader = 0;
	}

	// Release the convolution shader object.
	if (m_ConvolutionShader)
	{
		m_ConvolutionShader->Shutdown();
		delete m_ConvolutionShader;
		m_ConvolutionShader = 0;
	}

	// Release the ortho mesh objects.
	if (m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the render textures.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	if (m_DownSampleTexture)
	{
		m_DownSampleTexture->Shutdown();
		delete m_DownSampleTexture;
		m_DownSampleTexture = 0;
	}
	if (m_ConvolutionTexture)
	{
		m_ConvolutionTexture->Shutdown();
		delete m_ConvolutionTexture;
		m_ConvolutionTexture = 0;
	}
	if (m_UpSampleTexture)
	{
		m_UpSampleTexture->Shutdown();
		delete m_UpSampleTexture;
		m_UpSampleTexture = 0;
	}

	///////////////////////////////////////////////////////
	////////////////////// Sky Dome //////////////////////
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}


	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	//if (m_NoiseTexture)
	//{
	//	m_NoiseTexture->Shutdown();
	//	delete m_NoiseTexture;
	//	m_NoiseTexture = 0;
	//}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;


	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Render the graphics.
	result = RenderGraphics();
	if(!result)
	{
		return false;
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;


	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Apply Terrain Deformation.
	keyDown = m_Input->IsSpacePressed();
	m_Terrain->GenerateHeightMap(m_Direct3D->GetDevice(), keyDown);	

	// Apply basic smoothing.
	keyDown = m_Input->IsXPressed();
	m_Terrain->SmoothTerrain(m_Direct3D->GetDevice(), keyDown);

	// Apply height-specific smoothing.
	keyDown = m_Input->IsCPressed();
	m_Terrain->FlattenPeaks(m_Direct3D->GetDevice(), keyDown);

	// Toggle Post Processing
	if (keyDown = m_Input->IsPPressed())
	{
		if (is_postProcessed == true)
		{
			is_postProcessed = false;
		} 
		else if (is_postProcessed == false)
		{
			is_postProcessed = true;
		}
	}

	///////// Camera Controls /////////
	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsAPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsZPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);
	
	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::RenderGraphics()
{
	bool result;


	if (is_postProcessed)
	{
		// Render the complete scene to a texture.
		result = RenderSceneToTexture();
		if (!result)
		{
			return false;
		}

		// Downscale the render texture for performance reasons.
		result = DownScaleTexture();
		if (!result)
		{
			return false;
		}

		// Apply the Convolution shader to the downscaled render texture.
		result = RenderConvolutionToTexture();
		if (!result)
		{
			return false;
		}

		// Upscale the render texture back to the correct size.
		result = UpScaleTexture();
		if (!result)
		{
			return false;
		}

		// Render the processed scene to the window.
		result = Render2DTextureScene();
		if (!result)
		{
			return false;
		}
	}
	else
	{
		result = RenderScene();
		if (!result)
		{
			return false;
		}
	}


	////////////////////// Render UI ////////////////////
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();
	
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


bool ApplicationClass::RenderSceneToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//////////////////// SkyDome ////////////////////
		// Get the position of the camera.
		cameraPosition = m_Camera->GetPosition();


		// Translate the sky dome to be centered around the camera position.
		D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// Turn off back face culling.
		m_Direct3D->TurnOffCulling();

		// Turn off the Z buffer.
		m_Direct3D->TurnZBufferOff();

		// Render the sky dome using the sky dome shader.
		m_SkyDome->Render(m_Direct3D->GetDeviceContext());
		m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

		// Turn back face culling back on.
		m_Direct3D->TurnOnCulling();

		// Turn the Z buffer back on.
		m_Direct3D->TurnZBufferOn();

		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);
	
	//////////////////// Terrain ////////////////////
		// Put the terrain object onto the pipeline for rendering.
		m_Terrain->Render(m_Direct3D->GetDeviceContext());

		// Render the scene now and it will draw to the render to texture instead of the back buffer.
		//Render the terrain using the terrain shader.
		result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetSandTexture(),
				m_Terrain->GetSlopeTexture());
		if (!result)
		{
			return false;
		}

	//////////////////// Cactus ////////////////////
		// Render the model buffers.
		m_Cube->Render(m_Direct3D->GetDeviceContext());
	
		// Render the cube using the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Cube->GetVertexCount(), m_Cube->GetInstanceCount(), worldMatrix,
			viewMatrix, projectionMatrix, m_Cube->GetTexture());
		if (!result)
		{
			return false;
		}
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::DownScaleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Set the render target to be the render to texture.
	m_DownSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the render to texture.
	m_DownSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Create a usable veiw matrix.
	D3DXMatrixIdentity(&viewMatrix);
	viewMatrix._43 = 0.1f;

	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_Direct3D->GetOrthoMatrix(projectionMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = m_BasicShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderConvolutionToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, convolutionKernel;
	float screenSizeX;
	bool result;


	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)m_ConvolutionTexture->GetTextureWidth();


	// Set the render target to be the render to texture.
	m_ConvolutionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the render to texture.
	m_ConvolutionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Create a veiw matrix.
	D3DXMatrixIdentity(&viewMatrix);
	viewMatrix._43 = 0.1f;

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_Direct3D->GetOrthoMatrix(projectionMatrix);

	// Create the convolution kernel matrix.
	D3DXMatrixIdentity(&convolutionKernel);
	convolutionKernel._22 = 0;
	convolutionKernel._13 = -1;
	convolutionKernel._31 = -1;

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = m_ConvolutionShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_DownSampleTexture->GetShaderResourceView(), convolutionKernel, screenSizeX);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::UpScaleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_UpSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the render to texture.
	m_UpSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Create a sensible veiw matrix.
	D3DXMatrixIdentity(&viewMatrix);
	viewMatrix._43 = 0.1f;

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = m_BasicShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), 
		worldMatrix, viewMatrix, orthoMatrix, m_ConvolutionTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::Render2DTextureScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Get the world, view, and ortho matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	//m_Camera->GetViewMatrix(viewMatrix);

	D3DXMatrixIdentity(&viewMatrix);
	viewMatrix._43 = 0.1f;

	m_Direct3D->GetOrthoMatrix(projectionMatrix);		// Using an orthographic projection.

	// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Debug - Render the ortho window with the processed texture onto the screen using the basic shader.
	result = m_BasicShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_UpSampleTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

 // Function that just rendered the scene normally.
bool ApplicationClass::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//////////////////// SkyDome ////////////////////
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();


	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_Direct3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	////////////////// Terrain //////////////////
	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetSandTexture(),
		m_Terrain->GetSlopeTexture());
	if (!result)
	{
		return false;
	}

	////////////////// Cactus //////////////////
	// Render the model buffers.
	m_Cube->Render(m_Direct3D->GetDeviceContext());

	// Render the cube using the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Cube->GetVertexCount(), m_Cube->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, m_Cube->GetTexture());
	if (!result)
	{
		return false;
	}

	return true;
}



