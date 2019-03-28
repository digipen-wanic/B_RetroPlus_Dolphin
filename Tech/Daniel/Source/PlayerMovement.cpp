//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.cpp
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
#include "PlayerMovement.h"

#include "Input.h"
#include "Parser.h"
#include "Physics.h"
#include "Space.h"		
#include "Transform.h"

#include "ColliderRectangle.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PlayerMovement::PlayerMovement()
		: Component("PlayerMovement"),
		  PlayerWalkSpeed(150.0f),
		  PlayerJumpSpeed(1500.0f),
		  PlayerClimbSpeed(100.0f),
		  gravity(0.0f, -300.0f),
		  onGround(false),
		  touchingLadder(false), onLadder(false),
		  hasHammer(false), hammerCooldown(10.0f),
		  isDying(false), deathDuration(5.0f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerMovement::Clone() const
	{
		return new PlayerMovement(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerMovement::Initialize()
	{
		// get object components
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

		// Get initial scale of player
		playerScale = transform->GetScale();

		// set map collision handler
		GetOwner()->GetComponent<Collider>()->SetMapCollisionHandler(&PlayerMapCollisionHandler);

		// set Player collision handler
		GetOwner()->GetComponent<Collider>()->SetCollisionHandler(&PlayerCollisionHandler);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerMovement::Update(float dt)
	{
		// Movement
		MoveHorizontal();

		// Can't jump or climb if hammer is active
		if (!hasHammer)
		{
			MoveVertical();
		}

		// Animate player based on movement, etc.
		Animate();

		// Hammer sequence
		if (hasHammer)
		{
			HammerSequence(dt);
		}

		// Death sequence
		if (isDying)
		{
			DeathSequence(dt);
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
			object.GetComponent<PlayerMovement>()->onGround = true;
		}
	}

	// Collision handler for Player.
	// Params:
	//   object = The Player.
	//   other  = The object the Player is colliding with.
	void PlayerCollisionHandler(GameObject& object, GameObject& other)
	{
		UNREFERENCED_PARAMETER(object);

		PlayerMovement* playerMovement = object.GetComponent<PlayerMovement>();

		// if the other object is a hazard, restart the level
		if (other.GetName() == "Barrel" || other.GetName() == "FireDude")
		{
			playerMovement->isDying = true;
		}

		// Destroy and collect hammer
		if (other.GetName() == "Hammer")
		{
			other.Destroy();
			playerMovement->hasHammer = true;
		}

		if (other.GetName() == "Ladder")
		{
			// Can't get on ladder if jumping
			if (playerMovement->onGround)
			{
				// Make sure center of player is on ladder (between collider extents)
				float otherExtentsX = other.GetComponent<ColliderRectangle>()->GetExtents().x;
				float otherTranslationX = other.GetComponent<Transform>()->GetTranslation().x;
				float playerTranslationX = playerMovement->transform->GetTranslation().x;

				// Check between
				if (playerTranslationX < otherTranslationX + otherExtentsX
					&& playerTranslationX > otherTranslationX - otherExtentsX)
				{
					playerMovement->touchingLadder = true;
				}
			}
		}
	}

	// Saves object data to a file.
	void PlayerMovement::Serialize(Parser& parser) const
	{
		// Other stuff?

		parser.WriteVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.WriteVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.WriteVariable("gravity", gravity);
	}

	// Loads object data from a file.
	void PlayerMovement::Deserialize(Parser& parser)
	{
		// Other stuff?

		parser.ReadVariable("PlayerWalkSpeed", PlayerWalkSpeed);
		parser.ReadVariable("PlayerJumpSpeed", PlayerJumpSpeed);
		parser.ReadVariable("gravity", gravity);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Moves horizontally based on input
	void PlayerMovement::MoveHorizontal() const
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
	void PlayerMovement::MoveVertical()
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

		// Set touchingLadder to false if the player is moving horizontally
		if (physics->GetVelocity().x != 0.0f)
		{
			touchingLadder = false;
		}

		// Gravity affects player if not on a ladder
		if (!onLadder)
		{
			physics->AddForce(gravity);
		}
	}

	// Animate player based on movements
	void PlayerMovement::Animate() const
	{
		// Flip player's sprite to the direction of travel
		if (physics->GetVelocity().x > 0)
		{
			transform->SetScale(playerScale);
		}
		else if (physics->GetVelocity().x < 0)
		{
			transform->SetScale(Vector2D(-playerScale.x, playerScale.y));
		}
	}

	// Hammer sequence/timer
	void PlayerMovement::HammerSequence(const float dt)
	{
		// Hammer timer
		static float timer = 0.0f;
		timer += dt;

		// Only run hammer for hammerCooldown seconds
		if (timer >= hammerCooldown)
		{
			timer = 0.0f;
			hasHammer = false;
		}
	}

	// Death animation, level reset, etc.
	void PlayerMovement::DeathSequence(const float dt)
	{
		// Death timer
		static float timer = 0.0f;
		timer += dt;

		// Play death animation
		if (static_cast<int>(floor(timer)) % 2)
		{

		}

		if (timer >= deathDuration)
		{
			// Restart and do other stuff here
			timer = 0.0f;
			GetOwner()->GetSpace()->RestartLevel();
		}

		// DO OTHER STUFF HERE
	}
}