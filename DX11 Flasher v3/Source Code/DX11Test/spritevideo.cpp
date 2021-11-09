#include "spritevideo.h"

SpriteVideo::SpriteVideo()
{
	m_frames = 0;
	m_TextureShader = 0;
	m_loop = true;
	m_playing = false;
}

SpriteVideo::~SpriteVideo()
{
}

bool SpriteVideo::Initialize(D3DClass* d3d, HWND hwnd, std::wstring folder, std::wstring namePrefix, std::wstring namePostfix, int startNo, int endNo, double frameTime, int width, int height)
{
	m_frameTime = frameTime;

	if(startNo > endNo)
		return false;

	m_totalFrames = endNo - startNo + 1;
	m_frames = new BitmapClass* [m_totalFrames];

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	bool result = m_TextureShader->Initialize(d3d->GetDevice(), hwnd);
	if(!result)
	{
		//MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	for(int i = 0; i < m_totalFrames; i++)
	{
//		std::cout << "ID: " << i << std::endl;
		std::wstring fName;
		WCHAR vOut[4];
		_itow(i, vOut, 10);
		fName = folder + namePrefix + vOut + namePostfix;
		WCHAR blah[512];
		wcscpy_s(blah, fName.c_str());
		/*fName = fName +

		WCHAR* fName = wcscat(folder, namePrefix);

		fName = wcscat(fName, vOut);
		fName = wcscat(fName, namePostfix);*/

		// init the bitmap
		// Create the bitmap object.
		BitmapClass* tempBM = new BitmapClass;
		if(!tempBM)
		{
			return false;
		}

		// Initialize the bitmap object.
		bool result = tempBM->Initialize(d3d->GetDevice(), width, height, blah, 256, 256);
		if(!result)
		{
			//MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
		tempBM->SetPosition(0,0);
		tempBM->SetDimensions(width, height);

		m_frames[i] = tempBM;
	}

	m_curFrame = 0;
	m_nextFrameTime = m_frameTime;
	m_clock = 0;
	m_playing = true;

	return true;
}

void SpriteVideo::Update(int time)
{
	if(m_clock < 0)
	{
		m_clock = 0;
		m_curFrame = 0;
		m_nextFrameTime = m_frameTime;
	}

	if(m_playing)
	{
		m_clock += time/5000.0;

		while(m_clock > m_nextFrameTime)
		{
			m_nextFrameTime += m_frameTime;
			m_curFrame++;

			if(m_curFrame >= m_totalFrames)
			{
				if(m_loop)
				{
					m_curFrame = 0;
					m_clock = 0;
					m_nextFrameTime = m_frameTime;
				}
				else
				{
					m_curFrame = m_totalFrames-1;
					m_playing = false;
				}
			}
		}
	}
}

void SpriteVideo::Draw(ID3D11DeviceContext* device, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	if(m_frames)
	{
		m_frames[m_curFrame]->Render(device);

		// Render the bitmap with the texture shader.
		m_TextureShader->Render(device, m_frames[m_curFrame]->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_frames[m_curFrame]->GetTexture());
	}
}

void SpriteVideo::Shutdown()
{
	if(m_frames)
	{
		for(int i = 0; i < m_totalFrames; i++)
		{
			m_frames[i]->Shutdown();
			delete m_frames[i];
		}
		delete [] m_frames;
		m_frames = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

}

void SpriteVideo::SetLoop(bool loop)
{
	m_loop = loop;
}

bool SpriteVideo::GetLoop()
{
	return m_loop;
}

void SpriteVideo::SetPlaying(bool playing)
{
	m_playing = playing;
}

bool SpriteVideo::GetPlaying()
{
	return m_playing;
}

double SpriteVideo::GetClockTime()
{
	return m_clock / 1000;
}

void SpriteVideo::SetClockTime(double)
{
	// do nothing
}

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }
