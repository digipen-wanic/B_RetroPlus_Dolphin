//------------------------------------------------------------------------------
//
// File Name:	PlayerAnimation.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

#include "Vector2D.h" // Vector2D
#include "DLPHN_PlayerController.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Animation;
class Physics;
class Transform;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	class PlayerAnimation : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerAnimation();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Fixed update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Choose the correct state based on velocity.
		void ChooseNextState(float dt);

		// Change states and start the appropriate animation.
		void ChangeCurrentState();

		// Flip the sprite based on velocity and current state.
		void FlipSprite() const;

		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		enum State
		{
			StateIdle,

			StateWalk,

			StateJump,
			StateLand,

			StateClimbing,
			StateClimbStop,
			StateDoneClimbing,

			StateHasHammer,
			StateHitBarrelTop,
			StateHitBarrelSide,

			StateDying,
			StateDead
		};

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Animation variables
		unsigned idleStart;

		unsigned walkStart;
		unsigned walkEnd;

		unsigned jumpStart;
		unsigned landStart;

		unsigned climbStart;
		unsigned climbEnd;

		unsigned hammerStart;
		unsigned hammerEnd;
		unsigned hammerSideStart;
		unsigned hammerTopStart;

		unsigned dyingStart;
		unsigned dyingEnd;
		unsigned deadStart;

		// Animation state
		State currentState;
		State nextState;

		// Components
		Animation* animation;
		Physics* physics;
		Transform* transform;
		PlayerController* playerController;

		// Flip
		Vector2D originalScale;
	};
}

//------------------------------------------------------------------------------
