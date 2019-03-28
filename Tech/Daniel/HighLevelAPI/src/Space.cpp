//------------------------------------------------------------------------------
//
// File Name:	Space.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Space.h"
#include "Level.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor
// Params:
//   name = The name of the space
Space::Space(const std::string& name)
	: BetaObject(name), paused(false), currentLevel(nullptr), nextLevel(nullptr), objectManager(this)
{
}

// Destructor
Space::~Space()
{
}

// Updates the state manager and object manager.
// Params:
//   dt = The change in time since the last call to this function.
void Space::Update(float dt)
{	
	// change to next level
	if (nextLevel != nullptr)
	{
		ChangeLevel();
	}

	// update objects
	objectManager.Update(dt);

	// update currentLevel
	if (currentLevel != nullptr && !paused)
	{
		currentLevel->Update(dt);
	}
}

// Shuts down the object manager
void Space::Shutdown()
{
	// Shutdown if the level exists
	if (currentLevel != nullptr)
	{
		currentLevel->Shutdown();

		objectManager.Shutdown();
		objectManager.Unload();

		currentLevel->Unload();

		delete currentLevel;
		currentLevel = nullptr;
	}
}

// Returns a boolean indicating whether objects in this space are paused.
// Returns:
//   True if the space is paused, false otherwise.
bool Space::IsPaused() const
{
	return paused;
}

// Returns the name of the level currently running in this space.
const std::string& Space::GetLevelName() const
{
	return GetName();
}

// Pauses the space, preventing objects from being updated, but objects are still drawn.
// Params:
//   value = A true/false value that indicates whether the space should be paused.
void Space::SetPaused(bool value)
{
	paused = value;
}

// Restarts the current level (next level = current)
void Space::RestartLevel()
{
	nextLevel = currentLevel;
}

// Returns the object manager, which you can use to retrieve and add objects.
GameObjectManager& Space::GetObjectManager()
{
	return objectManager;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Updates current/next level pointers and calls shutdown/unload/load/init
void Space::ChangeLevel()
{	
	// restarting
	if (currentLevel != nullptr)
	{
		currentLevel->Shutdown();
		objectManager.Shutdown();
	}
	
	// switching levels
	if (currentLevel != nextLevel)
	{
		if (currentLevel != nullptr)
		{
			objectManager.Unload();
			currentLevel->Unload();
		}

		delete currentLevel;
		currentLevel = nextLevel;
		
		currentLevel->Load();
	}
	
	nextLevel = nullptr;
	currentLevel->Initialize();
}

//------------------------------------------------------------------------------
