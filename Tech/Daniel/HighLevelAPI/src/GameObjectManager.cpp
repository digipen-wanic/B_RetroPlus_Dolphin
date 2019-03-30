//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
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
#include "GameObjectManager.h"

#include "Collider.h"
#include "GameObject.h"
#include "Space.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
GameObjectManager::GameObjectManager(Space* space)
	: BetaObject("ObjectManager", space),
	  timeAccumulator(0.0f), fixedUpdateDt(1.0f / 60.0f)
{
}

// Destructor
GameObjectManager::~GameObjectManager()
{
	Shutdown();
	Unload();
}

// Update all objects in the active game objects list.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManager::Update(float dt)
{
	// Update components if not paused
	if (!static_cast<Space*>(GetParent())->IsPaused())
	{
		VariableUpdate(dt);
		FixedUpdate(dt);
	}

	// Destroy marked objects
	DestroyObjects();

	// Draw objects
	Draw();
}

// Shutdown the game object manager, destroying all active objects.
void GameObjectManager::Shutdown(void)
{
	// Delete all objects
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end();)
	{
		delete *it;
		it = gameObjectActiveList.erase(it);
	}
}

// Unload the game object manager, destroying all object archetypes.
void GameObjectManager::Unload(void)
{
	// Delete all archetypes
	for (auto it = gameObjectArchetypes.begin(); it != gameObjectArchetypes.end();)
	{
		delete *it;
		it = gameObjectArchetypes.erase(it);
	}
}

// Add a game object to the active game object list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddObject(GameObject& gameObject)
{
	// Set parent to this manager
	gameObject.SetParent(GetParent());

	// Initialize, add to gameObject list, and increment numObjects
	gameObject.Initialize();
	gameObjectActiveList.push_back(&gameObject);
}

// Add a game object to the game object archetype list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddArchetype(GameObject& gameObject)
{
	// Add to archetype list, increment numArchetypes
	gameObjectArchetypes.push_back(&gameObject);
}

// Returns a pointer to the first active game object matching the specified name.
// Params:
//	 objectName = The name of the object to be returned, if found.
// Returns:
//   If the named object is found,
//	   then return the pointer to the named game object,
//	   else return nullptr.
GameObject* GameObjectManager::GetObjectByName(const std::string& objectName) const
{
	// Loop through gameObjects and return object with matching name
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		if (gameObjectActiveList[i]->GetName() == objectName)
		{
			return gameObjectActiveList[i];
		}
	}

	// return nullptr if not found
	return nullptr;
}

// Returns a pointer to the first game object archetype matching the specified name.
// Params:
//	 objectName = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return nullptr.
GameObject* GameObjectManager::GetArchetypeByName(const std::string& objectName) const
{
	// Loop through archetypes and return archetype with matching name
	for (size_t i = 0; i < gameObjectArchetypes.size(); i++)
	{
		std::string test = gameObjectArchetypes[i]->GetName();
		if (gameObjectArchetypes[i]->GetName() == objectName)
		{
			return gameObjectArchetypes[i];
		}
	}

	// return nullptr if not found
	return nullptr;
}

// Returns the number of active objects with the given name.
// Params:
//   objectName = The name of the objects that should be counted.
unsigned GameObjectManager::GetObjectCount(const std::string& objectName) const
{
	unsigned count = 0;

	// Loop through objects and count the objects with the same name
	// if they are not marked for deletion
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		GameObject* obj = gameObjectActiveList[i];

		if (obj->GetName() == objectName && !obj->IsDestroyed())
		{
			count++;
		}
	}

	return count;	
}

// Update object logic using variable timestep.
void GameObjectManager::VariableUpdate(float dt)
{
	// Loop through objects and update them
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Update(dt);
	}
}

// Update object physics using fixed timestep.
void GameObjectManager::FixedUpdate(float dt)
{
	timeAccumulator += dt;

	// Every fixedUpdateDt, loop through all objects and call FixedUpdate()
	while (timeAccumulator >= dt)
	{
		for (size_t i = 0; i < gameObjectActiveList.size(); i++)
		{
			gameObjectActiveList[i]->FixedUpdate(fixedUpdateDt);
		}

		// Check collisions
		CheckCollisions();

		timeAccumulator -= fixedUpdateDt;
	}
}

// Destroy any objects marked for destruction.
void GameObjectManager::DestroyObjects()
{
	// Loop through objects and delete those marked for destruction
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end();)
	{
		// determine whether the object is marked for destruction
		if ((*it)->IsDestroyed())
		{
			// remove the object, and reset the iterator
			delete *it;
			it = gameObjectActiveList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// Draw all game objects in the active game object list.
void GameObjectManager::Draw(void)
{
	// Loop through objects and call Draw
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Draw();
	}
}

// Check for collisions between each pair of objects
void GameObjectManager::CheckCollisions()
{
	// Loop through objects and check collisions
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		GameObject* obj = gameObjectActiveList[i];
		
		// Skip destroyed objects
		if (obj->IsDestroyed())
			continue;

		// make sure object has a collider, otherwise, skip it
		if (obj->GetComponent<Collider>() != nullptr)
		{
			// store object's collider
			Collider* cldr = obj->GetComponent<Collider>();

			// Loop through each object after obj
			for (size_t j = i + 1; j < gameObjectActiveList.size(); j++)
			{
				GameObject* otherObj = gameObjectActiveList[j];

				// Skip destroyed objects
				if (otherObj->IsDestroyed())
					continue;

				// make sure object has a collider, otherwise, skip it
				if (otherObj->GetComponent<Collider>() != nullptr)
				{
					// store other object's collider
					Collider* otherCldr = otherObj->GetComponent<Collider>();
					
					// Check for collision
					cldr->CheckCollision(*otherCldr);
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			continue;
		}
	}
}

//------------------------------------------------------------------------------
