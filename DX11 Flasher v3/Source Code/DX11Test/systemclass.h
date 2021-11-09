#pragma comment(lib, "Winmm.lib")

////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>
// MMSystem is used for timeGetTime()
#include <MMSystem.h>

///////////////
// LIBRARIES //
///////////////
#include "AntTweakBar.h"

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"
#include "Flasher.h"
#include "PresetModeSwap.h"
#include "FPSMonitor.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	GraphicsClass* getGraphics();
	Flasher* getFlasher();
	PresetModeSwap* getModeSwap();
	FpsMonitor* getFPSMonitor();

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	bool InitializeTweakBar(ID3D11Device*);
	bool InitializeVideo();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	PresetModeSwap* m_modeSwap;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	Flasher* m_flasher;
	FpsMonitor* m_fps;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	/////////////////////////
	// CALLBACK PROTOTYPES //
	/////////////////////////
	//extern int testValue;
	// Callback function called by AntTweakBar to set the sponge recursion level
	//void TW_CALL SetWidthCB(const void *value, void * /*clientData*/);
	// Callback function called by AntTweakBar to get the sponge recursion level
	//void TW_CALL GetWidthCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashTimeCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimeCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimeCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimeCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashTimePercentCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimePercentCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimePercentCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimePercentCB(void *value, void * /*clientData*/);

void TW_CALL SetTotalIntervalCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalCB(void *value, void * /*clientData*/);

void TW_CALL SetTotalIntervalHzCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalHzCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashGridCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashGridCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashBlackCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashBlackCB(void *value, void * /*clientData*/);

void TW_CALL SetGridHeightCB(const void *value, void * /*clientData*/);
void TW_CALL GetGridHeightCB(void *value, void * /*clientData*/);
void TW_CALL SetGridWidthCB(const void *value, void * /*clientData*/);
void TW_CALL GetGridWidthCB(void *value, void * /*clientData*/);

void TW_CALL SetShowVideoCB(const void *value, void * /*clientData*/);
void TW_CALL GetShowVideoCB(void *value, void * /*clientData*/);

void TW_CALL SetPresetModeCB(const void *value, void * /*clientData*/);
void TW_CALL GetPresetModeCB(void *value, void * /*clientData*/);

void TW_CALL SetIsBlackBGCB(const void *value, void * /*clientData*/);
void TW_CALL GetIsBlackBGCB(void *value, void * /*clientData*/);

void TW_CALL SetClockTimeCB(const void *value, void * /*clientData*/);
void TW_CALL GetClockTimeCB(void *value, void * /*clientData*/);

void TW_CALL SetFrameTimeCB(const void *value, void * /*clientData*/);
void TW_CALL GetFrameTimeCB(void *value, void * /*clientData*/);

void TW_CALL SetFPSCB(const void *value, void * /*clientData*/);
void TW_CALL GetFPSCB(void *value, void * /*clientData*/);

void TW_CALL SavePresetsCB(void *clientData);
void TW_CALL ReloadPresetsCB(void *clientData);
void TW_CALL ReturnDefaultPresetCB(void *clientData);

void TW_CALL SetCanFlashCB(const void *value, void * /*clientData*/);
void TW_CALL GetCanFlashCB(void *value, void * /*clientData*/);

void TW_CALL SetEdgeFlashCB(const void *value, void * /*clientData*/);
void TW_CALL GetEdgeFlashCB(void *value, void * /*clientData*/);

void TW_CALL SetGraphicFlashCB(const void *value, void * /*clientData*/);
void TW_CALL GetGraphicFlashCB(void *value, void * /*clientData*/);

void TW_CALL SetShowBlackUnderlayCB(const void *value, void * /*clientData*/);
void TW_CALL GetShowBlackUnderlayCB(void *value, void * /*clientData*/);

void TW_CALL SetBitmapIDOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetBitmapIDOneCB(void *value, void * /*clientData*/);
void TW_CALL SetFlashBitmapOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashBitmapOneCB(void *value, void * /*clientData*/);
void TW_CALL SetShowBitmapOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetShowBitmapOneCB(void *value, void * /*clientData*/);

void TW_CALL SetBitmapIDTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetBitmapIDTwoCB(void *value, void * /*clientData*/);
void TW_CALL SetFlashBitmapTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashBitmapTwoCB(void *value, void * /*clientData*/);
void TW_CALL SetShowBitmapTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetShowBitmapTwoCB(void *value, void * /*clientData*/);

void TW_CALL SetBitmapIDThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetBitmapIDThreeCB(void *value, void * /*clientData*/);
void TW_CALL SetFlashBitmapThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashBitmapThreeCB(void *value, void * /*clientData*/);
void TW_CALL SetShowBitmapThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetShowBitmapThreeCB(void *value, void * /*clientData*/);

void TW_CALL SetFixedXCB(const void *value, void * /*clientData*/);
void TW_CALL GetFixedXCB(void *value, void * /*clientData*/);
void TW_CALL SetFixedYCB(const void *value, void * /*clientData*/);
void TW_CALL GetFixedYCB(void *value, void * /*clientData*/);
void TW_CALL SetUseFixedPosCB(const void *value, void * /*clientData*/);
void TW_CALL GetUseFixedPosCB(void *value, void * /*clientData*/);
void TW_CALL SetSequenceGraphicFlashCB(const void *value, void * /*clientData*/);
void TW_CALL GetSequenceGraphicFlashCB(void *value, void * /*clientData*/);
void TW_CALL SetUsePerImageClockCB(const void *value, void * /*clientData*/);
void TW_CALL GetUsePerImageClockCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashTimeBMPOneCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimeBMPOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimeBMPOneCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimeBMPOneCB(void *value, void * /*clientData*/);
void TW_CALL SetFlashTimePercentBMPOneCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimePercentBMPOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimePercentBMPOneCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimePercentBMPOneCB(void *value, void * /*clientData*/);
void TW_CALL SetTotalIntervalBMPOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalBMPOneCB(void *value, void * /*clientData*/);
void TW_CALL SetTotalIntervalHzBMPOneCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalHzBMPOneCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashTimeBMPTwoCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimeBMPTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimeBMPTwoCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimeBMPTwoCB(void *value, void * /*clientData*/);
void TW_CALL SetFlashTimePercentBMPTwoCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimePercentBMPTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimePercentBMPTwoCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimePercentBMPTwoCB(void *value, void * /*clientData*/);
void TW_CALL SetTotalIntervalBMPTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalBMPTwoCB(void *value, void * /*clientData*/);
void TW_CALL SetTotalIntervalHzBMPTwoCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalHzBMPTwoCB(void *value, void * /*clientData*/);

void TW_CALL SetFlashTimeBMPThreeCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimeBMPThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimeBMPThreeCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimeBMPThreeCB(void *value, void * /*clientData*/);
void TW_CALL SetFlashTimePercentBMPThreeCB(const void *value, void * /*clientData*/);
void TW_CALL SetOffFlashTimePercentBMPThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetFlashTimePercentBMPThreeCB(void *value, void * /*clientData*/);
void TW_CALL GetOffFlashTimePercentBMPThreeCB(void *value, void * /*clientData*/);
void TW_CALL SetTotalIntervalBMPThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalBMPThreeCB(void *value, void * /*clientData*/);
void TW_CALL SetTotalIntervalHzBMPThreeCB(const void *value, void * /*clientData*/);
void TW_CALL GetTotalIntervalHzBMPThreeCB(void *value, void * /*clientData*/);

/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;

#endif