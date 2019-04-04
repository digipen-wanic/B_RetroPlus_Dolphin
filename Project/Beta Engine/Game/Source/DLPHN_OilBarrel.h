//------------------------------------------------------------------------------
//
// File Name:	OilBarrel.h
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
class Animation;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Prototypes:
	//------------------------------------------------------------------------------

	// Collision handler for oil barrel
	// Params:
	//   object = The oil barrel.
	//   other  = The object the oil barrel is colliding with.
	//   intersection = The intersection between both objects, but only works with line collisions
	void OilBarrelCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	class OilBarrel : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		OilBarrel();

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

		// Collision handler for oil barrel
		// Params:
		//   object = The oil barrel.
		//   other  = The object the oil barrel is colliding with.
		//   intersection = The intersection between both objects, but only works with line collisions
		friend void OilBarrelCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

		// Saves object data to a file.
		virtual void Serialize(Parser& parser) const override;

		// Loads object data from a file.
		virtual void Deserialize(Parser& parser) override;

		// Returns whether the oil barrel is on fire
		bool isFlaming() const;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Spawns a flame
		void SpawnFlame();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Animation* animation;
		Transform* transform;

		// Flame
		GameObject* flameArchetype;

		// Animation
		float animationTimer;
		float animationSpeed;

		bool flaming;
	};
}

//------------------------------------------------------------------------------
