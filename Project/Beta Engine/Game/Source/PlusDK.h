//------------------------------------------------------------------------------
//
// File Name:	LevelEditorLevel.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Level.h>
#include <fmod.hpp>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class SpriteSource;
class Texture;
class SoundManager;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class PlusDK : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create an instance of the Omega game state.
		PlusDK();

		// Load archetypes, meshes
		void Load();

		// Initialize the memory associated with the Omega game state.
		void Initialize();

		// Update the Omega game state.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

		// Set high score
		void Shutdown();

		// Unload textures, sprite sources, meshes
		void Unload();

		// Current music channel
		static FMOD::Channel* musicChannelPlus;
		
	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------
		
		// Window Title
		const std::string WindowTitle = "Level 1";

		// Sound manager
		SoundManager* soundManager;
	};
}

//----------------------------------------------------------------------------
