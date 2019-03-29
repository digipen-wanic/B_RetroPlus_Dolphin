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
#include "PlayerAnimation.h"

#include <Space.h>
#include <Transform.h>
#include <Physics.h>
#include <Animation.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	PlayerAnimation::PlayerAnimation()
		: Component("PlayerAnimation"),
		  walkStart(0), walkEnd(3),
		  jumpStart(3), landStart(4),
		  idleStart(0),
		  climbStart(10), climbEnd(12),
		  hammerStart(17), hammerEnd(20), hammerTopStart(18), hammerSideStart(19),
		  dyingStart(25), dyingEnd(29), deadStart(29)
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
		playerMovement = GetOwner()->GetComponent<Behaviors::PlayerMovement>();

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
		if (playerMovement->getOnGround() && currentState == StateJump)
		{
			nextState = StateLand;
		}

		// Check for ladder movement
		if (playerMovement->getOnLadder())
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
		switch (playerMovement->getHammerStatus())
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
		if (playerMovement->getDeathStatus() == 1)
		{
			nextState = StateDying;
		}

		// Dead (halo frame)
		else if (playerMovement->getDeathStatus() == 2)
		{
			nextState = StateDead;
		}
	}

	// Change states and start the appropriate animation.
	void PlayerAnimation::ChangeCurrentState()
	{
		if (currentState != nextState)
		{
			currentState = nextState;
			switch (currentState)
			{
			// Play idle animation
			case StateIdle:
				animation->Play(idleStart, 1, 1, false);
				break;

			// Play walking animation
			case StateWalk:
				animation->Play(walkStart, walkEnd, 0.1f, true);
				break;

			// Play jumping animation
			case StateJump:
				animation->Play(jumpStart, 1, 1, false);
				break;

			// Play landing animation
			case StateLand:
				animation->Play(landStart, 1, 1, false);
				break;

			// Climbing ladder
			case StateClimbing:
				animation->Play(climbStart, climbEnd, 0.3f, true);
				break;

			// Stopping on ladder
			case StateClimbStop:
				animation->Play(climbStart, 1, 1, false);
				break;

			// Done climbing to top of ladder
			case StateDoneClimbing:
				
				// TODO: animation

				break;

			// Hammer idle/walk
			case StateHasHammer:
				animation->Play(hammerStart, hammerEnd, 0.05f, true);
				break;
				
			// Hit a barrel on the top
			case StateHitBarrelTop:
				animation->Play(hammerTopStart, 1, 1, false);
				break;

			// Hit a barrel on the side
			case StateHitBarrelSide:
				animation->Play(hammerSideStart, 1, 1, false);
				break;

			// Dying (rotating animation)
			case StateDying:
				animation->Play(dyingStart, dyingEnd, 0.15f, true);
				break;

			// Dead (halo frame)
			case StateDead:
				animation->Play(deadStart, 1, 1, false);
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