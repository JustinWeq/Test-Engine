#pragma once
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
namespace JR_Sound
{
	class Sound
	{
	private:
		struct WaveHeader
		{
			char chunkid[4];
			unsigned long chunkZise;
			char format[4];
			char subChunkId[4];
			unsigned long sunChunckSize;
			unsigned short audioFormat;
			unsigned short numChannels;
			unsigned long sampleRate;
			unsigned long bytesPerSecond;
			unsigned short blockAlign;
			unsigned short bistPerSample;
			char dataChunkId[4];
			unsigned long dataSize;
		};

	public:
		Sound();
		~Sound();
		
		bool init(HWND hwnd);
		void shutdown();

	private:
		bool initDirectSound(HWND hwnd);
		void shutdownDirectSound();

		bool loadWaveFile(char* filename, IDirectSoundBuffer8** buffer);
		void shutdownWaveFile(IDirectSoundBuffer8** buffer);

		bool playWaveFile();

	private:
		IDirectSound8* m_DirectSound;
		IDirectSoundBuffer* m_primaryBuffer;

		IDirectSoundBuffer8* m_secondaryBuffer;
	};
}