//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.cpp
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
#include "GameObjectFactory.h"

#include "Animation.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "ColliderPoint.h"
#include "ColliderRectangle.h"
#include "ColliderTilemap.h"
#include "GameObject.h"
#include "Parser.h"
#include "Physics.h"
#include "Sprite.h"
#include "SpriteTilemap.h"
#include "Transform.h"

#include <iostream>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a single instance of the specified game object.
// Loads the object from a text file (if it exists).
// Params:
//   name = The name of the object.
//   mesh = The mesh used for the object's sprite (if any).
//   spriteSource = The sprite source used for the object's sprite (if any).
// Returns:
//	 If the object file exists and can be loaded, then return a pointer to a new instance
//   of the specified game object type, else nullptr.
GameObject* GameObjectFactory::CreateObject(const std::string& name,
	Mesh* mesh, SpriteSource* spriteSource)
{
	// make a new object and parser
	GameObject* obj = new GameObject(name);
	Parser parser(objectFilePath + name + std::string(".txt"), std::fstream::in);

	try
	{
		// Deserialize the object from file
		obj->Deserialize(parser);
		
		// Determine whether a Sprite exists, then add mesh and spritesource
		Sprite* objSprite = obj->GetComponent<Sprite>();
		if (objSprite != nullptr)
		{
			objSprite->SetMesh(mesh);
			objSprite->SetSpriteSource(spriteSource);
		}
	}
	catch (const ParseException& exception)
	{
		std::cout << exception.what() << std::endl;

		// patch up potential leaks
		delete obj;
		obj = nullptr;
	}

	return obj;
}

// Create a single instance of the specified component.
// Params:
//   name = The name of the component.
// Returns:
//	 If the component exists, then return a pointer to a new instance 
//    of the specified component, else nullptr.
Component* GameObjectFactory::CreateComponent(const std::string& name)
{
	// Loop through registered components
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); ++it)
	{
		// Find correct type
		std::string type(typeid(**it).name());

		// If the type is found, return a clone
		if (type.substr(6) == name)
		{
			return (*it)->Clone();
		}
	}

	return nullptr;
}

// Saves an object to an archetype file so it can be loaded later.
void GameObjectFactory::SaveObjectToFile(GameObject* object)
{
	// serialize object
	Parser parser(objectFilePath + object->GetName() + std::string(".txt"), std::fstream::out);
	object->Serialize(parser);

	std::cout << object->GetName() << ": Written to file" << std::endl;
}

// Returns an instance of the factory.
GameObjectFactory& GameObjectFactory::GetInstance()
{
	// only ever instantiate one GameObjectFactory, and return the same one later
	static GameObjectFactory GOF;
	return GOF;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
GameObjectFactory::GameObjectFactory()
	: objectFilePath("Assets\\Objects\\")
{
	RegisterComponent<Animation>();
	RegisterComponent<ColliderCircle>();
	RegisterComponent<ColliderLine>();
	RegisterComponent<ColliderPoint>();
	RegisterComponent<ColliderRectangle>();
	RegisterComponent<ColliderTilemap>();
	RegisterComponent<Physics>();
	RegisterComponent<Sprite>();
	RegisterComponent<SpriteTilemap>();
	RegisterComponent<Transform>();
}

// Destructor is private to prevent accidental destruction
GameObjectFactory::~GameObjectFactory()
{
	// Loop through all registered components
	for (auto it = registeredComponents.begin(); it != registeredComponents.end(); ++it)
	{
		delete *it;
	}
}

//------------------------------------------------------------------------------
