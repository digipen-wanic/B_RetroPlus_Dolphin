//------------------------------------------------------------------------------
//
// File Name:	GameObject.cpp
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
#include "GameObject.h"

#include "Component.h"
#include "Space.h"
#include <Parser.h>
#include "GameObjectFactory.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new game object.
// Params:
//	 name = The name of the game object being created.
GameObject::GameObject(const std::string& name)
	: BetaObject(name), isDestroyed(false)
{
	components.reserve(10);
}

// Clone a game object from another game object.
// Params:
//	 other = A reference to the object being cloned.
GameObject::GameObject(const GameObject& other)
	: BetaObject(other.GetName(), other.GetParent()), isDestroyed(false)
{
	components.reserve(other.components.size());

	// Set each component to its counterpart
	for (unsigned i = 0; i < other.components.size(); i++)
	{
		components.push_back(other.components[i]->Clone());
		components[i]->SetParent(this);
	}
}

// Free the memory associated with a game object.
GameObject::~GameObject()
{
	// Delete all components
	for (size_t i = 0; i < components.size(); i++)
	{
		delete components[i];
	}
}

// Initialize this object's components and set it to active.
void GameObject::Initialize()
{
	// Call Initialize() on each component
	for (unsigned i = 0; i < components.size(); i++)
		components[i]->Initialize();
}

// Update any components attached to the game object.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObject::Update(float dt)
{
	// Ensure object isn't destroyed
	if (isDestroyed) return;

	// Call Update() on each component
	for (unsigned i = 0; i < components.size(); i++)
		components[i]->Update(dt);
}

// Updates components using a fixed timestep (usually just physics)
// Params:
//	 dt = A fixed change in time, usually 1/60th of a second.
void GameObject::FixedUpdate(float dt)
{
	// Ensure object isn't destroyed
	if (isDestroyed)
		return;

	// Call FixedUpdate() on each component
	for (unsigned i = 0; i < components.size(); i++)
		components[i]->FixedUpdate(dt);
}

// Draw any visible components attached to the game object.
void GameObject::Draw()
{
	// Call Draw() on each component
	for (unsigned i = 0; i < components.size(); i++)
		components[i]->Draw();
}

// Adds a component to the object.
void GameObject::AddComponent(Component* component)
{
	// set parent to this object
	component->SetParent(this);

	// increase numComponents, and add to array
	components.push_back(component);
}

// Mark an object for destruction.
void GameObject::Destroy()
{
	isDestroyed = true;
}

// Whether the object has been marked for destruction.
// Returns:
//		True if the object will be destroyed, false otherwise.
bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

// Get the space that contains this object.
Space* GameObject::GetSpace() const
{
	// cast parent to Space*
	return static_cast<Space*>(BetaObject::GetParent());
}

// Saves object data to a file.
void GameObject::Serialize(Parser& parser) const
{
	parser.WriteValue(GetName());
	parser.BeginScope();
	parser.WriteVariable("numComponents", components.size());

	// Loop through each individual component and output their respective serializations
	for (unsigned i = 0; i < components.size(); ++i)
	{
		parser.WriteValue(std::string(typeid(*components[i]).name()).substr(6));
		parser.BeginScope();
		components[i]->Serialize(parser);
		parser.EndScope();
	}

	parser.EndScope();
}

// Loads object data from a file.
void GameObject::Deserialize(Parser& parser)
{
	parser.ReadSkip(GetName());
	parser.ReadSkip('{');

	unsigned numComponents;
	parser.ReadVariable("numComponents", numComponents);
	components.reserve(numComponents);

	// Loop through each individual component and read their respective values
	for (unsigned i = 0; i < numComponents; i++)
	{
		// Find and create the component from the file
		std::string component;
		parser.ReadValue(component);
		Component* newComponent = GameObjectFactory::GetInstance().CreateComponent(component);

		// Ensure component exists
		if (newComponent == nullptr)
			throw ParseException(component, "The given component could not be found.");

		// Add the component
		AddComponent(newComponent);

		// Deserialize the component
		parser.ReadSkip('{');
		newComponent->Deserialize(parser);
		parser.ReadSkip('}');
	}

	parser.ReadSkip('}');
}

//------------------------------------------------------------------------------
