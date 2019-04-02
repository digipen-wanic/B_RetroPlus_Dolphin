//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectManager.h"
#include "Space.h"
#include "GameObject.h"
#include "Collider.h"

GameObjectManager::GameObjectManager(Space * space) : BetaObject("ObjectManager", space)
{
	timeAccumulator = 0.0f;
}

GameObjectManager::~GameObjectManager()
{
	Shutdown();
	Unload();
}

void GameObjectManager::Update(float dt)
{
	if (!static_cast<Space*>(GetParent())->IsPaused()) {
		VariableUpdate(dt);
		FixedUpdate(dt);
	}
	DestroyObjects();
	Draw();
}

void GameObjectManager::Shutdown(void)
{
	std::vector<GameObject*>::iterator i;

	for (i = gameObjectActiveList.begin(); i != gameObjectActiveList.end(); ++i)
	{
		delete (*i);
		*i = nullptr;
	}

	gameObjectActiveList.clear();
	gameObjectActiveList.shrink_to_fit();
}

void GameObjectManager::Unload(void)
{
	std::vector<GameObject*>::iterator i;

	for (i = gameObjectArchetypes.begin(); i != gameObjectArchetypes.end(); ++i)
	{
		delete (*i);
		*i = nullptr;
	}

	gameObjectArchetypes.clear();
	gameObjectArchetypes.shrink_to_fit();
}

void GameObjectManager::AddObject(GameObject & _gameObject)
{
	if (&_gameObject == nullptr) {
		std::cout << "Attempted to add Null object" << std::endl;
		return;
	}

	gameObjectActiveList.push_back(&_gameObject);
	gameObjectActiveList.back()->SetParent(GetParent());
	gameObjectActiveList.back()->Initialize();
}

void GameObjectManager::AddArchetype(GameObject & _gameObject)
{
	gameObjectArchetypes.push_back(&_gameObject);
}

GameObject * GameObjectManager::GetObjectByName(const std::string & objectName) const
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		if (gameObjectActiveList[i]->GetName() == objectName) {
			return gameObjectActiveList[i];
		}
	}
	std::cout << "Couldn't find object with name of " << objectName << "." << std::endl;
	return nullptr;
}

GameObject * GameObjectManager::GetArchetypeByName(const std::string & objectName) const
{
	for (size_t i = 0; i < gameObjectArchetypes.size(); i++)
	{
		if (gameObjectArchetypes[i]->GetName() == objectName) {
			return gameObjectArchetypes[i];
		}
	}

	std::cout << "Couldn't find archetype with name of " << objectName << "." << std::endl;
	return nullptr;
}

unsigned GameObjectManager::GetObjectCount(const std::string & objectName) const
{
	int output = 0;

	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		if (gameObjectActiveList[i]->GetName() == objectName && !gameObjectActiveList[i]->IsDestroyed()) {
			output++;
		}
	}

	return output;
}

void GameObjectManager::DestroyAllObjects()
{
	if (gameObjectActiveList.empty()) {
		return;
	}

	for (size_t i = 0; i < gameObjectActiveList.size(); i++) {
		delete gameObjectActiveList[i];
		gameObjectActiveList[i] = nullptr;

		gameObjectActiveList.erase(gameObjectActiveList.begin() + i);
	}

	gameObjectActiveList.shrink_to_fit();
}

std::vector<GameObject*> GameObjectManager::GetGameObjectActiveList()
{
	return gameObjectActiveList;
}

std::string GameObjectManager::GenerateUniqueGameObjectName(const std::string & name)
{
	std::string outputName = name;

	unsigned numberToAppend = 0;

	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		std::string GOName = gameObjectActiveList[i]->GetName();

		size_t pos = GOName.find_last_of("_");

		std::string currentName = GOName.substr(0, pos);

		if (currentName == name)
		{
			if (pos != std::string::npos) {
				std::string currentNumberString = GOName.substr(pos+1);

				int curNumber = std::stoi(currentNumberString);

				numberToAppend = curNumber + 1;
			}
			else {
				numberToAppend++;
			}
		}
	}

	if (numberToAppend > 0)
	{
		outputName = outputName + "_" + std::to_string(numberToAppend);
	}

	return outputName;
}

void GameObjectManager::VariableUpdate(float dt)
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Update(dt);
	}
}

void GameObjectManager::FixedUpdate(float dt)
{
	timeAccumulator += dt;

	if (timeAccumulator >= fixedUpdateDt)
	{
		for (size_t i = 0; i < gameObjectActiveList.size(); i++)
		{
			gameObjectActiveList[i]->FixedUpdate(fixedUpdateDt);
		}

		CheckCollisions();

		timeAccumulator -= fixedUpdateDt;
	}
}

void GameObjectManager::DestroyObjects()
{
	if (gameObjectActiveList.empty()) {
		return;
	}

	for (size_t i = 0; i < gameObjectActiveList.size(); i++) {
		if (gameObjectActiveList[i]->IsDestroyed()) {
			delete gameObjectActiveList[i];
			gameObjectActiveList[i] = nullptr;

			gameObjectActiveList.erase(gameObjectActiveList.begin() + i);
		}
	}

	//gameObjectArchetypes.clear();
	gameObjectActiveList.shrink_to_fit();
}

void GameObjectManager::Draw(void)
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Draw();
	}
}

void GameObjectManager::CheckCollisions()
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		GameObject* go1 = gameObjectActiveList[i];
		if (!go1->IsDestroyed()) {
			/*Collider* collider1 = go1->GetComponent<Collider>();
			if (collider1 != nullptr) {
				for (size_t x = 0; x < gameObjectActiveList.size(); x++)
				{
					GameObject* go2 = gameObjectActiveList[x];
					if (!go2->IsDestroyed()) {
						Collider* collider2 = go2->GetComponent<Collider>();
						if (collider2 != nullptr && go1 != go2) {
							collider1->CheckCollision(*collider2);
						}
					}
				}
			}*/
			// Skip the collisoin steps if the first object is destroyed
			if (go1->IsDestroyed())
				continue;
			// Get the first set of colliders...
			std::vector<Collider*> colliderSet1 = go1->GetAllComponents<Collider>();
			// .. and check them against the second set of colliders if it is not empty
			if (colliderSet1.size() > 0)
			{
				// Get the second set of colliders...
				for (size_t x = i; x < gameObjectActiveList.size(); ++x)
				{
					GameObject* go2 = gameObjectActiveList[x];
					// Skip the collision code if the second object is destroyed
					if (go2->IsDestroyed())
						continue;

					std::vector<Collider*> colliderSet2 = go2->GetAllComponents<Collider>();
					// And now check every collider against each other
					for (size_t go1ColliderIndex = 0; go1ColliderIndex < colliderSet1.size(); ++go1ColliderIndex)
					{
						for (size_t go2ColliderIndex = 0; go2ColliderIndex < colliderSet2.size(); ++go2ColliderIndex)
						{
							// Check collision between the two objects
							colliderSet1[go1ColliderIndex]->CheckCollision(*colliderSet2[go2ColliderIndex]);
						}
					}
				}
			}
		}
	}
}
