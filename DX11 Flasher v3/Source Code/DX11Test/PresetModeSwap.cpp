#include "PresetModeSwap.h"

PresetModeSwap::PresetModeSwap()
{
	m_storedSettings = 0;
	m_Graphics = 0;
	m_flasher = 0;
}

PresetModeSwap::~PresetModeSwap()
{

}

bool PresetModeSwap::Initialize(GraphicsClass* graphics, Flasher* flasher)
{
	m_Graphics = graphics;
	m_flasher = flasher;
	m_storedSettings = new Mode*[DEFAULT_MODECOUNT];
	m_modeCount = DEFAULT_MODECOUNT;
	m_curMode = 0;
	m_queueMode = m_curMode;

	for(int i = 0; i < m_modeCount; i++)
		m_storedSettings[i] = new Mode();

	if(!loadModeFile())
	{
		for(int i = 0; i < m_modeCount; i++)
			populateDefaults(i);
		if(!saveModeFile())
			return false;
	}

	ApplyMode(m_curMode);
	return true;
}

void PresetModeSwap::Update()
{
	if(m_queueMode != m_curMode)
		ApplyMode(m_queueMode);

	Mode* p_mode = m_storedSettings[m_curMode];
	p_mode->flashTime = m_flasher->GetTimer()->GetPhaseOne();
	p_mode->flashOffTime = m_flasher->GetTimer()->GetPhaseTwo();
	p_mode->flashBlack = m_flasher->GetFlashBlack();
	p_mode->showVideo = m_Graphics->GetShowVideo();
	p_mode->blackBG = m_Graphics->GetIsBlackBG();
	p_mode->useGrid = m_flasher->GetFlashGrid();
	p_mode->gridW = m_flasher->GetGridWidth();
	p_mode->gridH = m_flasher->GetGridHeight();
	p_mode->edgeFlash = m_flasher->GetEdgeFlash();
	p_mode->fixedX = m_flasher->GetFixedX();
	p_mode->fixedY = m_flasher->GetFixedY();
	p_mode->useFixedPos = m_flasher->GetUseFixedPos();

	p_mode->graphicFlash = m_flasher->GetGraphicFlash();
	p_mode->showBlackUnderlay = m_flasher->GetShowBlackUnderlay();
	p_mode->sequenceFlash = m_flasher->GetSequenceGraphicFlash();
	p_mode->perImageClock = m_flasher->GetUsePerImageClock();

	p_mode->bitmapID1 = m_flasher->GetBitmapID(0);
	p_mode->showBitmap1 = m_flasher->GetShowBitmap(0);
	p_mode->flashBitmap1 = m_flasher->GetFlashBitmap(0);
	p_mode->flashTimeBitmap1 = m_flasher->GetBitmapTimer(0)->GetPhaseOne();
	p_mode->flashOffTimeBitmap1 = m_flasher->GetBitmapTimer(0)->GetPhaseTwo();

	p_mode->bitmapID2 = m_flasher->GetBitmapID(1);
	p_mode->showBitmap2 = m_flasher->GetShowBitmap(1);
	p_mode->flashBitmap2 = m_flasher->GetFlashBitmap(1);
	p_mode->flashTimeBitmap2 = m_flasher->GetBitmapTimer(1)->GetPhaseOne();
	p_mode->flashOffTimeBitmap2 = m_flasher->GetBitmapTimer(1)->GetPhaseTwo();

	p_mode->bitmapID3 = m_flasher->GetBitmapID(2);
	p_mode->showBitmap3 = m_flasher->GetShowBitmap(2);
	p_mode->flashBitmap3 = m_flasher->GetFlashBitmap(2);
	p_mode->flashTimeBitmap3 = m_flasher->GetBitmapTimer(2)->GetPhaseOne();
	p_mode->flashOffTimeBitmap3 = m_flasher->GetBitmapTimer(2)->GetPhaseTwo();

}

