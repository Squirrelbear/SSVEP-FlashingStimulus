////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_flasher;
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}

	m_flasher = new Flasher;
	if(!m_flasher)
	{
		return false;
	}

	result = m_flasher->Initialize(screenWidth, screenHeight, m_Graphics, m_hwnd);
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Flasher.", L"Error", MB_OK);
		return false;
	}

	m_modeSwap = new PresetModeSwap;
	if(!m_modeSwap)
	{
		return false;
	}

	result = m_modeSwap->Initialize(m_Graphics, m_flasher);
	if(!result)
	{
		return false;
	}

	m_fps = new FpsMonitor;
	if(!m_fps)
	{
		return false;
	}

	result = InitializeTweakBar(m_Graphics->GetDevice());
	if(!result)
	{
		return false;
	}

	//m_Graphics->saveCardDetails();

	return true;
}

void SystemClass::Shutdown()
{
	if(m_flasher)
	{
		m_flasher->Shutdown();
		delete m_flasher;
		m_flasher = 0;
	}

	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	if(m_modeSwap)
	{
		m_modeSwap->Shutdown();
		delete m_modeSwap;
		m_modeSwap = 0;
	}

	if(m_fps)
	{
		delete m_fps;
		m_fps = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	LONGLONG prev_time;
	LONGLONG cur_time = timeGetTime();       // current time
	DWORD  time_count=5;    // ms per frame, default if no performance counter
	LONGLONG perf_cnt;       // performance timer frequency
	BOOL perf_flag=FALSE;    // flag determining which timer to use
	LONGLONG next_time=0;    // time to render next frame
	//BOOL move_flag=TRUE;     // flag noting if we have moved yet

	// is there a performance counter available?

	if (QueryPerformanceFrequency((LARGE_INTEGER *) &perf_cnt)) {
		// yes, set time_count and timer choice flag
		perf_flag=TRUE;
		time_count=perf_cnt/50;        // calculate time per frame based on frequency
		QueryPerformanceCounter((LARGE_INTEGER *) &next_time);
	} else {
		// no performance counter, read in using timeGetTime
		next_time=timeGetTime();
	}

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			prev_time = cur_time;

			// use the appropriate method to get time
			if (perf_flag)
				QueryPerformanceCounter((LARGE_INTEGER *) &cur_time);
			else
				cur_time=timeGetTime();

			LONGLONG timeDiff = cur_time - prev_time;
			if(timeDiff < 0) timeDiff = 0;

			// Check if the user pressed escape and wants to exit the application.
			if(m_Input->IsKeyDown(VK_ESCAPE))
			{
				done = true;
			}

			m_flasher->Update(timeDiff);

			result = m_Graphics->Update(timeDiff);
			if(!result)
			{
				done = true;
			}

			m_modeSwap->Update();
			m_fps->Update(timeDiff/5000.0);

			// is it time to render the frame?
			if (cur_time>next_time) {
				m_fps->frameOccurence();
				m_Graphics->BeginFrame();
				m_flasher->Render(m_Graphics->GetD3D()->GetDeviceContext());
				TwDraw();
				m_Graphics->EndFrame();

				// set time for next frame
				next_time += time_count;

				// If we get more than a frame ahead, allow us to drop one
				// Otherwise, we will never catch up if we let the error
				// accumulate, and message handling will suffer
				if (next_time < cur_time)
					next_time = cur_time + time_count;
			}
		}

	}

	return;

}

