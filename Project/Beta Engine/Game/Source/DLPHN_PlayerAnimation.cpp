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
		  climbStart(10), climbEnd(11),
		  hammerStart(17), hammerEnd(19), hammerTopStart(18), hammerSideStart(19),
		  dyingStart(25), dyingEnd(28), deadStart(29)
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
		PlayerController = GetOwner()->GetComponent<DLPHN::PlayerController>();

		// initialize with starting values
		animation->Play(idleStart, 1, 1, false);
		originalScale = transform->GetScale();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		ChooseNextState();
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
	void PlayerAnimation::ChooseNextState()
	{
		// jump if in air and not on ladder
		if (physics->GetVelocity().y != 0.0f)
		{
			nextState = StateJump;
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
		if (PlayerController->getOnGround() && currentState == StateJump)
		{
			nextState = StateLand;
		}

		// Check for ladder movement
		if (PlayerController->getOnLadder())
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
		switch (PlayerController->getHammerStatus())
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
		if (PlayerController->getDeathStatus() == 1)
		{
			nextState = StateDying;
		}

		// Dead (halo frame)
		else if (PlayerController->getDeathStatus() == 2)
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
					// Dear Daniel,
					// Turns out that Jumpman doesn't have an idle animation, he just stops whatever animation he had while walking.
					// Sincerely,
					// David Wong
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
				animation->Play(0.1f, walkStart, walkEnd, true);
				break;

			// Play jumping animation
			case StateJump:
				animation->Play(1, jumpStart, 1, false);
				break;

			// Play landing animation
			case StateLand:
				animation->Play(1, landStart, 1, false);
				break;

			// Climbing ladder
			case StateClimbing:
				animation->Play(0.3f, climbStart, climbEnd, true);
				break;

			// Stopping on ladder
			case StateClimbStop:
				animation->Play(1, climbStart, 1, false);
				break;

			// Done climbing to top of ladder
			case StateDoneClimbing:
				
				// TODO: animation

				break;

			// Hammer idle/walk
			case StateHasHammer:
				animation->Play(0.05f, hammerStart, hammerEnd, true);
				break;
				
			// Hit a barrel on the top
			case StateHitBarrelTop:
				animation->Play(1, hammerTopStart, 1, false);
				break;

			// Hit a barrel on the side
			case StateHitBarrelSide:
				animation->Play(1, hammerSideStart, 1, false);
				break;

			// Dying (rotating animation)
			case StateDying:
				animation->Play(0.15f, dyingStart, dyingEnd, true);
				break;

			// Dead (halo frame)
			case StateDead:
				animation->Play(1, deadStart, 1, false);
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