bool PresetModeSwap::ApplyMode(int mode)
{
	if(mode < 0 || mode >= DEFAULT_MODECOUNT)
		return false;

	m_curMode = mode;
	Mode* p_mode = m_storedSettings[m_curMode];
	int flashTime = p_mode->flashTime;
	int flashOffTime = p_mode->flashOffTime;
	m_flasher->GetTimer()->SetPhases(flashTime, flashOffTime);
	bool flashBlack = p_mode->flashBlack;
	m_flasher->SetFlashBlack(flashBlack);
	bool showVideo = p_mode->showVideo;
	m_Graphics->SetShowVideo(showVideo);
	bool blackBG = p_mode->blackBG;
	m_Graphics->SetIsBlackBG(blackBG);
	bool useGrid = p_mode->useGrid;
	m_flasher->SetFlashGrid(p_mode->useGrid);
	int gridW = p_mode->gridW;
	int gridH = p_mode->gridH;
	m_flasher->SetGridWidth(gridW);
	m_flasher->SetGridHeight(gridH);
	m_flasher->SetEdgeFlash(p_mode->edgeFlash);
	m_flasher->SetFixedX(p_mode->fixedX);
	m_flasher->SetFixedY(p_mode->fixedY);
	m_flasher->SetUseFixedPos(p_mode->useFixedPos);

	m_flasher->SetGraphicFlash(p_mode->graphicFlash);
	m_flasher->SetShowBlackUnderlay(p_mode->showBlackUnderlay);
	m_flasher->SetSequenceGraphicFlash(p_mode->sequenceFlash);
	m_flasher->SetUsePerImageClock(p_mode->perImageClock);

	m_flasher->SetBitmapID(0, p_mode->bitmapID1);
	m_flasher->SetFlashBitmap(0, p_mode->flashBitmap1);
	m_flasher->SetShowBitmap(0, p_mode->showBitmap1);
	m_flasher->GetBitmapTimer(0)->SetPhases(p_mode->flashTimeBitmap1, p_mode->flashOffTimeBitmap1);
	m_flasher->SetBitmapID(1, p_mode->bitmapID2);
	m_flasher->SetFlashBitmap(1, p_mode->flashBitmap2);
	m_flasher->SetShowBitmap(1, p_mode->showBitmap2);
	m_flasher->GetBitmapTimer(1)->SetPhases(p_mode->flashTimeBitmap2, p_mode->flashOffTimeBitmap2);
	m_flasher->SetBitmapID(2, p_mode->bitmapID3);
	m_flasher->SetFlashBitmap(2, p_mode->flashBitmap3);
	m_flasher->SetShowBitmap(2, p_mode->showBitmap3);
	m_flasher->GetBitmapTimer(2)->SetPhases(p_mode->flashTimeBitmap3, p_mode->flashOffTimeBitmap3);

	return true;
}

bool PresetModeSwap::QueueMode(int mode)
{
	if(mode < 0 || mode >= DEFAULT_MODECOUNT)
		return false;

	m_queueMode = mode;

	return true;
}

int PresetModeSwap::GetMode()
{
	return m_curMode;
}

void PresetModeSwap::Shutdown()
{
	saveModeFile();
	for(int i = 0; i < m_modeCount; i++)
		delete m_storedSettings[i];
	delete[] m_storedSettings;
	m_storedSettings = 0;

	// let the system class shut these down; just clear the variables
	m_Graphics = 0;
	m_flasher = 0;
}

bool PresetModeSwap::loadModeFile()
{
	ifstream myfile ("modes.dat");
	if(!myfile.is_open())
		return false;

	int version;
	myfile >> version;
	if(version < MIN_PRESETS_FILEVERSION)
		return false;

	for(int i = 0; i < m_modeCount; i++)
	{
		Mode* p_mode = m_storedSettings[i];
		myfile >> p_mode->flashTime;
		myfile >> p_mode->flashOffTime;
		myfile >> p_mode->flashBlack;
		myfile >> p_mode->showVideo;
		myfile >> p_mode->blackBG;
		myfile >> p_mode->useGrid;
		myfile >> p_mode->gridW;
		myfile >> p_mode->gridH;
		myfile >> p_mode->edgeFlash;
		myfile >> p_mode->fixedX;
		myfile >> p_mode->fixedY;
		myfile >> p_mode->useFixedPos;

		myfile >> p_mode->graphicFlash;
		myfile >> p_mode->showBlackUnderlay;
		myfile >> p_mode->sequenceFlash;
		myfile >> p_mode->perImageClock;

		myfile >> p_mode->bitmapID1;
		myfile >> p_mode->flashBitmap1;
		myfile >> p_mode->showBitmap1;
		myfile >> p_mode->flashTimeBitmap1;
		myfile >> p_mode->flashOffTimeBitmap1;

		myfile >> p_mode->bitmapID2;
		myfile >> p_mode->flashBitmap2;
		myfile >> p_mode->showBitmap2;
		myfile >> p_mode->flashTimeBitmap2;
		myfile >> p_mode->flashOffTimeBitmap2;

		myfile >> p_mode->bitmapID3;
		myfile >> p_mode->flashBitmap3;
		myfile >> p_mode->showBitmap3;
		myfile >> p_mode->flashTimeBitmap3;
		myfile >> p_mode->flashOffTimeBitmap3;
	}

	return true;
}

