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
#include <Sprite.h>
#include <SpriteSource.h>
#include <Transform.h>

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
		transform = GetOwner()->GetComponent<Transform>();
		
		// Get flame archetype based on whether the level is alpha or beta (plus)
		if (GetOwner()->GetComponent<Sprite>()->GetSpriteSource()->GetName() == "OilBarrel")
		{
			flameArchetype = GameObjectFactory::GetInstance().CreateObject("Flame");
		}
		else
		{
			flameArchetype = GameObjectFactory::GetInstance().CreateObject("FlamePlus");
		}

		// Set collision handler
		GetOwner()->GetComponent<ColliderRectangle>()->SetCollisionHandler(&OilBarrelCollisionHandler);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void OilBarrel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		static bool startFlame = true;

		// Play animation if flaming
		if (flaming && startFlame)
		{
			startFlame = false;
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
		if (other.GetName() == "Barrel" || other.GetName() == "BarrelPlus")
		{
			// Destroy the barrel
			other.Destroy();
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
		GameObjectManager& GOM = GetOwner()->GetSpace()->GetObjectManager();

		// Only spawn up to two flames at a time
		if (GOM.GetObjectCount("Flame") + GOM.GetObjectCount("FlamePlus") < 2)
		{
			GameObject* flame = new GameObject(*flameArchetype);

			// Set position
			Vector2D newPos(transform->GetTranslation().x + 10.0f, transform->GetTranslation().y);
			flame->GetComponent<Transform>()->SetTranslation(newPos);

			// Add to object manager
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*flame);
		}
	}
}

//------------------------------------------------------------------------------
