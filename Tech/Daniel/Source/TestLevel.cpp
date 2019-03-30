//------------------------------------------------------------------------------
//
// File Name:	TestLevel.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "TestLevel.h"

#include "Animation.h"
#include <Engine.h>
#include <ColliderTilemap.h>
#include "GameObject.h"
#include <Graphics.h>
#include <Input.h>
#include <Mesh.h>
#include "MeshHelper.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "Physics.h"
#include "Space.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include <SpriteTilemap.h>
#include <Texture.h>
#include "Tilemap.h"
#include "Transform.h"

//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 2.
	TestLevel::TestLevel()
		: Level("TestLevel"),
		meshPlayer(nullptr), texturePlayer(nullptr), spriteSourcePlayer(nullptr),
		dataMap(nullptr), textureMap(nullptr), spriteSourceMap(nullptr), meshMap(nullptr),
		columnsPlayer(5), rowsPlayer(6),
		columnsMap(4), rowsMap(3)
	{
	}

	// Load the resources associated with Level 2.
	void TestLevel::Load()
	{
		std::cout << "TestLevel::Load" << std::endl;

		// create resources for tilemap
		dataMap = Tilemap::CreateTilemapFromFile("Assets/Levels/TestLevel.txt");

		if (dataMap == nullptr)
		{
			std::cout << "TestLevel::Load: Error loading map!" << std::endl;
		}
		else
		{
			meshMap = CreateQuadMesh(Vector2D(1.0f / columnsMap, 1.0f / rowsMap), Vector2D(0.5, 0.5f));
			textureMap = Texture::CreateTextureFromFile("Tilemap.png");
			spriteSourceMap = new SpriteSource(columnsMap, rowsMap, textureMap);
			Graphics::GetInstance().SetBackgroundColor();
		}

		// generic quadmesh
		genericQuadMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// create resources for meshPlayer
		texturePlayer = Texture::CreateTextureFromFile("Player.png");
		spriteSourcePlayer = new SpriteSource(columnsPlayer, rowsPlayer, texturePlayer);
		meshPlayer = CreateQuadMesh(Vector2D(1.0f / columnsPlayer, 1.0f / rowsPlayer), Vector2D(0.5f, 0.5f));

		// create resources for screenshot
		textureScreenshot = Texture::CreateTextureFromFile("Screenshot.png");
		spriteSourceScreenshot = new SpriteSource(1, 1, textureScreenshot);

		// create resources for hammer
		textureHammer = Texture::CreateTextureFromFile("Hammer.png");
		spriteSourceHammer = new SpriteSource(1, 1, textureHammer);

		// create resources for ladder
		textureLadder = Texture::CreateTextureFromFile("Ladder.png");
		spriteSourceLadder = new SpriteSource(1, 1, textureLadder);
	}

	// Initialize the memory associated with Level 2.
	void TestLevel::Initialize()
	{
		std::cout << "TestLevel::Initialize" << std::endl;

		// Set camera properties
		Graphics::GetInstance().SetDepthEnabled(true);
		Graphics::GetInstance().GetCurrentCamera().SetFOV(180.0f);
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(Vector2D(250.0f, -500.0f));

		// ObjectManager & ObjectFactory variables
		GameObjectManager& GOM = GetSpace()->GetObjectManager();
		GameObjectFactory& GOF = GameObjectFactory::GetInstance();

		// create tilemap
		GameObject* Tilemap = new GameObject("Tilemap");

		// Create transform
		Transform* transform = new Transform(-350.0f, 250.0f);
		transform->SetScale(Vector2D(100.0f, 100.0f));

		// Create SpriteTilemap
		SpriteTilemap* spritetilemap = new SpriteTilemap();
		spritetilemap->SetMesh(meshMap);
		spritetilemap->SetSpriteSource(spriteSourceMap);
		spritetilemap->SetTilemap(dataMap);

		// Create ColliderTilemap
		ColliderTilemap* colliderTilemap = new ColliderTilemap();
		colliderTilemap->SetTilemap(dataMap);

		// Add components
		Tilemap->AddComponent(transform);
		Tilemap->AddComponent(spritetilemap);
		Tilemap->AddComponent(colliderTilemap);

		GOM.AddObject(*Tilemap);

		// Create playerHammer
		GOM.AddObject(*GOF.CreateObject("PlayerHammer"));

		// Create player
		GOM.AddObject(*GOF.CreateObject("Player", meshPlayer, spriteSourcePlayer));

		// Create test ladder
		GameObject* ladder1 = GOF.CreateObject("Ladder", genericQuadMesh, spriteSourceLadder);
		GOM.AddObject(*ladder1);

		// Create test hammer(s)
		GameObject* hammer1 = GOF.CreateObject("Hammer", genericQuadMesh, spriteSourceHammer);
		GOM.AddObject(*hammer1);

		// Create test firedude
		GOM.AddObject(*GOF.CreateObject("FireDude"));

		// Set up background screenshot
		//GOM.AddObject(*GOF.CreateObject("Screenshot", genericQuadMesh, spriteSourceScreenshot));
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void TestLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Unload the resources associated with Level 2.
	void TestLevel::Unload()
	{
		std::cout << "TestLevel::Unload" << std::endl;

		delete spriteSourcePlayer;
		delete texturePlayer;
		delete meshPlayer;
		
		delete spriteSourceHammer;
		delete textureHammer;

		delete spriteSourceLadder;
		delete textureLadder;

		delete textureScreenshot;
		delete spriteSourceScreenshot;

		delete genericQuadMesh;

		delete spriteSourceMap;
		delete textureMap;
		delete meshMap;
		delete dataMap;
	}
}

//----------------------------------------------------------------------------