bool SystemClass::Frame()
{
	/*bool result;

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if(!result)
	{
		return false;
	}*/

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// kill the Tweak Bars instances
	TwTerminate();

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

bool SystemClass::InitializeTweakBar(ID3D11Device* m_device)
{
	// Initialize AntTweakBar
    if (!TwInit(TW_DIRECT3D11, m_device))
    {
        //MessageBoxA(wnd, TwGetLastError(), "AntTweakBar initialization failed", MB_OK|MB_ICONERROR);
		return false;
    }

    // Create a tweak bar
    TwBar *bar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='Flasher sample to test SSVEP stimulus.' "); // Message added to the help bar.
    int barSize[2] = {224, 600};
    TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

	TwAddVarCB(bar, "Preset Mode", TW_TYPE_INT32, SetPresetModeCB, GetPresetModeCB, NULL, "min=0 max=19 group=PresetModes");

	TwAddVarCB(bar, "Flash Time", TW_TYPE_INT32, SetFlashTimeCB, GetFlashTimeCB, NULL, "min=0 max=1000 group=Time");
    TwAddVarCB(bar, "Flash Off Time", TW_TYPE_INT32, SetOffFlashTimeCB, GetOffFlashTimeCB, NULL, "min=0 max=1000 group=Time");
    TwAddVarCB(bar, "Flash %", TW_TYPE_FLOAT, SetFlashTimePercentCB, GetFlashTimePercentCB, NULL, "min=0 max=1 group=Time");
	TwAddVarCB(bar, "Flash Off %", TW_TYPE_FLOAT, SetOffFlashTimePercentCB, GetOffFlashTimePercentCB, NULL, "min=0 max=1 group=Time");

	TwAddVarCB(bar, "Interval", TW_TYPE_INT32, SetTotalIntervalCB, GetTotalIntervalCB, NULL, "min=1 max=2000 group=Time");
    TwAddVarCB(bar, "Interval Hz", TW_TYPE_FLOAT, SetTotalIntervalHzCB, GetTotalIntervalHzCB, NULL, "min=1 max=2000 group=Time");

	TwAddVarCB(bar, "Show Flash", TW_TYPE_BOOLCPP, SetCanFlashCB, GetCanFlashCB, NULL, "group=Display");
	TwAddVarCB(bar, "Flash Black", TW_TYPE_BOOLCPP, SetFlashBlackCB, GetFlashBlackCB, NULL, "group=Display");
    TwAddVarCB(bar, "Show Video", TW_TYPE_BOOLCPP, SetShowVideoCB, GetShowVideoCB, NULL, "group=Display");
	TwAddVarCB(bar, "Black Background", TW_TYPE_BOOLCPP, SetIsBlackBGCB, GetIsBlackBGCB, NULL, "group=Display");

	TwAddVarCB(bar, "Flash Grid", TW_TYPE_BOOLCPP, SetFlashGridCB, GetFlashGridCB, NULL, "group=Display");
	TwAddVarCB(bar, "Grid Width", TW_TYPE_INT32, SetGridWidthCB, GetGridWidthCB, NULL, "min=1 max=100 group=GridOptions");
    TwAddVarCB(bar, "Grid Height", TW_TYPE_INT32, SetGridHeightCB, GetGridHeightCB, NULL, "min=1 max=100 group=GridOptions");
	TwAddVarCB(bar, "Edge Flash", TW_TYPE_BOOLCPP, SetEdgeFlashCB, GetEdgeFlashCB, NULL, "group=GridOptions");
	TwAddVarCB(bar, "Use Fixed Pos", TW_TYPE_BOOLCPP, SetUseFixedPosCB, GetUseFixedPosCB, NULL, "group=GridOptions");
	
	TwAddVarCB(bar, "FixedGridXPos", TW_TYPE_INT32, SetFixedXCB, GetFixedXCB, NULL, "min=1 max=100 group=GridOptions");
	TwAddVarCB(bar, "FixedGridYPos", TW_TYPE_INT32, SetFixedYCB, GetFixedYCB, NULL, "min=1 max=100 group=GridOptions");

	TwDefine(" TweakBar/GridOptions     group=Display  opened=false   ");

	TwAddVarCB(bar, "Graphic Flash", TW_TYPE_BOOLCPP, SetGraphicFlashCB, GetGraphicFlashCB, NULL, "group=FlashGraphics");
	TwAddVarCB(bar, "Black Underlay", TW_TYPE_BOOLCPP, SetShowBlackUnderlayCB, GetShowBlackUnderlayCB, NULL, "group=FlashGraphics");
	TwAddVarCB(bar, "Sequence Flash", TW_TYPE_BOOLCPP, SetSequenceGraphicFlashCB, GetSequenceGraphicFlashCB, NULL, "group=FlashGraphics");
	TwAddVarCB(bar, "Per Graphic Clock", TW_TYPE_BOOLCPP, SetUsePerImageClockCB, GetUsePerImageClockCB, NULL, "group=FlashGraphics");
	
	
	TwAddVarCB(bar, "BitmapID1", TW_TYPE_INT32, SetBitmapIDOneCB, GetBitmapIDOneCB, NULL, "group=FlashGraphicOne");
	TwAddVarCB(bar, "Flash1", TW_TYPE_BOOLCPP, SetFlashBitmapOneCB, GetFlashBitmapOneCB, NULL, "group=FlashGraphicOne");
	TwAddVarCB(bar, "Show1", TW_TYPE_BOOLCPP, SetShowBitmapOneCB, GetShowBitmapOneCB, NULL, "group=FlashGraphicOne");
	
	TwAddVarCB(bar, "Flash Time One", TW_TYPE_INT32, SetFlashTimeBMPOneCB, GetFlashTimeBMPOneCB, NULL, "min=0 max=1000 group=ClockOne");
	TwAddVarCB(bar, "Flash Off Time One", TW_TYPE_INT32, SetOffFlashTimeBMPOneCB, GetOffFlashTimeBMPOneCB, NULL, "min=0 max=1000 group=ClockOne");
	TwAddVarCB(bar, "Flash % One", TW_TYPE_FLOAT, SetFlashTimePercentBMPOneCB, GetFlashTimePercentBMPOneCB, NULL, "min=0 max=1 group=ClockOne");
	TwAddVarCB(bar, "Flash Off % One", TW_TYPE_FLOAT, SetOffFlashTimePercentBMPOneCB, GetOffFlashTimePercentBMPOneCB, NULL, "min=0 max=1 group=ClockOne");
	TwAddVarCB(bar, "Interval One", TW_TYPE_INT32, SetTotalIntervalBMPOneCB, GetTotalIntervalBMPOneCB, NULL, "min=1 max=2000 group=ClockOne");
	TwAddVarCB(bar, "Interval Hz One", TW_TYPE_FLOAT, SetTotalIntervalHzBMPOneCB, GetTotalIntervalHzBMPOneCB, NULL, "min=1 max=2000 group=ClockOne");

	TwDefine(" TweakBar/ClockOne     group=FlashGraphicOne  opened=false   ");

	TwAddVarCB(bar, "BitmapID2", TW_TYPE_INT32, SetBitmapIDTwoCB, GetBitmapIDTwoCB, NULL, "group=FlashGraphicTwo");
	TwAddVarCB(bar, "Flash2", TW_TYPE_BOOLCPP, SetFlashBitmapTwoCB, GetFlashBitmapTwoCB, NULL, "group=FlashGraphicTwo");
	TwAddVarCB(bar, "Show2", TW_TYPE_BOOLCPP, SetShowBitmapTwoCB, GetShowBitmapTwoCB, NULL, "group=FlashGraphicTwo");
	
	TwAddVarCB(bar, "Flash Time Two", TW_TYPE_INT32, SetFlashTimeBMPTwoCB, GetFlashTimeBMPTwoCB, NULL, "min=0 max=1000 group=ClockTwo");
	TwAddVarCB(bar, "Flash Off Time Two", TW_TYPE_INT32, SetOffFlashTimeBMPTwoCB, GetOffFlashTimeBMPTwoCB, NULL, "min=0 max=1000 group=ClockTwo");
	TwAddVarCB(bar, "Flash % Two", TW_TYPE_FLOAT, SetFlashTimePercentBMPTwoCB, GetFlashTimePercentBMPTwoCB, NULL, "min=0 max=1 group=ClockTwo");
	TwAddVarCB(bar, "Flash Off % Two", TW_TYPE_FLOAT, SetOffFlashTimePercentBMPTwoCB, GetOffFlashTimePercentBMPTwoCB, NULL, "min=0 max=1 group=ClockTwo");
	TwAddVarCB(bar, "Interval Two", TW_TYPE_INT32, SetTotalIntervalBMPTwoCB, GetTotalIntervalBMPTwoCB, NULL, "min=1 max=2000 group=ClockTwo");
	TwAddVarCB(bar, "Interval Hz Two", TW_TYPE_FLOAT, SetTotalIntervalHzBMPTwoCB, GetTotalIntervalHzBMPTwoCB, NULL, "min=1 max=2000 group=ClockTwo");

	TwDefine(" TweakBar/ClockTwo     group=FlashGraphicTwo  opened=false   ");	

	TwAddVarCB(bar, "BitmapID3", TW_TYPE_INT32, SetBitmapIDThreeCB, GetBitmapIDThreeCB, NULL, "group=FlashGraphicThree");
	TwAddVarCB(bar, "Flash3", TW_TYPE_BOOLCPP, SetFlashBitmapThreeCB, GetFlashBitmapThreeCB, NULL, "group=FlashGraphicThree");
	TwAddVarCB(bar, "Show3", TW_TYPE_BOOLCPP, SetShowBitmapThreeCB, GetShowBitmapThreeCB, NULL, "group=FlashGraphicThree");
	
	TwAddVarCB(bar, "Flash Time Three", TW_TYPE_INT32, SetFlashTimeBMPThreeCB, GetFlashTimeBMPThreeCB, NULL, "min=0 max=1000 group=ClockThree");
	TwAddVarCB(bar, "Flash Off Time Three", TW_TYPE_INT32, SetOffFlashTimeBMPThreeCB, GetOffFlashTimeBMPThreeCB, NULL, "min=0 max=1000 group=ClockThree");
	TwAddVarCB(bar, "Flash % Three", TW_TYPE_FLOAT, SetFlashTimePercentBMPThreeCB, GetFlashTimePercentBMPThreeCB, NULL, "min=0 max=1 group=ClockThree");
	TwAddVarCB(bar, "Flash Off % Three", TW_TYPE_FLOAT, SetOffFlashTimePercentBMPThreeCB, GetOffFlashTimePercentBMPThreeCB, NULL, "min=0 max=1 group=ClockThree");
	TwAddVarCB(bar, "Interval Three", TW_TYPE_INT32, SetTotalIntervalBMPThreeCB, GetTotalIntervalBMPThreeCB, NULL, "min=1 max=2000 group=ClockThree");
	TwAddVarCB(bar, "Interval Hz Three", TW_TYPE_FLOAT, SetTotalIntervalHzBMPThreeCB, GetTotalIntervalHzBMPThreeCB, NULL, "min=1 max=2000 group=ClockThree");

	TwDefine(" TweakBar/ClockThree     group=FlashGraphicThree  opened=false   ");	

	TwDefine(" TweakBar/FlashGraphicOne     group=FlashGraphics  opened=false   ");
	TwDefine(" TweakBar/FlashGraphicTwo     group=FlashGraphics  opened=false   ");
	TwDefine(" TweakBar/FlashGraphicThree   group=FlashGraphics  opened=false   ");

	TwAddVarCB(bar, "Clock Time", TW_TYPE_DOUBLE, SetClockTimeCB, GetClockTimeCB, NULL, "group=Debug readonly=true");
	TwAddVarCB(bar, "FPS", TW_TYPE_DOUBLE, SetFPSCB, GetFPSCB, NULL, "group=Debug readonly=true");
	TwAddVarCB(bar, "Frame Time", TW_TYPE_DOUBLE, SetFrameTimeCB, GetFrameTimeCB, NULL, "group=Debug readonly=true");
	TwAddButton(bar, "Save", SavePresetsCB, NULL, " label='Save ALL current modes to File' group=Debug");
	TwAddButton(bar, "Reload", ReloadPresetsCB, NULL, " label='Reload ALL modes' group=Debug");
	TwAddButton(bar, "Reset Default", ReturnDefaultPresetCB, NULL, " label='Reset the current mode to defaults.' group=Debug");


    // Add variables to the tweak bar
    //TwAddVarCB(bar, "Width", TW_TYPE_INT32, SetWidthCB, GetWidthCB, NULL, "min=1 max=500 group=Sponge keyincr=l keydecr=L");
    /*TwAddVarCB(bar, "Ambient Occlusion", TW_TYPE_BOOLCPP, SetSpongeAOCB, GetSpongeAOCB, NULL, "group=Sponge key=o");
    TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_SpongeRotation, "opened=true axisz=-z group=Sponge");
    TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &g_Animate, "group=Sponge key=a");
    TwAddVarRW(bar, "Animation speed", TW_TYPE_FLOAT, &g_AnimationSpeed, "min=-10 max=10 step=0.1 group=Sponge keyincr=+ keydecr=-");
    TwAddVarRW(bar, "Light direction", TW_TYPE_DIR3F, &g_LightDir, "opened=true axisz=-z showval=false");
    TwAddVarRW(bar, "Camera distance", TW_TYPE_FLOAT, &g_CamDistance, "min=0 max=4 step=0.01 keyincr=PGUP keydecr=PGDOWN");
    TwAddVarRW(bar, "Background", TW_TYPE_COLOR4F, &g_BackgroundColor, "colormode=hls");*/

	return true;

}

GraphicsClass* SystemClass::getGraphics()
{
	return m_Graphics;
}

Flasher* SystemClass::getFlasher()
{
	return m_flasher;
}

PresetModeSwap* SystemClass::getModeSwap()
{
	return m_modeSwap;
}

FpsMonitor* SystemClass::getFPSMonitor()
{
	return m_fps;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	// Send event message to AntTweakBar
    if (TwEventWin(hwnd, umessage, wparam, lparam))
        return 0; // Event has been handled by AntTweakBar

	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

//void TW_CALL SetWidthCB(const void *value, void * /*clientData*/)
//{
	//ApplicationHandle->getGraphics()->getBitmap()->SetDimensions(*static_cast<const int *>(value), ApplicationHandle->getGraphics()->getBitmap()->GetHeight());
//}
//void TW_CALL GetWidthCB(void *value, void * /*clientData*/)
//{
	//*static_cast<int *>(value) = ApplicationHandle->getGraphics()->getBitmap()->GetWidth();
//}*/

void TW_CALL SetFlashTimeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetTimer()->SetPhaseOne(*static_cast<const int *>(value));
}
void TW_CALL SetOffFlashTimeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetTimer()->SetPhaseTwo( *static_cast<const int *>(value));
}
void TW_CALL GetFlashTimeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetTimer()->GetPhaseOne();
}
void TW_CALL GetOffFlashTimeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetTimer()->GetPhaseTwo();
}

