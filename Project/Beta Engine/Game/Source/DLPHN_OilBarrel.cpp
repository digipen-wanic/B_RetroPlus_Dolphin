//------------------------------------------------------------------------------
//
// File Name:	OilBarrel.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DLPHN_OilBarrel.h"

#include <Animation.h>
#include <ColliderRectangle.h>
#include <GameObjectFactory.h>
#include <parser.h>
#include <Space.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	OilBarrel::OilBarrel()
		: Component("OilBarrel"),
		animation(nullptr),
		flameArchetype(nullptr),
		animationTimer(0.0f), animationSpeed(0.2f),
		flaming(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* OilBarrel::Clone() const
	{
		return new OilBarrel(*this);
	}

	// Initialize this component (happens at object creation).
	void OilBarrel::Initialize()
	{
		// Fetch components
		animation = GetOwner()->GetComponent<Animation>();
		flameArchetype = GameObjectFactory::GetInstance().CreateObject("Flame");

		// Set collision handler
		GetOwner()->GetComponent<ColliderRectangle>()->SetCollisionHandler(&OilBarrelCollisionHandler);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void OilBarrel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Play animation if flaming
		if (flaming)
		{
			animation->Play(animationSpeed, 1, 2, true);
		}
	}

	// Collision handler for oil barrel
	// Params:
	//   object = The oil barrel.
	//   other  = The object the oil barrel is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void OilBarrelCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		UNREFERENCED_PARAMETER(intersection);

		OilBarrel* oilBarrel = object.GetComponent<OilBarrel>();

		// Spawn flame and ignite barrel if not already on fire
		if (other.GetName() == "Barrel")
		{
			oilBarrel->SpawnFlame();
			oilBarrel->flaming = true;
		}
	}

	// Saves object data to a file.
	void OilBarrel::Serialize(Parser& parser) const
	{
		parser.WriteVariable("animationSpeed", animationSpeed);
	}

	// Loads object data from a file.
	void OilBarrel::Deserialize(Parser& parser)
	{
		parser.ReadVariable("animationSpeed", animationSpeed);
	}

	// Returns whether the oil barrel is on fire
	bool OilBarrel::isFlaming() const
	{
		return flaming;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Spawns a flame
	void OilBarrel::SpawnFlame()
	{
		GameObject* flame = new GameObject(*flameArchetype);
		GetOwner()->GetSpace()->GetObjectManager().AddObject(*flame);
	}
}

//------------------------------------------------------------------------------
