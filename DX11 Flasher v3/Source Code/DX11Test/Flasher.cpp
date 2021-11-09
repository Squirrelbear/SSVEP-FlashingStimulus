#include "Flasher.h"

Flasher::Flasher()
{
	m_timer = 0;
	m_bitmap = 0;
	m_bitmapWhite = 0;
	m_bitmapBlack = 0;
	m_TextureShader = 0;

	m_bitmapsInUse = 0;
	m_flashGraphicBitmaps = 0;
	m_flashGraphics = 0;
	m_bitmapBlackBG = 0;
}

Flasher::Flasher(const Flasher&)
{
}

Flasher::~Flasher()
{
}


bool Flasher::Initialize(int screenWidth, int screenHeight, GraphicsClass* graphics, HWND hwnd)
{
	m_graphics = graphics;

	// configure display area
	m_displayRect = SimpleRect();
	m_displayRect.width = screenWidth;
	m_displayRect.height = screenHeight;
	m_displayRect.x = 0;
	m_displayRect.y = 0;

	// load bitmaps
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	bool result = m_TextureShader->Initialize(m_graphics->GetD3D()->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_bitmapWhite = new BitmapClass;
	if(!m_bitmapWhite)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_bitmapWhite->Initialize(m_graphics->GetD3D()->GetDevice(), screenWidth, screenHeight, L"../Resources/white.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	m_bitmapWhite->SetPosition(0,0);
	m_bitmapWhite->SetDimensions(screenWidth, screenHeight);

	// Create the bitmap object.
	m_bitmapBlack = new BitmapClass;
	if(!m_bitmapBlack)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_bitmapBlack->Initialize(m_graphics->GetD3D()->GetDevice(), screenWidth, screenHeight, L"../Resources/black.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	m_bitmapBlack->SetPosition(0,0);
	m_bitmapBlack->SetDimensions(screenWidth, screenHeight);

	// initialize all other variables
	SetFlashBlack(true);
	SetFlashGrid(false);
	SetFlash(true);
	SetGridHeight(8);
	SetGridWidth(8);
	SetEdgeFlash(false);
	SetCanFlash(true);
	m_curEdgeFlashCell = 0;
	SetGraphicFlash(true);
	SetShowBlackUnderlay(false);
	SetFixedX(0);
	SetFixedY(0);
	SetUseFixedPos(false);
	SetSequenceGraphicFlash(false);
	SetUsePerImageClock(false);
	m_curFlashIndex = 0;

	m_timer = new AdaptiveTimer;
	if(!m_timer)
	{
		return false;
	}

	m_timer->Initialize(0.1f, 30);

	m_flashGraphics = new FlashGraphic[3];

	int graphicWidth = screenWidth/4;
	int graphicHeight = screenHeight/4;
	int posX = 0;
	int posY = 3 * screenHeight / 4;

	if(!loadFlashGraphics(graphicWidth, graphicHeight, screenWidth, screenHeight))
	{
		return false;
	}

	m_flashGraphics[0] = FlashGraphic(posX, posY);
	m_flashGraphics[0].m_bitmap = m_flashGraphicBitmaps[0];
	m_flashGraphics[0].bitmapID = 0;
	m_bitmapsInUse[0] = true;
	m_flashGraphics[0].m_bitmap->SetPosition(posX,posY);
	m_flashGraphics[0].m_flashGraphicShow = false;

	m_flashGraphics[0].m_perImageClock = new AdaptiveTimer;
	if(!m_flashGraphics[0].m_perImageClock)
	{
		return false;
	}

	m_flashGraphics[0].m_perImageClock->Initialize(0.16f, 16);


	posX = (screenWidth - graphicWidth) / 2;
	m_flashGraphics[1] = FlashGraphic(posX, posY);
	m_flashGraphics[1].m_bitmap = m_flashGraphicBitmaps[1];
	m_flashGraphics[1].bitmapID = 1;
	m_bitmapsInUse[1] = true;
	m_flashGraphics[1].m_bitmap->SetPosition(posX,posY);
	m_flashGraphics[1].m_flashGraphicShow = false;

	m_flashGraphics[1].m_perImageClock = new AdaptiveTimer;
	if(!m_flashGraphics[1].m_perImageClock)
	{
		return false;
	}

	m_flashGraphics[1].m_perImageClock->Initialize(0.16f, 16);

	posX = screenWidth - graphicWidth;
	m_flashGraphics[2] = FlashGraphic(posX, posY);
	m_flashGraphics[2].m_bitmap = m_flashGraphicBitmaps[2];
	m_flashGraphics[2].bitmapID = 2;
	m_bitmapsInUse[2] = true;
	m_flashGraphics[2].m_bitmap->SetPosition(posX,posY);
	m_flashGraphics[2].m_flashGraphicShow = false;

	m_flashGraphics[2].m_perImageClock = new AdaptiveTimer;
	if(!m_flashGraphics[2].m_perImageClock)
	{
		return false;
	}

	m_flashGraphics[2].m_perImageClock->Initialize(0.16f, 16);

	m_bitmapBlackBG = new BitmapClass;
	if(!m_bitmapBlackBG)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_bitmapBlackBG->Initialize(m_graphics->GetD3D()->GetDevice(), screenWidth, screenHeight, L"../Resources/black.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	m_bitmapBlackBG->SetPosition(0,screenHeight - graphicHeight);
	m_bitmapBlackBG->SetDimensions(screenWidth, graphicHeight);


	return true;
}

void Flasher::Shutdown()
{

	// Release the bitmap object.
	if(m_bitmap)
	{
		m_bitmap->Shutdown();
		delete m_bitmap;
		m_bitmap = 0;
	}

	if(m_flashGraphicBitmaps)
	{
		for(int i = 0; i < m_flashGraphicCount; i++)
		{
			if(m_flashGraphicBitmaps[i])
			{
				m_flashGraphicBitmaps[i]->Shutdown();
				delete m_flashGraphicBitmaps[i];
				m_flashGraphicBitmaps[i] = 0;
			}
		}

		delete[] m_flashGraphicBitmaps;
		m_flashGraphicBitmaps = 0;
	}

	if(m_bitmapsInUse)
	{
		delete[] m_bitmapsInUse;
		m_bitmapsInUse = 0;
	}

	if(m_bitmapBlackBG)
	{
		m_bitmapBlackBG->Shutdown();
		delete m_bitmapBlackBG;
		m_bitmapBlackBG = 0;
	}

	if(m_flashGraphics)
	{
		for(int i = 0; i < 3; i++)
		{
			if(m_flashGraphics[i].m_perImageClock)
			{
				delete m_flashGraphics[i].m_perImageClock;
				m_flashGraphics[i].m_perImageClock = 0;
			}
		}

		delete[] m_flashGraphics;
		m_flashGraphics = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if(m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}
}


void Flasher::Update(int time)
{
	if(m_graphicFlash && m_usePerImageClock)
	{
		SetFlash(false);

		for(int i = 0; i < 3; i++)
		{
			m_flashGraphics[i].m_perImageClock->Update(time);
			if(m_flashGraphics[i].m_perImageClock->IsEdgeTriggered())
			{
				m_flashGraphics[i].m_flashGraphicShow = m_flashGraphics[i].m_perImageClock->IsPhaseOne();
			}
		}
		return;
	}

	m_timer->Update(time);
	if(m_timer->IsEdgeTriggered())
	{
		SetFlash(m_timer->IsPhaseOne());

		if(m_edgeFlash && !m_timer->IsPhaseOne())
			m_curEdgeFlashCell = GetNextEdge();

		if(m_sequenceGraphicFlash && m_timer->IsPhaseOne())
		{
			m_flashGraphics[m_curFlashIndex].m_flashGraphicShow = false;

			int index = -1;
			for(m_curFlashIndex = m_curFlashIndex+1; m_curFlashIndex < 3; m_curFlashIndex++)
			{
				if(m_flashGraphics[m_curFlashIndex].flash)
				{
					m_flashGraphics[m_curFlashIndex].flash = true;
					index = m_curFlashIndex;
					break;
				}
			}

			if(index == -1)
			{
				for(m_curFlashIndex = 0; m_curFlashIndex < 3; m_curFlashIndex++)
				{
					if(m_flashGraphics[m_curFlashIndex].flash)
					{
						m_flashGraphics[m_curFlashIndex].flash = true;
						index = m_curFlashIndex;
						break;
					}
				}
			}
		}
		else if(m_sequenceGraphicFlash)
		{
			for(int i = 0; i < 3; i++)
			{
				m_flashGraphics[i].m_flashGraphicShow = false;
			}
		}
	}

	if(m_edgeFlash)
	{
		POINT p = GetEdgeFlashCoords();
		m_bitmap->SetPosition((p.x / m_cellWidth) * m_cellWidth,(p.y / m_cellHeight) * m_cellHeight);
		m_bitmap->SetDimensions(m_cellWidth, m_cellHeight);
	}
	else if(m_flashGrid)
	{
		int x,y;

		if(m_useFixedPos)
		{
			x = m_fixedX;
			y = m_fixedY;
		}
		else
		{
			POINT p;
			GetCursorPos(&p);
			x = (p.x / m_cellWidth);
			y = (p.y / m_cellHeight);
		}
		
		m_bitmap->SetPosition(x * m_cellWidth,y * m_cellHeight);
		m_bitmap->SetDimensions(m_cellWidth, m_cellHeight);
	}
	else
	{
		m_bitmap->SetPosition(0,0);
		m_bitmap->SetDimensions(m_displayRect.width, m_displayRect.height);
	}
}

bool Flasher::Render(ID3D11DeviceContext* context)
{
	if(m_showBlackUnderlay && m_graphicFlash)
	{
		bool result = m_bitmapBlackBG->Render(context);
		if(!result)
		{
			return false;
		}

		D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
		m_graphics->GetCamera()->GetViewMatrix(viewMatrix);
		m_graphics->GetD3D()->GetWorldMatrix(worldMatrix);
		m_graphics->GetD3D()->GetOrthoMatrix(orthoMatrix);

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(context, m_bitmapBlackBG->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmapBlackBG->GetTexture());
		if(!result)
		{
			return false;
		}
	}

	if(m_graphicFlash)
	{
		for(int i = 0; i < 3; i++)
		{
			bool showGraphic = false;
			// if the show option is enabled always show
			if(m_flashGraphics[i].show) 
				showGraphic = true;
			// if the flashing is using default graphic flash
			if(!m_usePerImageClock && !m_sequenceGraphicFlash
				&& m_flashGraphics[i].flash && GetFlash())
				showGraphic = true;
			// if the flashing is clocked
			if(m_usePerImageClock && m_flashGraphics[i].flash
				 && m_flashGraphics[i].m_flashGraphicShow)
				 showGraphic = true;
			// if the flashing is sequenced
			if(m_curFlashIndex == i && m_sequenceGraphicFlash 
				&& GetFlash())
				showGraphic = true;

			/*if(m_flashGraphics[i].show || 
				(m_flashGraphics[i].flash 
					&& ((GetFlash() ) || 
						((m_usePerImageClock || m_sequenceGraphicFlash) 
								&& m_flashGraphics[i].m_flashGraphicShow))))*/
			if(showGraphic)
			{
				bool result = m_flashGraphics[i].m_bitmap->Render(context);
				if(!result)
				{
					return false;
				}

				D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
				m_graphics->GetCamera()->GetViewMatrix(viewMatrix);
				m_graphics->GetD3D()->GetWorldMatrix(worldMatrix);
				m_graphics->GetD3D()->GetOrthoMatrix(orthoMatrix);

				// Render the bitmap with the texture shader.
				result = m_TextureShader->Render(context, m_flashGraphics[i].m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_flashGraphics[i].m_bitmap->GetTexture());
				if(!result)
				{
					return false;
				}
			}
		}
	}
	else if(m_canFlash && GetFlash() && m_bitmap)
	{
		bool result = m_bitmap->Render(context);
		if(!result)
		{
			return false;
		}

		D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
		m_graphics->GetCamera()->GetViewMatrix(viewMatrix);
		m_graphics->GetD3D()->GetWorldMatrix(worldMatrix);
		m_graphics->GetD3D()->GetOrthoMatrix(orthoMatrix);

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(context, m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmap->GetTexture());
		if(!result)
		{
			return false;
		}
	}

	return true;
}


void Flasher::SetFlashGrid(bool flashGrid)
{
	m_flashGrid = flashGrid;
}

bool Flasher::GetFlashGrid()
{
	return m_flashGrid;
}


void Flasher::SetFlash(bool flash)
{
	m_flash = flash;
}

bool Flasher::GetFlash()
{
	return m_flash;
}


void Flasher::SetFlashBlack(bool black)
{
	if(m_black != black)
	{
		m_black = black;

		if(m_black)
			m_bitmap = m_bitmapBlack;
		else
			m_bitmap = m_bitmapWhite;
	}
}

bool Flasher::GetFlashBlack()
{
	return m_black;
}


void Flasher::SetGridHeight(int gridHeight)
{
	m_gridHeight = gridHeight;
	m_cellHeight = m_displayRect.height / gridHeight;
}

void Flasher::SetGridWidth(int gridWidth)
{
	m_gridWidth = gridWidth;
	m_cellWidth = m_displayRect.width / gridWidth;
}

int Flasher::GetGridHeight()
{
	return m_gridHeight;
}

int Flasher::GetGridWidth()
{
	return m_gridWidth;
}

AdaptiveTimer* Flasher::GetTimer()
{
	return m_timer;
}

bool Flasher::GetCanFlash()
{
	return m_canFlash;
}

void Flasher::SetCanFlash(bool canFlash)
{
	m_canFlash = canFlash;
}

bool Flasher::GetEdgeFlash()
{
	return m_edgeFlash;
}

void Flasher::SetEdgeFlash(bool edgeFlash)
{
	m_edgeFlash = edgeFlash;
}


void Flasher::SetBitmapID(int graphicID, int bitmapID)
{

	int prevGraphic = m_flashGraphics[graphicID].bitmapID;
	m_bitmapsInUse[prevGraphic] = false;

	int newTarget = -1;
	// if the new id is in use already find the next id in the direction
	if(m_bitmapsInUse[bitmapID])
	{
		if(bitmapID > prevGraphic)
		{
			// go forward searching
			int search = prevGraphic+1;
			while(newTarget == -1 && search < m_flashGraphicCount)
			{
				if(!m_bitmapsInUse[search])
				{
					newTarget = search;
					break;
				}
				search++;
			}

			search = 0;
			while(newTarget == -1 && search < prevGraphic)
			{
				if(!m_bitmapsInUse[search])
				{
					newTarget = search;
					break;
				}
				search++;
			}
		}
		else
		{
			int search = prevGraphic-1;
			while(newTarget == -1 && search > 0)
			{
				if(!m_bitmapsInUse[search])
				{
					newTarget = search;
					break;
				}
				search--;
			}

			search = m_flashGraphicCount-1;
			while(newTarget == -1 && search > prevGraphic)
			{
				if(!m_bitmapsInUse[search])
				{
					newTarget = search;
					break;
				}
				search--;
			}

		}

	}
	else 
	{
		newTarget = bitmapID;
	}

	if(newTarget == -1)
	{
		// no new bitmap found to swap to...
		m_bitmapsInUse[prevGraphic] = true;
		return;
	}

	m_bitmapsInUse[newTarget] = true;
	m_flashGraphics[graphicID].bitmapID = newTarget;
	m_flashGraphics[graphicID].m_bitmap = m_flashGraphicBitmaps[newTarget];

	m_flashGraphics[graphicID].m_bitmap->SetPosition(m_flashGraphics[graphicID].location.x,
													  m_flashGraphics[graphicID].location.y);

}

int Flasher::GetBitmapID(int graphicID)
{
	return m_flashGraphics[graphicID].bitmapID;
}

void Flasher::SetShowBitmap(int graphicID, bool showBitmap )
{
	m_flashGraphics[graphicID].show = 	showBitmap;
}

bool Flasher::GetShowBitmap(int graphicID)
{
	return m_flashGraphics[graphicID].show;
}

void Flasher::SetFlashBitmap(int graphicID, bool flashGraphic)
{
	m_flashGraphics[graphicID].flash = flashGraphic;
}

bool Flasher::GetFlashBitmap(int graphicID)
{
	return m_flashGraphics[graphicID].flash;
}

void Flasher::SetGraphicFlash(bool flash)
{
	m_graphicFlash = flash;
}

bool Flasher::GetGraphicFlash()
{
	return m_graphicFlash;
}

void Flasher::SetShowBlackUnderlay(bool show)
{
	m_showBlackUnderlay = show;
}

bool Flasher::GetShowBlackUnderlay()
{
	return m_showBlackUnderlay;
}

int Flasher::GetFixedX()
{
	return m_fixedX;
}

int Flasher::GetFixedY()
{
	return m_fixedY;
}

void Flasher::SetFixedX(int newX)
{
	m_fixedX = newX;
}

void Flasher::SetFixedY(int newY)
{
	m_fixedY = newY;
}

bool Flasher::GetUseFixedPos()
{
	return m_useFixedPos;
}

void Flasher::SetUseFixedPos(bool useFixedPos)
{
	m_useFixedPos = useFixedPos;
}

bool Flasher::GetSequenceGraphicFlash()
{
	return m_sequenceGraphicFlash;
}

void Flasher::SetSequenceGraphicFlash(bool sequenceGraphicFlash)
{
	m_sequenceGraphicFlash = sequenceGraphicFlash;
}

bool Flasher::GetUsePerImageClock()
{
	return m_usePerImageClock;
}

void Flasher::SetUsePerImageClock(bool usePerImageClock)
{
	m_usePerImageClock = usePerImageClock;
}

AdaptiveTimer* Flasher::GetBitmapTimer(int graphicID)
{
	return m_flashGraphics[graphicID].m_perImageClock;
}

bool Flasher::loadFlashGraphics(int graphicWidth, int graphicHeight, int screenWidth, int screenHeight)
{
	std::ifstream myfile ("flashgraphics.dat");
	if(!myfile.is_open())
		return false;

	myfile >> m_flashGraphicCount;
	if(m_flashGraphicCount < 4)
		return false;

	m_flashGraphicBitmaps = new BitmapClass*[m_flashGraphicCount];
	m_bitmapsInUse = new bool[m_flashGraphicCount];

	for(int i = 0; i < m_flashGraphicCount; i++)
	{
		m_bitmapsInUse[i] = false;
	}

	std::string fileName;
	std::getline(myfile, fileName); // get rid of the empty line
	for(int i = 0; i < m_flashGraphicCount; i++)
	{
		std::getline(myfile, fileName);
		fileName = "../Resources/" + fileName;
		// null-call to get the size
		size_t needed = ::mbstowcs(NULL,&fileName[0],fileName.length());

		// allocate
		std::wstring output;
		output.resize(needed);

		// real call
		::mbstowcs(&output[0],&fileName[0],fileName.length());
		
		WCHAR wstrfileName[512];
		wcscpy_s(wstrfileName, output.c_str());


		m_flashGraphicBitmaps[i] = new BitmapClass;
		if(!m_flashGraphicBitmaps[i])
		{
			return false;
		}

		// Initialize the bitmap object.
		bool result = m_flashGraphicBitmaps[i]->Initialize(m_graphics->GetD3D()->GetDevice(), screenWidth, screenHeight, wstrfileName, 256, 256);
		if(!result)
		{
			return false;
		}
		m_flashGraphicBitmaps[i]->SetPosition(0,0);
		m_flashGraphicBitmaps[i]->SetDimensions(graphicWidth, graphicHeight);
	}

	myfile.close();

	return true;
}

POINT Flasher::GetEdgeFlashCoords()
{
	POINT p = POINT();
	// this indicates the cell is invalid
	if(m_curEdgeFlashCell > m_gridWidth + m_gridHeight - 1)
		m_curEdgeFlashCell = 0;

	if(m_curEdgeFlashCell < m_gridHeight)
	{
		p.x = (m_gridWidth - 1) * m_cellWidth;
		p.y = (m_curEdgeFlashCell) * m_cellHeight;
	}
	else
	{
		p.x = (m_gridWidth - (m_curEdgeFlashCell - m_gridHeight) - 1) * m_cellWidth;
		p.y = (m_gridHeight - 1) * m_cellHeight;
	}

	return p;
}

int Flasher::GetNextEdge()
{
	if(m_curEdgeFlashCell > m_gridWidth + m_gridHeight - 1)
		return 0;
	else
		return m_curEdgeFlashCell+1;
}
