//------------------------------------------------------------------------------
//
// File Name:	SoundManager.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Lab 8
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SoundManager.h"

#include <fmod_errors.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor - initializes FMOD.
SoundManager::SoundManager()
	: BetaObject("SoundManager"),
	  musicVolume(0.0f), effectsVolume(0.0f),
	  audioFilePath("Audio/"), bankFilePath("Banks/"), eventPrefix("event:/"),
	  numSounds(0), numBanks(0),
	  musicChannel(nullptr), effectsChannelGroup(nullptr),
	  system(nullptr), studioSystem(nullptr)
{
	// Create and initialize high-level system
	FMOD_Assert(FMOD::Studio::System::create(&studioSystem));
	FMOD_Assert(studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));

	// Get low-level system
	FMOD_Assert(studioSystem->getLowLevelSystem(&system));

	// Get effects channel group
	FMOD_Assert(system->createChannelGroup("SoundEffects", &effectsChannelGroup));
}

// Destructor
SoundManager::~SoundManager()
{
	FMOD_Assert(studioSystem->release());
}

// Update the FMOD system.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Update studio system
	FMOD_Assert(studioSystem->update());
}

// Shutdown the sound manager.
void SoundManager::Shutdown(void)
{
	// Loop through and free all sounds
	for (size_t i = 0; i < numSounds; i++)
	{
		FMOD_Assert(soundList[i]->release());
	}

	// Loop through and free all banks
	for (size_t i = 0; i < numBanks; i++)
	{
		FMOD_Assert(bankList[i]->unload());
	}
}

// Creates a non-looping FMOD sound.
// Params:
//	 filename = Name of the sound file (WAV).
void SoundManager::AddEffect(const std::string& filename)
{
	AddSound(filename, FMOD_DEFAULT);
}

// Creates an FMOD stream for a music file.
// Params:
//	 filename = Name of the music file (MP3).
void SoundManager::AddMusic(const std::string& filename)
{
	AddSound(filename, FMOD_DEFAULT | FMOD_LOOP_NORMAL);
}

// Creates an FMOD sound bank
// Params:
//   filename = Name of the FMOD bank file.
void SoundManager::AddBank(const std::string& filename)
{
	// Get full filepath
	std::string filepath = "Assets/" + audioFilePath + bankFilePath + filename;

	// Load bank file and add at next available spot
	FMOD::Studio::Bank* bank;
	FMOD_Assert(studioSystem->loadBankFile(filepath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
	bankList[numBanks] = bank;
	numBanks++;
}

// Plays a sound with the specified name.
// Params:
//	 name = The name of the sound to be played.
// Return:
//   The channel that was used to play the given sound.
FMOD::Channel* SoundManager::PlaySound(const std::string& name_)
{
	char soundName[100] = { 0 };
	FMOD_MODE soundMode;

	for (size_t i = 0; i < numSounds; i++)
	{
		// get sound name
		FMOD_Assert(soundList[i]->getName(soundName, 100));

		// skip sounds with a different name
		if (strcmp(soundName, name_.c_str()) != 0)
			continue;

		// get sound mode
		FMOD_Assert(soundList[i]->getMode(&soundMode));

		// check sound mode and play music/effect accordingly
		if (soundMode & FMOD_CREATESTREAM)
		{
			return PlayMusic(soundList[i]);
		}
		else
		{
			return PlayEffect(soundList[i]);
		}
	}

	// return null if no sound was found with the name
	return nullptr;
}

// Starts an audio event with the given name.
// The event must be in one of the loaded banks.
// Params:
//	 name = The name of the event that will be started.
FMOD::Studio::EventInstance* SoundManager::PlayEvent(const std::string& name_)
{
	std::string	eventName = eventPrefix + name_;
	FMOD::Studio::EventDescription* eventDesc;
	FMOD::Studio::EventInstance* eventInst;

	// get event
	FMOD_Assert(studioSystem->getEvent(eventName.c_str(), &eventDesc));

	// get instance, start, and release
	FMOD_Assert(eventDesc->createInstance(&eventInst));
	FMOD_Assert(eventInst->start());
	FMOD_Assert(eventInst->release());
	return eventInst;
}

// Set the volume of the music channel.
// Params:
//	 volume = New value for the music volume (0.0 to 1.0).
void SoundManager::SetMusicVolume(float volume)
{
	musicVolume = volume;

	// set musicChannel volume if it exists
	if (musicChannel != nullptr)
		FMOD_Assert(musicChannel->setVolume(volume));
}

// Set the volume of the sound effects channel group.
// Params:
//	 volume = New value for the FX volume (0.0 to 1.0).
void SoundManager::SetEffectsVolume(float volume)
{
	effectsVolume = volume;

	// set effectsChannelGroup volume if it exists
	if (effectsChannelGroup != nullptr)
		FMOD_Assert(effectsChannelGroup->setVolume(volume));
}

// Get the volume of the music channel.
// Return:
//	 volume = Current value for the music volume.
float SoundManager::GetMusicVolume() const
{
	return musicVolume;
}

// Get the volume of the sound effects channel group.
// Return:
//	 volume = Current value for the FX volume.
float SoundManager::GetEffectsVolume() const
{
	return effectsVolume;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Different behaviors for SFX vs BGM
FMOD::Channel* SoundManager::PlayEffect(FMOD::Sound* sound) const
{
	FMOD::Channel* channel;

	// play sound with low-level system
	system->playSound(sound, effectsChannelGroup, false, &channel);

	return channel;
}

FMOD::Channel* SoundManager::PlayMusic(FMOD::Sound* sound)
{
	// stop music if the channel is not null
	if (musicChannel != nullptr)
		FMOD_Assert(musicChannel->stop());

	// play music with low-level system
	FMOD_Assert(system->playSound(sound, nullptr, false, &musicChannel));
	return musicChannel;
}

// Add FX/BGM helper
void SoundManager::AddSound(const std::string& filename, FMOD_MODE mode)
{
	// Get full filepath
	std::string filepath = "Assets/" + audioFilePath + filename;
	FMOD::Sound* sound;

	// create sound using low-level system and add to sound list
	FMOD_Assert(system->createSound(filepath.c_str(), mode, nullptr, &sound));
	soundList[numSounds] = sound;
	numSounds++;
}

// Prints error code from function result
void SoundManager::FMOD_Assert(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
	}
}

//------------------------------------------------------------------------------
