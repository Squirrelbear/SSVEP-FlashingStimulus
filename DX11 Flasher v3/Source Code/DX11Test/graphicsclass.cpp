////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Video = 0;
	//m_flasher = 0;
	//m_TextureShader = 0;
	//m_Bitmap = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	result = InitPlayer(hwnd);
	if(!result)
	{
		return false;
	}

	/*
	m_flasher = new Flasher;
	if(!m_flasher)
	{
		return false;
	}

	result = m_flasher->Initialize(screenWidth, screenHeight, this, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Flasher.", L"Error", MB_OK);
		return false;
	}*/

	/*
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Resources/SwordItem.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	m_Bitmap->SetPosition(100,100);
	*/

	m_showVideo = true;
	m_isBlackBG = true;

	return true;
}


void GraphicsClass::Shutdown()
{
	/*
	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}*/

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	
	// release the video object
	if(m_Video)
	{
		m_Video->Shutdown();
		delete m_Video;
		m_Video = 0;
	}

	/*if(m_flasher)
	{
		m_flasher->Shutdown();
		delete m_flasher;
		m_flasher = 0;
	}*/

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Update(LONGLONG timeDiff)
{
	m_Video->Update(timeDiff);
	return true;
}

bool GraphicsClass::BeginFrame()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	//bool result;

	// Clear the buffers to begin the scene.
	if(m_isBlackBG)
		m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	else
		m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	DrawVideo(hwnd);

	return true;
}

bool GraphicsClass::EndFrame()
{
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	//m_flasher->Render(m_D3D->GetDeviceContext());

	/*
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if(!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if(!result)
	{
		return false;
	}*/

	// Draw tweak bars
    TwDraw();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

ID3D11Device* GraphicsClass::GetDevice()
{
	return m_D3D->GetDevice();
}

D3DClass* GraphicsClass::GetD3D()
{
	return m_D3D;
}

CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
}

/*BitmapClass* GraphicsClass::getBitmap()
{
	return m_Bitmap;
}*/

void GraphicsClass::saveCardDetails()
{
	std::ofstream fout;
	fout.open("carddetails.txt");

	char* cardName = new char[128];
	int memorySize = 0;
	m_D3D->GetVideoCardInfo(cardName, memorySize);

	fout << "Card name: " << cardName << std::endl;
	fout << "Memory: " << memorySize << std::endl;
	fout.close();
}

bool GraphicsClass::InitPlayer(HWND hwnd)
{
	m_Video = new (std::nothrow) SpriteVideo();
    if (m_Video == NULL)
    {
        return false;
    }

	RECT rect_ClientArea;
	GetClientRect (hwnd, &rect_ClientArea);
	int width = rect_ClientArea.right - rect_ClientArea.left;
	int height = rect_ClientArea.bottom - rect_ClientArea.top;

	std::wstring folder;
	if (_access("../Resources/video/video0.dds", 0) == -1)
	{
		folder = L"C:\\outputVideo\\dds2\\";
	}
	else
	{
		folder = L"../Resources/video/";
	}

	if(!m_Video->Initialize(m_D3D, hwnd, folder, L"video", L".dds", 0, 922, 1000.0 * 13.94 / 923, width, height ))
	{
		return false;
	}

    return true;
}

bool GraphicsClass::GetShowVideo()
{
	return m_showVideo;
}

void GraphicsClass::SetShowVideo(bool showVideo)
{
	m_showVideo = showVideo;
}

bool GraphicsClass::GetIsBlackBG()
{
	return m_isBlackBG;
}

void GraphicsClass::SetIsBlackBG(bool isBlack)
{
	m_isBlackBG = isBlack;
}

void GraphicsClass::DrawVideo(HWND hwnd)
{
	if(m_showVideo && m_Video)
	{
		D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);
		m_Video->Draw(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);
	}
}

SpriteVideo* GraphicsClass::GetVideoPlayer()
{
	return m_Video;
}