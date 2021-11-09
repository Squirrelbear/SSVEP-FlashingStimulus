////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "AntTweakBar.h"

// Video playback
#include "spritevideo.h"

#include <fstream>
#include <io.h>

//class SpriteVideo;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool BeginFrame();
	bool EndFrame();
	bool Update(LONGLONG);
	
	ID3D11Device* GetDevice();
	D3DClass* GetD3D();
	CameraClass* GetCamera();

	void saveCardDetails();

	// video methods
	bool InitPlayer(HWND hwnd);
	void DrawVideo(HWND hwnd);
	SpriteVideo* GetVideoPlayer();

	bool GetShowVideo();
	void SetShowVideo(bool);
	//BitmapClass* getBitmap();

	bool GetIsBlackBG();
	void SetIsBlackBG(bool);

private:
	bool Render(float);
	bool Frame();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	HWND hwnd;
	//TextureShaderClass* m_TextureShader;
	//BitmapClass* m_Bitmap;
	SpriteVideo* m_Video;

	bool m_showVideo;
	bool m_isBlackBG;
};
#endif