//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.h
// Author(s):	Jeremy Kings (j.kings)
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

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	// Map collision handler for Player objects.
	// Params:
	//   object = The Player object.
	//   collision = Which sides the Player collided on.
	void PlayerMapCollisionHandler(GameObject& object,
		const MapCollision& collision);

	// Collision handler for Player.
	// Params:
	//   object = The Player.
	//   other  = The object the Player is colliding with.
	void PlayerCollisionHandler(GameObject& object, GameObject& other);

	class PlayerMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerMovement();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Map collision handler for Player objects.
		// Params:
		//   object = The Player object.
		//   collision = Which sides the Player collided on.
		friend void PlayerMapCollisionHandler(GameObject& object, const MapCollision& collision);

		// Collision handler for Player.
		// Params:
		//   object = The Player.
		//   other  = The object the Player is colliding with.
		friend void PlayerCollisionHandler(GameObject& object, GameObject& other);

		// Saves object data to a file.
		virtual void Serialize(Parser& parser) const override;

		// Loads object data from a file.
		virtual void Deserialize(Parser& parser) override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal() const;

		// Moves vertically based on input
		void MoveVertical();

		// Animate player based on movements
		void Animate() const;

		// Hammer sequence/timer
		void HammerSequence(const float dt);

		// Death animation, level reset, etc.
		void DeathSequence(const float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Movement properties
		float PlayerWalkSpeed;
		float PlayerJumpSpeed;
		float PlayerClimbSpeed;
		Vector2D gravity;

		// Components
		Transform* transform;
		Vector2D playerScale;
		Physics* physics;

		// Jumping
		bool onGround;

		// Climbing
		bool touchingLadder;
		bool onLadder;

		// Hammer
		bool hasHammer;
		float hammerCooldown;

		// Death
		bool isDying;
		float deathDuration;
	};
}

//------------------------------------------------------------------------------
