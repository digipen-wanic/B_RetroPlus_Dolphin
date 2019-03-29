//------------------------------------------------------------------------------
//
// File Name:	PlayerController.cpp
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
#include "PlayerController.h"

#include "Input.h"
#include "Parser.h"
#include "Physics.h"
#include "Space.h"
#include "Transform.h"

#include "ColliderRectangle.h"
#include "ColliderCircle.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PlayerController::PlayerController()
		: Component("PlayerController"),
		PlayerWalkSpeed(150.0f),
		PlayerJumpSpeed(1500.0f),
		PlayerClimbSpeed(100.0f),
		gravity(0.0f, -300.0f),
		onGround(false),
		touchingLadder(false), onLadder(false),
		hammerStatus(0), hammerCooldown(10.0f),
		deathStatus(0), deathDuration(4.0f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerController::Clone() const
	{
		return new PlayerController(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerController::Initialize()
	{
		// get object components
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
		
		// Save offset and hide circle in player
		circleOffset = GetOwner()->GetComponent<ColliderCircle>()->GetOffset();
		GetOwner()->GetComponent<ColliderCircle>()->SetOffset(Vector2D(0.0f, 0.0f));

		// set map collision handler
		GetOwner()->GetComponent<Collider>()->SetMapCollisionHandler(&PlayerMapCollisionHandler);

		// set player collision handler
		GetOwner()->GetComponent<ColliderRectangle>()->SetCollisionHandler(&PlayerCollisionHandler);
		GetOwner()->GetComponent<ColliderCircle>()->SetCollisionHandler(&HammerCollisionHandler);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerController::Update(float dt)
	{
		// Death sequence
		if (deathStatus)
		{
			DeathSequence(dt);
		}

		// Not dying
		else
		{
			// Horizontal walking
			MoveHorizontal();

			// Hammer sequence
			if (hammerStatus)
			{
				HammerSequence(dt);
			}
			// Can't jump or climb if hammer is active
			else
			{
				MoveVertical();
			}

			// Add gravity to player (if not on ladder)
			if (!onLadder)
			{
				physics->AddForce(gravity);
			}
		}
	}

	// Map collision handler for Player objects.
	// Params:
	//   object = The Player object.
	//   collision = Which sides the Player collided on.
	void PlayerMapCollisionHandler(GameObject& object, const MapCollision& collision)
	{
		// if there is a bottom collision, set onGround to true
		if (collision.bottom)
		{
			object.GetComponent<PlayerController>()->onGround = true;
		}
	}

	// Collision handler for Player.
	// Params:
	//   object = The Player.
	//   other  = The object the Player is colliding with.
	void PlayerCollisionHandler(GameObject& object, GameObject& other)
	{
		UNREFERENCED_PARAMETER(object);

		PlayerController* playerController = object.GetComponent<PlayerController>();

		// Make sure player doesn't have hammer
		if (!playerController->hammerStatus)
		{
			// Destroy and collect hammer
			if (other.GetName() == "Hammer")
			{
				other.Destroy();
				playerController->hammerStatus = 1;
			}

			if (other.GetName() == "Ladder")
			{
				// Can't get on ladder if jumping or have hammer
				if (playerController->onGround)
				{
					// Make sure center of player is on ladder (between collider extents)
					float otherExtentsX = other.GetComponent<ColliderRectangle>()->GetExtents().x;
					float otherTranslationX = other.GetComponent<Transform>()->GetTranslation().x;
					float playerTranslationX = playerController->transform->GetTranslation().x;

					// Check between extents
					if (playerTranslationX < otherTranslationX + otherExtentsX && playerTranslationX > otherTranslationX - otherExtentsX)
					{
						// Correct player's translation to ladder
						// PlayerController->transform->SetTranslation(Vector2D(otherTranslationX, PlayerController->transform->GetTranslation().y));

						// player is now touchingLadder
						playerController->touchingLadder = true;
					}
				}
			}
		}

		// Hazard objects
		if (other.GetName() == "Barrel" || other.GetName() == "FireDude")
		{
			// Set status to dying if not already
			if (!playerController->deathStatus)
			{
				playerController->deathStatus = 1;
			}
		}
	}

	// Collision handler for hammer.
	// Params:
	//   object = The hammer.
	//   other  = The object the Player is colliding with.
	void HammerCollisionHandler(GameObject& object, GameObject& other)
	{
		UNREFERENCED_PARAMETER(object);

		// Hazard objects
		if (other.GetName() == "Barrel" || other.GetName() == "FireDude")
		{
			// TODO:
			// Play destroy barrel animation
			other.Destroy();
		}
	}

	// Saves object data to a file.
	void PlayerController::Serialize(Parser& parser) const
	{
		// Other stuff?

		parser.WriteVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.WriteVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.WriteVariable("gravity", gravity);
	}

	// Loads object data from a file.
	void PlayerController::Deserialize(Parser& parser)
	{
		// Other stuff?

		parser.ReadVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.ReadVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.ReadVariable("gravity", gravity);
	}

	// Returns whether the player is grounded
	bool PlayerController::getOnGround() const
	{
		return onGround;
	}

	// Returns whether the player is on a ladder
	bool PlayerController::getOnLadder() const
	{
		return onLadder;
	}

	// Returns whether the player has a hammer
	// (0 = No hammer, 1 = Has hammer, 2 = Hit barrel on top, 3 = Hit barrel on side)
	unsigned PlayerController::getHammerStatus() const
	{
		return hammerStatus;
	}

	// Returns player's death status
	// (0 = Alive, 1 = Dying, 2 = Dead)
	unsigned PlayerController::getDeathStatus() const
	{
		return deathStatus;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Moves horizontally based on input
	void PlayerController::MoveHorizontal() const
	{
		Input& input = Input::GetInstance();

		// Don't move horizontally if jumping or in the air
		if (onGround)
		{
			// Move right with right arrow
			if (input.CheckHeld(VK_RIGHT) && !input.CheckHeld(VK_LEFT))
			{
				physics->SetVelocity(Vector2D(PlayerWalkSpeed, physics->GetVelocity().y));
			}

			// Move left with left arrow
			else if (input.CheckHeld(VK_LEFT) && !input.CheckHeld(VK_RIGHT))
			{
				physics->SetVelocity(Vector2D(-PlayerWalkSpeed, physics->GetVelocity().y));
			}

			// Idle
			else
			{
				physics->SetVelocity(Vector2D(0.0f, physics->GetVelocity().y));
			}
		}
	}

	// Moves vertically based on input
	void PlayerController::MoveVertical()
	{
		Input& input = Input::GetInstance();

		// Check for climbing ladder
		if (touchingLadder)
		{
			// Going up ladder
			if (input.CheckHeld(VK_UP))
			{
				onLadder = true;
				onGround = false;

				// Velocity to climb
				physics->SetVelocity(Vector2D(0.0f, PlayerClimbSpeed));
			}
			else if (input.CheckHeld(VK_DOWN))
			{
				// Get off ladder if on ground
				if (onGround)
				{
					onLadder = false;
				}

				// Move down ladder
				else
				{
					physics->SetVelocity(Vector2D(physics->GetVelocity().x, -PlayerClimbSpeed));
				}
			}
			// Don't move if on ladder and no vertical movement
			else if (onLadder)
			{
				physics->SetVelocity(Vector2D(0.0f, 0.0f));
			}
		}

		// Check for jump
		else if (input.CheckTriggered(VK_UP) && onGround)
		{
			onGround = false;

			// Set jumping velocity
			physics->SetVelocity(Vector2D(physics->GetVelocity().x, PlayerJumpSpeed));
		}

		// Set onGround to false if player is moving vertically
		if (physics->GetVelocity().y != 0)
		{
			onGround = false;
		}

		// Not touching/on ladder if the player is moving horizontally
		if (physics->GetVelocity().x != 0.0f)
		{
			onLadder = false;
			touchingLadder = false;
		}
	}

	// Hammer sequence/timer
	void PlayerController::HammerSequence(const float dt)
	{
		// Hammer timer
		static float timer = 0.0f;
		timer += dt;

		// Offset circle to match hammer
		if (physics->GetVelocity().x > 0)
		{
			GetOwner()->GetComponent<ColliderCircle>()->SetOffset(circleOffset);
		}
		else if (physics->GetVelocity().x < 0)
		{
			GetOwner()->GetComponent<ColliderCircle>()->SetOffset(Vector2D(-circleOffset.x, circleOffset.y));
		}

		// Only run hammer for hammerCooldown seconds
		if (timer >= hammerCooldown)
		{
			// Hide collidercircle in player
			GetOwner()->GetComponent<ColliderCircle>()->SetOffset(Vector2D(0.0f, 0.0f));

			// Reset variables
			timer = 0.0f;
			hammerStatus = 0;
		}
	}

	// Death animation, level reset, etc.
	void PlayerController::DeathSequence(const float dt)
	{
		// Death timer
		static float timer = 0.0f;
		timer += dt;

		// Stop player from moving
		physics->SetVelocity(Vector2D(0.0f, 0.0f));

		// Die after a few seconds
		if (timer >= deathDuration - 2.0f)
			deathStatus = 2;

		// Restart level and finish stuff up
		if (timer >= deathDuration)
		{
			// Restart and do other stuff here
			timer = 0.0f;
			GetOwner()->GetSpace()->RestartLevel();
		}

		// DO OTHER STUFF HERE
	}
}