void TW_CALL SetFlashTimePercentCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetTimer()->SetPhaseOnePercent(*static_cast<const float *>(value));
}
void TW_CALL SetOffFlashTimePercentCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetTimer()->SetPhaseTwoPercent(*static_cast<const float *>(value));

}
void TW_CALL GetFlashTimePercentCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetTimer()->GetPhaseOnePercent();
}
void TW_CALL GetOffFlashTimePercentCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetTimer()->GetPhaseTwoPercent();
}

void TW_CALL SetTotalIntervalCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetTimer()->SetInterval(*static_cast<const int *>(value));
}
void TW_CALL GetTotalIntervalCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetTimer()->GetInterval();
}

void TW_CALL SetTotalIntervalHzCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetTimer()->SetIntervalHz(*static_cast<const float *>(value));
}
void TW_CALL GetTotalIntervalHzCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetTimer()->GetIntervalHz();
}

void TW_CALL SetFlashGridCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFlashGrid( *static_cast<const bool *>(value));
}
void TW_CALL GetFlashGridCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetFlashGrid();
}

void TW_CALL SetFlashBlackCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFlashBlack( *static_cast<const bool *>(value));
}
void TW_CALL GetFlashBlackCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetFlashBlack();
}

void TW_CALL SetGridHeightCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetGridHeight( *static_cast<const int *>(value));
}
void TW_CALL GetGridHeightCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetGridHeight();
}
void TW_CALL SetGridWidthCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetGridWidth( *static_cast<const int *>(value));
}
void TW_CALL GetGridWidthCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetGridWidth();
}

