#pragma once
#include "PotiEngine/Export.h"
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <iostream>

namespace PotiEngine
{
	class WorldEditor;
	class POTI_ENGINE_API AudioSystem
	{
		public:
			AudioSystem(std::string filepath);
			~AudioSystem();

			void SetSamples(drwav wav);

			ALuint CreateBuffer(std::vector<std::int16_t> samples);

			void SetBuffer(std::vector<std::int16_t> samples);
			void SetBuffer(ALuint buffer);

			void Play(std::string filepath, float volume);

			void PopulateInspector();

		private:
			std::string m_filepath;
			drwav m_wav;
			ALCdevice* m_device;
			ALCcontext* m_context;
			std::vector<std::int16_t> m_samples;
			ALuint m_buffer;

			ALuint m_source;

			float m_volume = 0.1f;
	};

}