#ifndef _PRESETMODESWAP_H_
#define _PRESETMODESWAP_H_

#include <iostream>
#include <fstream>
#include "Flasher.h"

#define DEFAULT_MODECOUNT 20
#define MIN_PRESETS_FILEVERSION 4
#define PRESETS_FILEVERSION 4

struct Mode {
	int flashTime;
	int flashOffTime;
	bool flashBlack;
	bool showVideo;
	bool blackBG;
	bool useGrid;
	int gridW, gridH;
	bool edgeFlash;
	int fixedX, fixedY;
	bool useFixedPos;

	bool graphicFlash;
	bool showBlackUnderlay;
	bool sequenceFlash;
	bool perImageClock;
	int bitmapID1,bitmapID2, bitmapID3;
	bool showBitmap1, showBitmap2, showBitmap3;
	bool flashBitmap1, flashBitmap2, flashBitmap3;
	int flashTimeBitmap1, flashTimeBitmap2, flashTimeBitmap3;
	int flashOffTimeBitmap1, flashOffTimeBitmap2, flashOffTimeBitmap3;
};

class PresetModeSwap
{
public:
	PresetModeSwap();
	~PresetModeSwap();

	bool Initialize(GraphicsClass*,Flasher*);
	void Update();
	bool ApplyMode(int);
	bool QueueMode(int);
	int GetMode();
	void Shutdown();

	bool loadModeFile();
	bool saveModeFile();

	void populateDefaults(int);

private:
	GraphicsClass* m_Graphics;
	Flasher* m_flasher;
	Mode** m_storedSettings;
	int m_modeCount;
	int m_curMode;
	int m_queueMode;
};

#endif