void TW_CALL SetShowVideoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getGraphics()->SetShowVideo( *static_cast<const bool *>(value));
}
void TW_CALL GetShowVideoCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getGraphics()->GetShowVideo();
}

void TW_CALL SetPresetModeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getModeSwap()->QueueMode( *static_cast<const int *>(value));
}
void TW_CALL GetPresetModeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getModeSwap()->GetMode();
}

void TW_CALL SetIsBlackBGCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getGraphics()->SetIsBlackBG( *static_cast<const bool *>(value));
}
void TW_CALL GetIsBlackBGCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getGraphics()->GetIsBlackBG();
}

void TW_CALL SetClockTimeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getGraphics()->GetVideoPlayer()->SetClockTime( *static_cast<const double *>(value));
}
void TW_CALL GetClockTimeCB(void *value, void * /*clientData*/)
{
	*static_cast<double *>(value) = ApplicationHandle->getGraphics()->GetVideoPlayer()->GetClockTime();
}

void TW_CALL SetFrameTimeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFPSMonitor()->SetFrameTime( *static_cast<const double *>(value));
}
void TW_CALL GetFrameTimeCB(void *value, void * /*clientData*/)
{
	*static_cast<double *>(value) = ApplicationHandle->getFPSMonitor()->GetFrameTime();
}

