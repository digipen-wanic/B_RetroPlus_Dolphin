//============================================================
// Author: Jayden Yamada
// File Name: DLPHN_Barrel.cpp
// Date Created: March 29 2019
//============================================================

//============================================================
// Includes:
//============================================================

#include "stdafx.h"
#include "DLPHN_Barrel.h"

#include <Animation.h>
#include <Collider.h>
#include <GameObject.h>
#include <Physics.h>
#include <Parser.h>
#include <Transform.h>

//============================================================

namespace DLPHN
{
	//============================================================
	// Public Functions:
	//============================================================

	// Default Constructor
	Barrel::Barrel()
		: Component("Barrel"),
		  physics(nullptr), animation(nullptr), transform(nullptr),
		  rollLength(4),
		  speed(10.0f)
	{
	}

	// Clones the component
	// Returns: A copy of the componnet
	Component* Barrel::Clone() const
	{
		return new Barrel(*this);
	}

	// Loads the component
	void Barrel::Load()
	{
	}

	// Initializes the component
	void Barrel::Initialize()
	{
		// Get component(s)
		physics = GetOwner()->GetComponent<Physics>();
		animation = GetOwner()->GetComponent<Animation>();
		transform = GetOwner()->GetComponent<Transform>();
		originalScale = transform->GetScale();

		// Set gravity of barrel
		// physics->SetGravity(Vector2D(0, -1));
		physics->SetVelocity(Vector2D(0, -1));

		// Start animation
		animation->Play(0.2f, 0, rollLength, true);

		// set player collision handler
		GetOwner()->GetComponent<Collider>()->SetCollisionHandler(&BarrelCollisionHandler);
	}

	// Updates the component
	void Barrel::Update(float dt)
	{
		Roll();
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Barrel::Serialize(Parser& parser) const
	{
		parser.WriteVariable("speed", speed);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Barrel::Deserialize(Parser& parser)
	{
		parser.ReadVariable("speed", speed);
	}

	// Collision handler for the barrel.
	// Params:
	//   object = The barrel.
	//   other  = The object the barrel is colliding with.
	void BarrelCollisionHandler(GameObject& object, GameObject& other)
	{
		Barrel* barrel = object.GetComponent<Barrel>();

		if (other.GetName() == "Hammer")
		{
			barrel->SmashBarrel();
		}
	}

	//============================================================
	// Private Functions:
	//============================================================

	// Moves barrel in direction
	void Barrel::Roll()
	{

	}

	// Handles basic animation
	void Barrel::Animate()
	{
		// Flip sprite depending on direction of travel
		if (physics->GetVelocity().x < 0.0f)
		{
			transform->SetScale(Vector2D(-originalScale.x, originalScale.y));
		}
		else
		{
			transform->SetScale(originalScale);
		}
	}

	// Destruction sequence for barrel
	void Barrel::SmashBarrel()
	{
		// Destroy this object
		GetOwner()->Destroy();
	}
}

//============================================================