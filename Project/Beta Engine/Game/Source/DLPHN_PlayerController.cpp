//------------------------------------------------------------------------------
//
// File Name:	PlayerController.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DLPHN_PlayerController.h"

#include <ColliderCircle.h>
#include <ColliderLine.h>
#include <ColliderPoint.h>
#include <ColliderRectangle.h>
#include <Engine.h>
#include <Input.h>
#include <Level.h>
#include <Parser.h>
#include <Physics.h>
#include <SoundManager.h>
#include <Space.h>
#include <Sprite.h>
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
	PlayerController::PlayerController()
		: Component("PlayerController"),
		PlayerWalkSpeed(150.0f),
		PlayerJumpSpeed(1500.0f),
		PlayerClimbSpeed(100.0f),
		gravity(0.0f, -300.0f),
		walkSoundTimer(0.0f),
		onGround(false),
		touchingLadder(false), onLadder(false), ladderTimer(0.0f),
		hammerStatus(0), hammerCooldown(10.0f),
		deathStatus(0), deathDuration(4.0f),
		playerHasWon(false)
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
		sprite = GetOwner()->GetComponent<Sprite>();

		// Get collider for player hammer
		playerHammer = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PlayerHammer");

		// Save offset and hide circle in player
		circleOffset = playerHammer->GetComponent<ColliderCircle>()->GetOffset().x;
		playerHammer->GetComponent<ColliderCircle>()->SetOffset(Vector2D(0.0f, 0.0f));

		// set player collision handlers
		ColliderPoint* pointCollider = GetOwner()->GetComponent<ColliderPoint>();
		pointCollider->SetCollisionHandler(&PointCollisionHandler);

		ColliderRectangle* rectangleCollider = GetOwner()->GetComponent<ColliderRectangle>();
		rectangleCollider->SetCollisionHandler(&RectangleCollisionHandler);

		ColliderCircle* hammerCollider = playerHammer->GetComponent<ColliderCircle>();
		hammerCollider->SetCollisionHandler(&HammerCollisionHandler);

		// Set up for sound effects
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->AddEffect("DLPHN_sound_win.wav");
		soundManager->AddEffect("DLPHN_sound_death.wav");
		soundManager->AddEffect("DLPHN_sound_jump.wav");
		soundManager->AddEffect("DLPHN_music_hammer.wav");
		soundManager->AddEffect("DLPHN_sound_hammerBoop.wav");
		soundManager->AddEffect("DLPHN_sound_movestandin.wav");
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The change in time since the last step.
	void PlayerController::Update(float dt)
	{
		// Keep playerHammer collider with player
		playerHammer->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
		
		// Increment timers
		ladderTimer += dt;
		walkSoundTimer += dt;

		// Death sequence
		if (deathStatus)
		{
			DeathSequence(dt);
		}

		// Not dying
		else if (!playerHasWon)
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

		// Check for win
		if (playerHasWon)
		{
			// Countdown to play win effect
			static float timer = 4.0f;

			// Stop player from moving
			physics->SetVelocity(Vector2D(0.0f, 0.0f));

			// Only play sound once
			if (timer == 4.0f)
			{
				soundManager->PlaySound("DLPHN_sound_win.wav");
			}

			timer -= dt;

			// Quit game after a few seconds
			if (timer <= 0.0f)
			{
				Engine::GetInstance().Stop();
			}
		}
	}

	// Map collision handler for player and ground
	// Params:
	//   object = The Player.
	//   other  = The object the Player is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void PointCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		UNREFERENCED_PARAMETER(object);

		PlayerController* playerController = object.GetComponent<PlayerController>();

		// Check whether the player touched a beam or not
		if (other.GetComponent<ColliderLine>() != nullptr)
		{
			// Janky Resolution Code
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

				// Solve the simple problem first and stop the player from falling vertically
				float yNudge = displacementVector.y * 2 - point->GetOffset().y;
				transform->SetTranslation(Vector2D(currentPosition.x, currentPosition.y + yNudge));
				physics->SetVelocity(velocity);
			}

			playerController->onGround = true;
		}
	}

	// Collision handler for Player.
	// Params:
	//   object = The Player.
	//   other  = The object the Player is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void RectangleCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		UNREFERENCED_PARAMETER(object);
		UNREFERENCED_PARAMETER(intersection);

		PlayerController* playerController = object.GetComponent<PlayerController>();

		if (other.GetName() == "WinZone")
		{
			playerController->playerHasWon = true;
		}

		// Make sure player doesn't have hammer
		if (!playerController->hammerStatus)
		{
			// Destroy and collect hammer
			if (other.GetName() == "Hammer")
			{
				other.Destroy();
				playerController->hammerStatus = 1;
			}

			// Ladder handling
			if (other.GetName() == "Ladder")
			{
				// Reset ladderTimer
				playerController->ladderTimer = 0.0f;

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
						// playerController->transform->SetTranslation(Vector2D(otherTranslationX, playerController->transform->GetTranslation().y));

						// player is now touchingLadder
						playerController->touchingLadder = true;
					}
				}
			}

			// If the player has not collided with a ladder in more
			// than a fraction of a second, treat it like a collision-ended event
			if (playerController->ladderTimer > 0.3f)
			{
				// Reset ladder
				playerController->touchingLadder = false;
				playerController->onLadder = false;
			}
		}

		// Hazard objects
		if (!playerController->playerHasWon && other.GetName() == "Barrel" || other.GetName() == "FireDude")
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
	void HammerCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		UNREFERENCED_PARAMETER(object);
		UNREFERENCED_PARAMETER(intersection);
		
		PlayerController* playerController = object.GetSpace()->GetObjectManager().GetObjectByName("Player")->GetComponent<PlayerController>();

		// Hazard objects
		if (!playerController->getDeathStatus() && other.GetName() == "Barrel" || other.GetName() == "FireDude")
		{
			// TODO: Stop time
			
			// Play destruction effect
			playerController->soundManager->PlaySound("DLPHN_sound_hammerBoop.wav");
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
	void PlayerController::MoveHorizontal()
	{
		Input& input = Input::GetInstance();

		// Don't move horizontally if jumping or in the air
		if (onGround)
		{
			// Move right with right arrow
			if (input.CheckHeld(VK_RIGHT) && !input.CheckHeld(VK_LEFT))
			{
				physics->SetVelocity(Vector2D(PlayerWalkSpeed, physics->GetVelocity().y));
				//transform->SetTranslation(Vector2D(transform->GetTranslation().x + PlayerWalkSpeed, transform->GetTranslation().y));

				if (walkSoundTimer > 0.15f)
				{
					walkSoundTimer = 0.0f;
					soundManager->PlaySound("DLPHN_sound_movestandin.wav");
				}
			}

			// Move left with left arrow
			else if (input.CheckHeld(VK_LEFT) && !input.CheckHeld(VK_RIGHT))
			{
				physics->SetVelocity(Vector2D(-PlayerWalkSpeed, physics->GetVelocity().y));
				//transform->SetTranslation(Vector2D(transform->GetTranslation().x - PlayerWalkSpeed, transform->GetTranslation().y));

				if (walkSoundTimer > 0.15f)
				{
					walkSoundTimer = 0.0f;
					soundManager->PlaySound("DLPHN_sound_movestandin.wav");
				}
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

			// Play jump sound
			soundManager->PlaySound("DLPHN_sound_jump.wav");
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
		static float soundTimer = 0.0f;
		timer += dt;
		soundTimer += dt;

		// Stop normal music channel once
		//if (timer == 0.0f + dt)
		//{
		//	soundManager->GetMusicChannel()->stop();
		//}

		// Play hammer music on loop
		if (soundTimer >= 2.725f || timer == 0.0f + dt)
		{
			soundTimer = 0.0f;

			soundManager->PlaySound("DLPHN_music_hammer.wav");
		}

		// Determine what frame is being displayed
		// (13, 15: side) (14, 16: top)
		switch (sprite->GetFrame())
		{
		case 13:
		case 15:
			// Determine what direction player is facing
			if (transform->GetScale().x < 0)
			{
				playerHammer->GetComponent<ColliderCircle>()->SetOffset(Vector2D(-circleOffset, 0.0f));
			}
			else
			{
				playerHammer->GetComponent<ColliderCircle>()->SetOffset(Vector2D(circleOffset, 0.0f));
			}
			break;

		case 14:
		case 16:
			playerHammer->GetComponent<ColliderCircle>()->SetOffset(Vector2D(0.0f, circleOffset));
			break;
		}

		// Only run hammer for hammerCooldown seconds
		if (timer >= hammerCooldown)
		{
			// Hide collidercircle in player
			playerHammer->GetComponent<ColliderCircle>()->SetOffset(Vector2D(0.0f, 0.0f));

			// Play normal music
			soundManager->GetMusicChannel()->stop();
			soundManager->PlaySound("DLPHN_music_theme.wav");

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

		// Play death sound once
		if (timer == 0.0f + dt)
		{
			soundManager->PlaySound("DLPHN_sound_death.wav");
		}

		// Stop player from moving
		physics->SetVelocity(Vector2D(0.0f, 0.0f));

		// Die after a few seconds
		if (timer >= deathDuration - 2.0f)
			deathStatus = 2;

		// Restart level
		if (timer >= deathDuration)
		{
			timer = 0.0f;
			GetOwner()->GetSpace()->RestartLevel();
		}
	}
}