bool PresetModeSwap::saveModeFile()
{
	std::ofstream myfile;
	myfile.open ("modes.dat");
	if(!myfile.is_open())
		return false;

	myfile << PRESETS_FILEVERSION << "\n";

	for(int i = 0; i < m_modeCount; i++)
	{
		Mode* p_mode = m_storedSettings[i];
		myfile << p_mode->flashTime << " ";
		myfile << p_mode->flashOffTime << " ";
		myfile << p_mode->flashBlack << " ";
		myfile << p_mode->showVideo << " ";
		myfile << p_mode->blackBG << " ";
		myfile << p_mode->useGrid << " ";
		myfile << p_mode->gridW << " ";
		myfile << p_mode->gridH << " ";
		myfile << p_mode->edgeFlash << " ";
		myfile << p_mode->fixedX << " ";
		myfile << p_mode->fixedY << " ";
		myfile << p_mode->useFixedPos << " ";

		myfile << p_mode->graphicFlash << " ";
		myfile << p_mode->showBlackUnderlay << " ";
		myfile << p_mode->sequenceFlash << " ";
		myfile << p_mode->perImageClock << " ";

		myfile << p_mode->bitmapID1 << " ";
		myfile << p_mode->flashBitmap1 << " ";
		myfile << p_mode->showBitmap1 << " ";
		myfile << p_mode->flashTimeBitmap1 << " ";
		myfile << p_mode->flashOffTimeBitmap1 << " ";

		myfile << p_mode->bitmapID2 << " ";
		myfile << p_mode->flashBitmap2 << " ";
		myfile << p_mode->showBitmap2 << " ";
		myfile << p_mode->flashTimeBitmap2 << " ";
		myfile << p_mode->flashOffTimeBitmap2 << " ";

		myfile << p_mode->bitmapID3 << " ";
		myfile << p_mode->flashBitmap3 << " ";
		myfile << p_mode->showBitmap3 << " ";
		myfile << p_mode->flashTimeBitmap3 << " ";
		myfile << p_mode->flashOffTimeBitmap3 << "\n";
	}
  	myfile.close();
  	return true;
}

void PresetModeSwap::populateDefaults(int mode)
{
	Mode* p_mode = m_storedSettings[mode];
	p_mode->flashTime = 15;
	p_mode->flashOffTime = 75;
	p_mode->flashBlack = true;
	p_mode->showVideo = true;
	p_mode->blackBG = false;
	p_mode->useGrid = false;
	p_mode->gridW = 8;
	p_mode->gridH = 8;
	p_mode->edgeFlash = false;
	p_mode->fixedX = 3;
	p_mode->fixedY = 3;
	p_mode->useFixedPos = false;

	p_mode->graphicFlash = false;
	p_mode->showBlackUnderlay = false;
	p_mode->sequenceFlash = false;
	p_mode->perImageClock = false;

	p_mode->bitmapID1 = 0;
	p_mode->showBitmap1 = false;
	p_mode->flashBitmap1 = true;
	p_mode->flashTimeBitmap1 = 15;
	p_mode->flashOffTimeBitmap1 = 75;

	p_mode->bitmapID2 = 1;
	p_mode->showBitmap2 = false;
	p_mode->flashBitmap2 = true;
	p_mode->flashTimeBitmap2 = 15;
	p_mode->flashOffTimeBitmap2 = 75;

	p_mode->bitmapID3 = 2;
	p_mode->showBitmap3 = false;
	p_mode->flashBitmap3 = true;
	p_mode->flashTimeBitmap3 = 15;
	p_mode->flashOffTimeBitmap3 = 75;
}