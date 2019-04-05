//------------------------------------------------------------------------------
//
// File Name:	PlayerAnimation.cpp
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
#include "DLPHN_PlayerAnimation.h"

#include <Space.h>
#include <Sprite.h>
#include <SpriteSource.h>
#include <Transform.h>
#include <Physics.h>
#include <Animation.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	PlayerAnimation::PlayerAnimation()
		: Component("PlayerAnimation"),
		  walkStart(0), walkEnd(2),
		  jumpStart(3), landStart(4),
		  idleStart(0),
		  climbStart(5), climbEnd(6),
		  hammerStart(13), hammerEnd(16), hammerTopStart(16), hammerSideStart(15),
		  dyingStart(17), dyingEnd(20), deadStart(21)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerAnimation::Clone() const
	{
		return new PlayerAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerAnimation::Initialize()
	{
		// create animation, physics, and transform objects
		animation = GetOwner()->GetComponent<Animation>();
		physics = GetOwner()->GetComponent<Physics>();
		transform = GetOwner()->GetComponent<Transform>();
		playerController = GetOwner()->GetComponent<DLPHN::PlayerController>();

		// Correct values if using plus animation (if needed)
		if (GetOwner()->GetComponent<Sprite>()->GetSpriteSource()->GetName() == "PlayerPlus")
		{
			walkEnd = 1;
			jumpStart = 2;
			landStart = 2;
			climbStart = 6;
			climbEnd = 7;
			hammerStart = 3;
			hammerEnd = 5;
			dyingStart = 8;
			dyingEnd = 8;
			deadStart = 8;
		}

		// initialize with starting values
		animation->Play(1, idleStart, 1, false);
		originalScale = transform->GetScale();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		ChooseNextState(dt);
		ChangeCurrentState();
		FlipSprite();
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void PlayerAnimation::Serialize(Parser& parser) const
	{
		UNREFERENCED_PARAMETER(parser);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void PlayerAnimation::Deserialize(Parser& parser)
	{
		UNREFERENCED_PARAMETER(parser);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void PlayerAnimation::ChooseNextState(float dt)
	{
		// jump if in air and not on ladder
		if (!playerController->getOnGround() && !playerController->getOnLadder())
		{
			static float timer = 0.0f;
			timer += dt;

			// Ensure player is actually in the air
			if (timer > 0.1f)
			{
				timer = 0.0f;
				nextState = StateJump;
			}
		}

		// walk if moving horizontally
		else if (physics->GetVelocity().x != 0.0f)
		{
			nextState = StateWalk;
		}
		// idle if not moving horizontally
		else
		{
			nextState = StateIdle;
		}

		// Landing from jumping
		if (playerController->getOnGround() && currentState == StateJump)
		{
			nextState = StateLand;
		}

		// Check for ladder movement
		if (playerController->getOnLadder())
		{
			// Make sure player is moving to animate
			if (physics->GetVelocity().y != 0.0f)
			{
				nextState = StateClimbing;
			}
			// Stop animating ladder movement
			else
			{
				nextState = StateClimbStop;
			}
		}
		
		// Hammer animation
		switch (playerController->getHammerStatus())
		{
			// Has hammer
		case 1:
			nextState = StateHasHammer;
			break;

			// Hit barrel on the top
		case 2:
			nextState = StateHitBarrelTop;
			break;

			// Hit barrel on side
		case 3:
			nextState = StateHitBarrelSide;
			break;
		}

		// Currently dying (spinning animation)
		if (playerController->getDeathStatus() == 1)
		{
			nextState = StateDying;
		}

		// Dead (halo frame)
		else if (playerController->getDeathStatus() == 2)
		{
			nextState = StateDead;
		}
	}

	// Change states and start the appropriate animation.
	void PlayerAnimation::ChangeCurrentState()
	{
		if (currentState != nextState)
		{
			State prevState = currentState;
			currentState = nextState;
			switch (currentState)
			{
			// Play idle animation
			case StateIdle:
				if (prevState == StateWalk)
				{
					// Turns out that Jumpman doesn't have an idle animation, he just stops whatever animation he had while walking.
					animation->Stop();
				}
				else
				{
					//animation->Play(1, idleStart, 1, false);
					animation->Play(1, idleStart, idleStart, false);
				}
				break;

			// Play walking animation
			case StateWalk:
				animation->Play(0.04f, walkStart, walkEnd, true);
				break;

			// Play jumping animation
			case StateJump:
				animation->Play(1, jumpStart, jumpStart, false);
				break;

			// Play landing animation
			case StateLand:
				animation->Play(1, landStart, landStart, false);
				break;

			// Climbing ladder
			case StateClimbing:
				animation->Play(0.3f, climbStart, climbEnd, true);
				break;

			// Stopping on ladder
			case StateClimbStop:
				animation->Stop();
				break;

			// Done climbing to top of ladder
			case StateDoneClimbing:
				
				// TODO: animation

				break;

			// Hammer idle/walk
			case StateHasHammer:
				animation->Play(0.08f, hammerStart, hammerEnd, true);
				break;
				
			// Hit a barrel on the top
			case StateHitBarrelTop:
				animation->Play(1, hammerTopStart, hammerTopStart, false);
				break;

			// Hit a barrel on the side
			case StateHitBarrelSide:
				animation->Play(1, hammerSideStart, hammerSideStart, false);
				break;

			// Dying (rotating animation)
			case StateDying:
				animation->Play(0.15f, dyingStart, dyingEnd, true);
				break;

			// Dead (halo frame)
			case StateDead:
				animation->Play(1, deadStart, deadStart, false);
				break;
			}
		}
	}

	// Flip the sprite based on velocity and current state.
	void PlayerAnimation::FlipSprite() const
	{
		if (currentState != StateIdle)
		{
			// flip in -x
			if (physics->GetVelocity().x < 0)
			{
				transform->SetScale(Vector2D(-originalScale.x, originalScale.y));
			}

			// flip in +x
			else if (physics->GetVelocity().x > 0)
			{
				transform->SetScale(originalScale);
			}
		}
	}
}

//------------------------------------------------------------------------------