void TW_CALL SetFPSCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFPSMonitor()->SetFPS( *static_cast<const double *>(value));
}
void TW_CALL GetFPSCB(void *value, void * /*clientData*/)
{
	*static_cast<double *>(value) = ApplicationHandle->getFPSMonitor()->GetFPS();
}


void TW_CALL SavePresetsCB(void *clientData)
{
	ApplicationHandle->getModeSwap()->saveModeFile();
}

void TW_CALL ReloadPresetsCB(void *clientData)
{
	ApplicationHandle->getModeSwap()->loadModeFile();
	ApplicationHandle->getModeSwap()->ApplyMode(ApplicationHandle->getModeSwap()->GetMode());
}

void TW_CALL ReturnDefaultPresetCB(void *clientData)
{
	ApplicationHandle->getModeSwap()->populateDefaults(ApplicationHandle->getModeSwap()->GetMode());
	ApplicationHandle->getModeSwap()->ApplyMode(ApplicationHandle->getModeSwap()->GetMode());
}

void TW_CALL SetCanFlashCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetCanFlash( *static_cast<const bool *>(value));
}
void TW_CALL GetCanFlashCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetCanFlash();
}

void TW_CALL SetEdgeFlashCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetEdgeFlash( *static_cast<const bool *>(value));
}
void TW_CALL GetEdgeFlashCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetEdgeFlash();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void TW_CALL SetGraphicFlashCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetGraphicFlash( *static_cast<const bool *>(value));
}
void TW_CALL GetGraphicFlashCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetGraphicFlash();
}

void TW_CALL SetShowBlackUnderlayCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetShowBlackUnderlay( *static_cast<const bool *>(value));
}
void TW_CALL GetShowBlackUnderlayCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetShowBlackUnderlay();
}

void TW_CALL SetBitmapIDOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetBitmapID( 0,  *static_cast<const int *>(value));
}
void TW_CALL GetBitmapIDOneCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapID(0);
}

void TW_CALL SetFlashBitmapOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFlashBitmap( 0, *static_cast<const bool *>(value));
}
void TW_CALL GetFlashBitmapOneCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetFlashBitmap(0);
}

void TW_CALL SetShowBitmapOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetShowBitmap( 0, *static_cast<const bool *>(value));
}
void TW_CALL GetShowBitmapOneCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetShowBitmap(0);
}

// !!!!!
void TW_CALL SetBitmapIDTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetBitmapID( 1,  *static_cast<const int *>(value));
}
void TW_CALL GetBitmapIDTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapID(1);
}

void TW_CALL SetFlashBitmapTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFlashBitmap( 1, *static_cast<const bool *>(value));
}
void TW_CALL GetFlashBitmapTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetFlashBitmap(1);
}

void TW_CALL SetShowBitmapTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetShowBitmap( 1, *static_cast<const bool *>(value));
}
void TW_CALL GetShowBitmapTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetShowBitmap(1);
}

//!!!!
void TW_CALL SetBitmapIDThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetBitmapID( 2,  *static_cast<const int *>(value));
}
void TW_CALL GetBitmapIDThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapID(2);
}

void TW_CALL SetFlashBitmapThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFlashBitmap( 2, *static_cast<const bool *>(value));
}
void TW_CALL GetFlashBitmapThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetFlashBitmap(2);
}

void TW_CALL SetShowBitmapThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetShowBitmap( 2, *static_cast<const bool *>(value));
}
void TW_CALL GetShowBitmapThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetShowBitmap(2);
}

void TW_CALL SetFixedXCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFixedX( *static_cast<const int *>(value));
}
void TW_CALL GetFixedXCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetFixedX();
}

void TW_CALL SetFixedYCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetFixedY( *static_cast<const int *>(value));
}
void TW_CALL GetFixedYCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetFixedY();
}

void TW_CALL SetUseFixedPosCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetUseFixedPos( *static_cast<const bool *>(value));
}
void TW_CALL GetUseFixedPosCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetUseFixedPos();
}

void TW_CALL SetSequenceGraphicFlashCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetSequenceGraphicFlash( *static_cast<const bool *>(value));
}
void TW_CALL GetSequenceGraphicFlashCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetSequenceGraphicFlash();
}

void TW_CALL SetUsePerImageClockCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->SetUsePerImageClock( *static_cast<const bool *>(value));
}
void TW_CALL GetUsePerImageClockCB(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = ApplicationHandle->getFlasher()->GetUsePerImageClock();
}

