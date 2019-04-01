//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
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

// BetaFramework Engine
#include <Engine.h>
#include <Graphics.h>

// Engine modules
#include "Space.h"
#include <SoundManager.h>
#include <SpaceManager.h>
#include <GameObjectFactory.h>

// Custom Components
#include "DLPHN_DonkeyKong.h"
#include "DLPHN_PlayerController.h"
#include "DLPHN_PlayerAnimation.h"
#include "DLPHN_Damsel.h"
#include "DLPHN_Barrel.h"

// Initial game state
#include "Level1.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR command_line, _In_ int show)
{
	// Enable memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// Unused parameters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);
	UNREFERENCED_PARAMETER(instance);

	// Create a new space called "Level"
	Space* space = new Space("Level", false);
	// Set initial level to the second level.
	space->SetLevel(new Levels::Level1());

	SpaceManager* spaceManager = new SpaceManager();
	spaceManager->AddSpace(*space);

	Engine::GetInstance().AddModule(spaceManager);

	GameObjectFactory::GetInstance().RegisterComponent<DLPHN::DonkeyKong>();
	GameObjectFactory::GetInstance().RegisterComponent<DLPHN::PlayerController>();
	GameObjectFactory::GetInstance().RegisterComponent<DLPHN::PlayerAnimation>();
	GameObjectFactory::GetInstance().RegisterComponent<DLPHN::Damsel>();
	GameObjectFactory::GetInstance().RegisterComponent<DLPHN::Barrel>();

	// Add Sound Manager
	Engine::GetInstance().AddModule(new SoundManager());

	// Game engine goes!
	const unsigned Height = 1080;
	const unsigned Width = 1920;
	Engine::GetInstance().Start(Width, Height, 200);

	return 0;
}
