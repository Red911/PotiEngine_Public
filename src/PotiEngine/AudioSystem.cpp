#include "PotiEngine/AudioSystem.h"
#include "PotiEngine/WorldEditor.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace PotiEngine
{
	AudioSystem::AudioSystem(std::string filepath) :
		m_filepath(filepath)
	{
		m_device = alcOpenDevice(nullptr);
		m_context = alcCreateContext(m_device, nullptr);
		alcMakeContextCurrent(m_context);

		drwav_init_file(&m_wav, m_filepath.c_str(), nullptr);

		alGenBuffers(1, &m_buffer);

		m_samples.resize(m_wav.channels * m_wav.totalPCMFrameCount);
		drwav_read_pcm_frames_s16(&m_wav, m_wav.totalPCMFrameCount, &m_samples[0]);

		alBufferData(m_buffer, AL_FORMAT_STEREO16, m_samples.data(), m_samples.size() * sizeof(std::int16_t), 44100);

		alGenSources(1, &m_source);

		alSourcei(m_source, AL_BUFFER, m_buffer);

		alSourcePlay(m_source);

		alSourcef(m_source, AL_GAIN, m_volume);
	}

	AudioSystem::~AudioSystem()
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_context);

		alcCloseDevice(m_device);
	}

	void AudioSystem::SetSamples(drwav wav)
	{
		m_samples.resize(wav.channels * wav.totalPCMFrameCount);
		drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, &m_samples[0]);
	}

	ALuint AudioSystem::CreateBuffer(std::vector<std::int16_t> samples)
	{
		ALuint buffer;

		alGenBuffers(1, &buffer);
		alBufferData(m_buffer, AL_FORMAT_STEREO16, m_samples.data(), m_samples.size() * sizeof(std::int16_t), 44100);
		return buffer;
	}

	void AudioSystem::SetBuffer(std::vector<std::int16_t> samples)
	{
		alBufferData(m_buffer, AL_FORMAT_STEREO16, m_samples.data(), m_samples.size() * sizeof(std::int16_t), 44100);
	}

	void AudioSystem::SetBuffer(ALuint buffer)
	{
		m_buffer = buffer;
	}

	void AudioSystem::Play(std::string filepath, float volume)
	{
		drwav_init_file(&m_wav, filepath.c_str(), nullptr);

		m_samples.resize(m_wav.channels * m_wav.totalPCMFrameCount);
		drwav_read_pcm_frames_s16(&m_wav, m_wav.totalPCMFrameCount, &m_samples[0]);

		m_buffer = CreateBuffer(m_samples);

		alGenSources(1, &m_source);

		alSourcei(m_source, AL_BUFFER, m_buffer);

		alSourcePlay(m_source);

		alSourcef(m_source, AL_GAIN, m_volume);
	}
	void AudioSystem::PopulateInspector()
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Audio"))
				{
					if (ImGui::BeginMenu("Music"))
					{
						ImGui::InputText("Music Path", &m_filepath);
						ImGui::SameLine();
						if (ImGui::Button("Update"))
						{
							alSourceStop(m_source);

							alDeleteBuffers(1, &m_buffer);

							alDeleteSources(1, &m_source);

							drwav_init_file(&m_wav, m_filepath.c_str(), nullptr);

							alGenBuffers(1, &m_buffer);

							m_samples.resize(m_wav.channels * m_wav.totalPCMFrameCount);
							drwav_read_pcm_frames_s16(&m_wav, m_wav.totalPCMFrameCount, &m_samples[0]);

							alBufferData(m_buffer, AL_FORMAT_STEREO16, m_samples.data(), m_samples.size() * sizeof(std::int16_t), 44100);

							alGenSources(1, &m_source);

							alSourcei(m_source, AL_BUFFER, m_buffer);

							alSourcePlay(m_source);

							alSourcef(m_source, AL_GAIN, m_volume);
						}
							

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Volume"))
					{
						if (ImGui::SliderFloat("Volume", &m_volume, 0.0f, 1.0f)) 
						{
							alSourcef(m_source, AL_GAIN, m_volume);
						}

						ImGui::EndMenu();
					}

					
					if (ImGui::MenuItem("Pause"))
					{
						alSourcePause(m_source);
					}

					if (ImGui::MenuItem("Resume"))
					{
						ALint state;
						alGetSourcei(m_source, AL_SOURCE_STATE, &state);
						
						if (state == AL_PAUSED)
						{
							alSourcePlay(m_source);
							
						}
												
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::EndMainMenuBar();
		}
    }
}