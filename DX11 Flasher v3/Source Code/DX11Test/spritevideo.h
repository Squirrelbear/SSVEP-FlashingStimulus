////////////////////////////////////////////////////////////////////////////////
// Filename: spritevideo.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPRITEVIDEO_H_
#define _SPRITEVIDEO_H_

//#include <sstream>
#include <string>
#include "d3dclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include <iostream>

template <typename T> std::string NumberToString ( T Number );

class SpriteVideo
{
public:
	SpriteVideo();
	~SpriteVideo();

	bool Initialize(D3DClass*, HWND, std::wstring, std::wstring, std::wstring, int, int, double, int, int);
	void Draw(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void Update(int time);
	void Shutdown();

	void SetLoop(bool);
	bool GetLoop();

	void SetPlaying(bool);
	bool GetPlaying();

	double GetClockTime();
	void SetClockTime(double);

private:
	bool m_loop;
	bool m_playing;
	BitmapClass** m_frames;
	TextureShaderClass* m_TextureShader;
	int m_totalFrames;
	int m_curFrame;
	double m_frameTime;
	double m_nextFrameTime;
	double m_clock;
};

#endif