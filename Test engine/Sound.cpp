#include "Sound.h"

namespace JR_Sound
{
	Sound::Sound()
	{
		m_DirectSound = NULL;
		m_primaryBuffer = NULL;
		m_secondaryBuffer = NULL;
	}

	Sound::~Sound()
	{
		shutdown();
	}

	bool Sound::init(HWND hwnd)
	{
		bool result;

		//init direct sound and the primary sound buffer
		result = initDirectSound(hwnd);
		if (!result)
		{
			return false;
		}

		//Load a wave file audio into a secondary buffer.
		result = loadWaveFile("BarFight.wav", &m_secondaryBuffer);
		if (!result)
		{
			return false;
		}

		result = playWaveFile();
		if (!result)
		{
			return false;
		}

		return true;
	}

	void Sound::shutdown()
	{
		//release the secondary buffer.
		shutdownWaveFile(&m_secondaryBuffer);

		//shut down the directsound api
		shutdownDirectSound();
	}

	bool Sound::initDirectSound(HWND hwnd)
	{
		HRESULT result;
		DSBUFFERDESC bufferDesc;
		WAVEFORMATEX waveFormat;

		//init the direct sound interface for the defualt sound device
		result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
		if (FAILED(result))
		{
			return false;
		}

		//Set the cooperative level to priority so the format of the primary sound buffer can be modified
		result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
		if (FAILED(result))
		{
			return false;
		}

		//Set up the primary buffer description
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = 0;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = NULL;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		//Get control of the primary sound buffer on the defulat sound device.
		result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
		if (FAILED(result))
		{
			return false;
		}

		//Setup the format of the primary sound buffer
		//In this case it is a .WAV file recorded at 44,100 amples per second in 16-bit stereo
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec* waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		//set the primary buffer to be the wave format specified.
		result = m_primaryBuffer->SetFormat(&waveFormat);
		if (FAILED(result))
		{
			return false;
		}

		return true;

	}

	void Sound::shutdownDirectSound()
	{
		//release the primary buffer
		if (m_primaryBuffer)
		{
			m_primaryBuffer->Release();
			m_primaryBuffer = NULL;
		}

		//Release the direct sound interface
		if (m_DirectSound) 
		{
			m_DirectSound->Release();
			m_DirectSound = NULL;
		}
	}

	bool Sound::loadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
	{
		int error;
		FILE* file;
		unsigned int count;
		WaveHeader waveHeader;
		WAVEFORMATEX waveFormat;
		DSBUFFERDESC bufferDesc;
		HRESULT result;
		IDirectSoundBuffer* tempBuffer;
		unsigned char* waveData;
		unsigned char* buffer;
		unsigned long buffersize;

		//Open the wave file in the binary
		error = fopen_s(&file, filename, "rb");
		if (error != 0)
		{
			return false;
		}

		//Read in the wave file header.
		count = fread(&waveHeader, sizeof(waveHeader), 1, file);
		if (count != 1)
		{
			return false;
		}

		//Check that the chunk id is the riff format
		if (waveHeader.chunkid[0] != 'R' || waveHeader.chunkid[1] != 'I' ||
			waveHeader.chunkid[2] != 'F' || waveHeader.chunkid[3] != 'F')
		{
			return false;
		}

		//Check that the file format is the wave format
		if ((waveHeader.format[0] != 'W') || (waveHeader.format[1] != 'A') ||
			waveHeader.format[2] != 'V' || waveHeader.format[3] != 'E')
		{
			return false;
		}

		//check that the subchunk id is the fmt format
		if (waveHeader.subChunkId[0] != 'f' || waveHeader.subChunkId[1] != 'm' ||
			waveHeader.subChunkId[2] != 't' )
		{
			return false;
		}

		//check that the audio format is WAVE_FORMAT_PCM
		if (waveHeader.audioFormat != WAVE_FORMAT_PCM)
		{
			return false;
		}

		//check that the wave file is in a stereo format
		if (waveHeader.numChannels != 2)
		{
			return false;
		}

		//check that the wave file was recorded at a sample rate of 44.1 Khz
		if (waveHeader.sampleRate != 44100)
		{
			return false;
		}

		//check that the wave file was recorded in a 16 bit format
		if (waveHeader.bistPerSample  != 16)
		{
			return false;
		}

		//Check for tha data chunk id
		if (waveHeader.dataChunkId[0] != 'd' || waveHeader.dataChunkId[1] != 'a' ||
			waveHeader.dataChunkId[2] != 't' || waveHeader.dataChunkId[3] != 'a')
		{
			return false;
		}

		//set the wave format of the secondary buffer that this wave file will be loaded into
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)* waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec* waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		//Set the description of the secondary buffer sound buffer that the wavefile will be loaded unto.
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = waveHeader.dataSize;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = &waveFormat;
		bufferDesc.guid3DAlgorithm = GUID_NULL;
		
		//Create a temporary sound buffer with the specific buffer settings.
		result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
		if (FAILED(result))
		{
			return false;
		}

		//Text the buffer format against the direct sound 8 interface and create the secondary buffer.
		result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Release the temporary buffer.
		tempBuffer->Release();
		tempBuffer = 0;

		//Mcove to the begining of the wave data which starts at the end of the data chunk header.
		fseek(file, sizeof(WaveHeader), SEEK_SET);

		//Create a tempoary buffer to hold the wave file data.
		waveData = new unsigned char[waveHeader.dataSize];
		if (!waveData)
		{
			return false;
		}

		//Read in the wave file dtat in the newly created buffer.
		count = fread(waveData, 1, waveHeader.dataSize, file);
		if (count != waveHeader.dataSize)
		{
			return false;
		}

		//Close the file once done reading
		error = fclose(file);
		if (error != 0)
		{
			return false;
		}


		// Lock the secondary buffer to write wave data into it.
		result = (*secondaryBuffer)->Lock(0, waveHeader.dataSize, (void**)&buffer, (DWORD*)&buffersize, NULL, 0, 0);
		if (FAILED(result))
		{
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy(buffer, waveData, waveHeader.dataSize);

		// Unlock the secondary buffer after the data has been written to it.
		result = (*secondaryBuffer)->Unlock((void*)buffer, buffersize, NULL, 0);
		if (FAILED(result))
		{
			return false;
		}


		//Realeas the wave data since it was copied into the secondary buffer.
		delete[] waveData;
		waveData = 0;

		return true;
	}

	void Sound::shutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
	{
		//release the secondary sound buffer
		if (*secondaryBuffer)
		{
			(*secondaryBuffer)->Release();
			*secondaryBuffer = NULL;
		}
	}

	bool Sound::playWaveFile()
	{
		HRESULT result;

		//Set the position to the beginning of the sound buffer
		result = m_secondaryBuffer->SetCurrentPosition(0);
		if (FAILED(result))
		{
			return false;
		}

		//Play the contents of the secondary sound buffer.
		result = m_secondaryBuffer->Play(0, 0, 0);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}
}