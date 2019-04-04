//------------------------------------------------------------------------------
//
// File Name:	FlameController.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class Animation;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Prototypes:
	//------------------------------------------------------------------------------

	// Map collision handler for flame and ground
	// Params:
	//   object = The flame.
	//   other  = The object the flame is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void FlamePointCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	// Collision handler for flame.
	// Params:
	//   object = The flame.
	//   other  = The object the flame is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void CircleCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	class FlameController : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		FlameController();

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

		// Map collision handler for flame and ground
		// Params:
		//   object = The flame.
		//   other  = The object the flame is colliding with.
		//   intersection = The intersection between both objects, but only works with line collisions
		friend void FlamePointCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

		// Collision handler for flame.
		// Params:
		//   object = The flame.
		//   other  = The object the flame is colliding with.
		//   intersection = The intersection between both objects, but only works with line collisions
		friend void CircleCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

		// Saves object data to a file.
		virtual void Serialize(Parser& parser) const override;

		// Loads object data from a file.
		virtual void Deserialize(Parser& parser) override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves vertically based on input
		void MoveVertical();

		// Controls basic animation
		void Animate();

		// Death animation, level reset, etc.
		void DeathSequence(const float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Movement properties
		float flameMoveSpeed;
		Vector2D gravity;

		// Components
		Transform* transform;
		Physics* physics;
		Animation* animation;

		// Animation
		Vector2D originalScale;
		float animationSpeed;

		// Walking
		float directionTimer;
		float directionCooldown;

		// Jumping
		bool onGround;

		// Climbing
		bool touchingLadder;
		bool onLadder;
		float ladderTimer;

		// Death
		bool isDying;
	};
}

//------------------------------------------------------------------------------
