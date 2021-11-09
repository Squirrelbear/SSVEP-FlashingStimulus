////////////////////////////////////////////////////////////////////////////////
// Filename: Flasher.h.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FLASHERCLASS_H_
#define _FLASHERCLASS_H_

#include "graphicsclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "AdaptiveTimer.h"

#include <Windows.h>
#include <cstdlib>

/////////////
// GLOBALS //
/////////////

struct SimpleRect
{
	int x, y, width, height;
};

struct FlashGraphic
{
	int bitmapID;
	BitmapClass* m_bitmap;
	SimpleRect location;
	bool flash;
	bool show;

	AdaptiveTimer* m_perImageClock;
	bool m_flashGraphicShow;

	FlashGraphic()
	{
		bitmapID = -1;
		m_bitmap = 0;
		location = SimpleRect();
		location.x = 0;
		location.y = 0;
		location.width = 0;
		location.height = 0;
		flash = false;
		show = true;
	}

	FlashGraphic(int x, int y)
	{
		bitmapID = -1;
		m_bitmap = 0;
		location = SimpleRect();
		location.x = x;
		location.y = y;
		location.width = 0;
		location.height = 0;
		flash = true;
		show = false;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Flasher
////////////////////////////////////////////////////////////////////////////////
class Flasher
{
public:
	Flasher();
	Flasher(const Flasher&);
	~Flasher();

	bool Initialize(int, int, GraphicsClass*, HWND);
	void Shutdown();

	void Update(int);
	bool Render(ID3D11DeviceContext*);

	void SetFlashGrid(bool);
	bool GetFlashGrid();

	void SetFlash(bool);
	bool GetFlash();

	void SetFlashBlack(bool);
	bool GetFlashBlack();

	void SetGridHeight(int);
	void SetGridWidth(int);
	int GetGridHeight();
	int GetGridWidth();

	AdaptiveTimer* GetTimer();

	bool GetCanFlash();
	void SetCanFlash(bool);
	bool GetEdgeFlash();
	void SetEdgeFlash(bool);

	void SetBitmapID(int, int);
	int GetBitmapID(int);

	void SetShowBitmap(int, bool );
	bool GetShowBitmap(int);

	void SetFlashBitmap(int, bool);
	bool GetFlashBitmap(int);

	void SetGraphicFlash(bool);
	bool GetGraphicFlash();

	void SetShowBlackUnderlay(bool);
	bool GetShowBlackUnderlay();

	int GetFixedX();
	int GetFixedY();
	void SetFixedX(int);
	void SetFixedY(int);

	bool GetUseFixedPos();
	void SetUseFixedPos(bool);

	bool GetSequenceGraphicFlash();
	void SetSequenceGraphicFlash(bool);

	bool GetUsePerImageClock();
	void SetUsePerImageClock(bool);

	AdaptiveTimer* GetBitmapTimer(int);

private:
	bool loadFlashGraphics(int, int, int, int);
	POINT GetEdgeFlashCoords();
	int GetNextEdge();

private:
	GraphicsClass* m_graphics;

	// m_color: the colour that is to be flashed (actually shades the bitmap this colour)
	float m_color[4];
	// temporary variable to make black or white
	bool m_black;

	// m_flash: is the flashing activated
	bool m_flash;
	// m_flashGrid: if true cells only will flash, if false the entire screen will flash
	bool m_flashGrid;

	bool m_canFlash;
	bool m_edgeFlash;
	int m_curEdgeFlashCell;

	// m_bitmap: The graphic that is to be displayed
	BitmapClass* m_bitmap;
	TextureShaderClass* m_TextureShader;
	// Preloaded bitmaps that can be used as the preset colours for now
	BitmapClass* m_bitmapWhite;
	BitmapClass* m_bitmapBlack;
	bool m_showBlackUnderlay;
	BitmapClass* m_bitmapBlackBG;
	// m_displayRect: the region used to display elements in. Used to calculate grid cells
	SimpleRect m_displayRect;
	// m_gridWidth, m_gridHeight: The width and height in number of cells that the screen is divided into
	int m_gridWidth, m_gridHeight;
	// m_cellWidth, m_cellHeigt: The dimensions that all the cells are defined as
	int m_cellWidth, m_cellHeight;

	int curCellX, curCellY;

	AdaptiveTimer* m_timer;

	bool m_graphicFlash;
	int m_flashGraphicCount;
	bool* m_bitmapsInUse;
	BitmapClass** m_flashGraphicBitmaps;
	FlashGraphic* m_flashGraphics;


	int m_fixedX, m_fixedY;
	bool m_useFixedPos;

	bool m_sequenceGraphicFlash;
	int m_curFlashIndex;

	bool m_usePerImageClock;
};

#endif