void TW_CALL SetFlashTimeBMPOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(0)->SetPhaseOne(*static_cast<const int *>(value));
}
void TW_CALL SetOffFlashTimeBMPOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(0)->SetPhaseTwo( *static_cast<const int *>(value));
}
void TW_CALL GetFlashTimeBMPOneCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(0)->GetPhaseOne();
}
void TW_CALL GetOffFlashTimeBMPOneCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(0)->GetPhaseTwo();
}

void TW_CALL SetFlashTimePercentBMPOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(0)->SetPhaseOnePercent(*static_cast<const float *>(value));
}
void TW_CALL SetOffFlashTimePercentBMPOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(0)->SetPhaseTwoPercent(*static_cast<const float *>(value));

}
void TW_CALL GetFlashTimePercentBMPOneCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(0)->GetPhaseOnePercent();
}
void TW_CALL GetOffFlashTimePercentBMPOneCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(0)->GetPhaseTwoPercent();
}

void TW_CALL SetTotalIntervalBMPOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(0)->SetInterval(*static_cast<const int *>(value));
}
void TW_CALL GetTotalIntervalBMPOneCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(0)->GetInterval();
}

void TW_CALL SetTotalIntervalHzBMPOneCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(0)->SetIntervalHz(*static_cast<const float *>(value));
}
void TW_CALL GetTotalIntervalHzBMPOneCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(0)->GetIntervalHz();
}

void TW_CALL SetFlashTimeBMPTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(1)->SetPhaseOne(*static_cast<const int *>(value));
}
void TW_CALL SetOffFlashTimeBMPTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(1)->SetPhaseTwo( *static_cast<const int *>(value));
}
void TW_CALL GetFlashTimeBMPTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(1)->GetPhaseOne();
}
void TW_CALL GetOffFlashTimeBMPTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(1)->GetPhaseTwo();
}

void TW_CALL SetFlashTimePercentBMPTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(1)->SetPhaseOnePercent(*static_cast<const float *>(value));
}
void TW_CALL SetOffFlashTimePercentBMPTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(1)->SetPhaseTwoPercent(*static_cast<const float *>(value));

}
void TW_CALL GetFlashTimePercentBMPTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(1)->GetPhaseOnePercent();
}
void TW_CALL GetOffFlashTimePercentBMPTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(1)->GetPhaseTwoPercent();
}

void TW_CALL SetTotalIntervalBMPTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(1)->SetInterval(*static_cast<const int *>(value));
}
void TW_CALL GetTotalIntervalBMPTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(1)->GetInterval();
}

void TW_CALL SetTotalIntervalHzBMPTwoCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(1)->SetIntervalHz(*static_cast<const float *>(value));
}
void TW_CALL GetTotalIntervalHzBMPTwoCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(1)->GetIntervalHz();
}

void TW_CALL SetFlashTimeBMPThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(2)->SetPhaseOne(*static_cast<const int *>(value));
}
void TW_CALL SetOffFlashTimeBMPThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(2)->SetPhaseTwo( *static_cast<const int *>(value));
}
void TW_CALL GetFlashTimeBMPThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(2)->GetPhaseOne();
}
void TW_CALL GetOffFlashTimeBMPThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(2)->GetPhaseTwo();
}

void TW_CALL SetFlashTimePercentBMPThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(2)->SetPhaseOnePercent(*static_cast<const float *>(value));
}
void TW_CALL SetOffFlashTimePercentBMPThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(2)->SetPhaseTwoPercent(*static_cast<const float *>(value));

}
void TW_CALL GetFlashTimePercentBMPThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(2)->GetPhaseOnePercent();
}
void TW_CALL GetOffFlashTimePercentBMPThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(2)->GetPhaseTwoPercent();
}

void TW_CALL SetTotalIntervalBMPThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(2)->SetInterval(*static_cast<const int *>(value));
}
void TW_CALL GetTotalIntervalBMPThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<int *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(2)->GetInterval();
}

void TW_CALL SetTotalIntervalHzBMPThreeCB(const void *value, void * /*clientData*/)
{
	ApplicationHandle->getFlasher()->GetBitmapTimer(2)->SetIntervalHz(*static_cast<const float *>(value));
}
void TW_CALL GetTotalIntervalHzBMPThreeCB(void *value, void * /*clientData*/)
{
	*static_cast<float *>(value) = ApplicationHandle->getFlasher()->GetBitmapTimer(2)->GetIntervalHz();
}