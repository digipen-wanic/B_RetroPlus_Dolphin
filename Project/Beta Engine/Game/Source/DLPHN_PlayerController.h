//------------------------------------------------------------------------------
//
// File Name:	PlayerController.h
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

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Prototypes:
	//------------------------------------------------------------------------------

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
	//   intersection = The intersection between both objects, but only works with line collisions
	void PlayerCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	// Collision handler for hammer.
	// Params:
	//   object = The hammer.
	//   other  = The object the Player is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void HammerCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	class PlayerController : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerController();

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
		//   intersection = The intersection between both objects, but only works with line collisions
		friend void PlayerCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

		// Collision handler for hammer.
		// Params:
		//   object = The hammer.
		//   other  = The object the Player is colliding with.
		//   intersection = The intersection between both objects, but only works with line collisions
		friend void HammerCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

		// Saves object data to a file.
		virtual void Serialize(Parser& parser) const override;

		// Loads object data from a file.
		virtual void Deserialize(Parser& parser) override;

		// Returns whether the player is grounded
		bool getOnGround() const;

		// Returns whether the player is on a ladder
		bool getOnLadder() const;

		// Returns hammer status
		// (0 = No hammer, 1 = Has hammer, 2 = Hit barrel on top, 3 = Hit barrel on side)
		unsigned getHammerStatus() const;

		// Returns player's death status
		// (0 = Alive, 1 = Dying, 2 = Dead)
		unsigned getDeathStatus() const;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal() const;

		// Moves vertically based on input
		void MoveVertical();

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
		Physics* physics;
		float circleOffset;
		GameObject* playerHammer;

		// Jumping
		bool onGround;

		// Climbing
		bool touchingLadder;
		bool onLadder;

		// Hammer
		unsigned hammerStatus;
		float hammerCooldown;

		// Death
		unsigned deathStatus;
		float deathDuration;
	};
}

//------------------------------------------------------------------------------
