//------------------------------------------------------------------------------
//
// File Name:	FlameController.cpp
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
#include "DLPHN_FlameController.h"

#include <Animation.h>
#include <ColliderCircle.h>
#include <ColliderLine.h>
#include <ColliderPoint.h>
#include <ColliderRectangle.h>
#include <Parser.h>
#include <Physics.h>
#include <Random.h>
#include <Space.h>
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
	FlameController::FlameController()
		: Component("FlameController"),
		flameMoveSpeed(3.0f),
		gravity(0.0f, -300.0f),
		transform(nullptr), physics(nullptr), animation(nullptr),
		animationSpeed(0.3f),
		directionTimer(0.0f), directionCooldown(3.0f),
		onGround(false),
		touchingLadder(false), onLadder(false), ladderTimer(0.0f),
		isDying(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* FlameController::Clone() const
	{
		return new FlameController(*this);
	}

	// Initialize this component (happens at object creation).
	void FlameController::Initialize()
	{
		// Fetch components
		physics = GetOwner()->GetComponent<Physics>();
		transform = GetOwner()->GetComponent<Transform>();
		animation = GetOwner()->GetComponent<Animation>();

		// Fetch original scale
		originalScale = transform->GetScale();

		// Initial horizontal movement
		physics->SetVelocity(Vector2D(flameMoveSpeed, physics->GetVelocity().y));

		// Cycle through animation
		animation->Play(animationSpeed, 0, 1, true);

		// Set collision handlers
		ColliderPoint* pointCollider = GetOwner()->GetComponent<ColliderPoint>();
		pointCollider->SetCollisionHandler(&FlamePointCollisionHandler);

		ColliderCircle* circleCollider = GetOwner()->GetComponent<ColliderCircle>();
		circleCollider->SetCollisionHandler(&CircleCollisionHandler);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void FlameController::Update(float dt)
	{
		// Slightly "bob" up and down
		static float timer = 0.0f;
		static bool dir = 1;
		timer += dt;
		if (timer > 0.75f)
		{
			timer = 0.0f;
			if (dir)
			{
				transform->SetTranslation(Vector2D(transform->GetTranslation().x, transform->GetTranslation().y - 5.0f));
			}
			else
			{
				transform->SetTranslation(Vector2D(transform->GetTranslation().x, transform->GetTranslation().y + 5.0f));
			}

			dir = !dir;
		}

		directionTimer += dt;

		// Switch directions randomly every directionCooldown seconds
		if (directionTimer > directionCooldown)
		{
			// Reset timer
			directionTimer = 0.0f;

			// 50% chance
			if (!RandomRange(0, 3))
			{
				physics->SetVelocity(-physics->GetVelocity());
			}
		}

		// Switch direction if on edge
		if (transform->GetTranslation().x < -350.0f)
		{
			transform->SetTranslation(Vector2D(transform->GetTranslation().x + 1.0f, transform->GetTranslation().y));
			physics->SetVelocity(-physics->GetVelocity());
		}
		if (transform->GetTranslation().x > 350.0f)
		{
			transform->SetTranslation(Vector2D(transform->GetTranslation().x - 1.0f, transform->GetTranslation().y));
			physics->SetVelocity(-physics->GetVelocity());
		}

		// Ladder movement
		MoveVertical();

		// Basic animation
		Animate();

		// Death seequence
		if (isDying)
		{
			DeathSequence(dt);
		}
	}

	// Map collision handler for flame and ground
	// Params:
	//   object = The flame.
	//   other  = The object the flame is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void FlamePointCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		UNREFERENCED_PARAMETER(object);

		FlameController* flameController = object.GetComponent<FlameController>();

		// Check whether the flame touched a beam or not
		if (other.GetComponent<ColliderLine>() != nullptr)
		{
			// Find the displacement vector from our point's position and the intersection's position
			Transform* transform = object.GetComponent<Transform>();
			Physics* physics = object.GetComponent<Physics>();
			ColliderPoint* point = object.GetComponent<ColliderPoint>();
			Vector2D currentPosition = transform->GetTranslation() + point->GetOffset();
			Vector2D displacementVector = intersection - currentPosition;

			Vector2D velocity = physics->GetVelocity();
			if (displacementVector.y > 0 && velocity.y < 0)
			{
				velocity.y = 0;

				// Calculate nudge
				float yNudge = displacementVector.y * 2 - point->GetOffset().y;

				// Stop from falling vertically with nudge
				transform->SetTranslation(Vector2D(currentPosition.x, currentPosition.y + yNudge));
				physics->SetVelocity(velocity);
			}

			flameController->onGround = true;
		}
	}

	// Collision handler for flame.
	// Params:
	//   object = The flame.
	//   other  = The object the flame is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void CircleCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		UNREFERENCED_PARAMETER(intersection);

		FlameController* flameController = object.GetComponent<FlameController>();
		
		// Hammer destroys flame
		if (other.GetName() == "playerHammer")
		{
			flameController->isDying = true;
		}

		// Ladder handling
		if (other.GetName() == "Ladder")
		{
			// Reset ladderTimer
			flameController->ladderTimer = 0.0f;

			// Can't get on ladder if jumping or have hammer
			if (flameController->onGround)
			{
				// Make sure center of flame is on ladder (between collider extents)
				float otherExtentsX = other.GetComponent<ColliderRectangle>()->GetExtents().x;
				float otherTranslationX = other.GetComponent<Transform>()->GetTranslation().x;
				float flameTranslationX = flameController->transform->GetTranslation().x;

				// Check between extents
				if (flameTranslationX < otherTranslationX + otherExtentsX && flameTranslationX > otherTranslationX - otherExtentsX)
				{
					// Correct flame's translation to ladder
					// flameController->transform->SetTranslation(Vector2D(otherTranslationX, flameController->transform->GetTranslation().y));

					// flame is now touchingLadder
					flameController->touchingLadder = true;
				}
			}
		}

		// If the flame has not collided with a ladder in more
		// than a fraction of a second, treat it like a collision-ended event
		if (flameController->ladderTimer > 0.3f)
		{
			// Reset ladder
			flameController->touchingLadder = false;
			flameController->onLadder = false;
		}
	}

	// Saves object data to a file.
	void FlameController::Serialize(Parser& parser) const
	{
		parser.WriteVariable("flameMoveSpeed", flameMoveSpeed);
		parser.WriteVariable("gravity", gravity);
		parser.WriteVariable("animationSpeed", animationSpeed);
		parser.WriteVariable("directionCooldown", directionCooldown);
	}

	// Loads object data from a file.
	void FlameController::Deserialize(Parser& parser)
	{
		 parser.ReadVariable("flameMoveSpeed", flameMoveSpeed);
		 parser.ReadVariable("gravity", gravity);
		 parser.ReadVariable("animationSpeed", animationSpeed);
		 parser.ReadVariable("directionCooldown", directionCooldown);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Moves vertically based on input
	void FlameController::MoveVertical()
	{
		// Check for climbing ladder
		if (touchingLadder)
		{
			// 50% chance to climb ladder
			if (RandomRange(0, 1))
			{
				onLadder = true;
				onGround = false;

				// Velocity to climb
				physics->SetVelocity(Vector2D(0.0f, flameMoveSpeed));
			}

			// Get off ladder if on ground
			if (onGround)
			{
				onLadder = false;
			}
		}
	}

	// Controls basic animation
	void FlameController::Animate()
	{
		// Flip horizontally based on movement
		if (physics->GetVelocity().x < 0.0f)
		{
			transform->SetScale(originalScale);
		}
		else
		{
			transform->SetScale(Vector2D(-originalScale.x, originalScale.y));
		}
	}

	// Death animation, level reset, etc.
	void FlameController::DeathSequence(const float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Temporary solution:
		GetOwner()->Destroy();

		// TODO:
		// Stop time
	}
}

//------------------------------------------------------------------------------
