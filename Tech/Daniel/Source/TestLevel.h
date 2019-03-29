//------------------------------------------------------------------------------
//
// File Name:	TestLevel.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Level.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class SpriteSource;
class Mesh;
class GameObject;
class Tilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class TestLevel : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 2.
		TestLevel();

		// Load the resources associated with Level 2.
		void Load() override;

		// Initialize the memory associated with Level 2.
		void Initialize() override;

		// Update Level 2.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Level 2.
		void Unload() override;

	private:

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		Mesh* genericQuadMesh;

		// Player
		Mesh* meshPlayer;
		Texture* texturePlayer;
		SpriteSource* spriteSourcePlayer;
		unsigned columnsPlayer;
		unsigned rowsPlayer;

		// Screenshot
		Texture* textureScreenshot;
		SpriteSource* spriteSourceScreenshot;

		// Hammer
		Texture* textureHammer;
		SpriteSource* spriteSourceHammer;

		// Ladder
		Texture* textureLadder;
		SpriteSource* spriteSourceLadder;

		// Tilemap
		Tilemap* dataMap;
		Texture* textureMap;
		SpriteSource* spriteSourceMap;
		Mesh* meshMap;
		unsigned columnsMap;
		unsigned rowsMap;
	};
}

//----------------------------------------------------------------------------
