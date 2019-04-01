//============================================================
// Author: Jayden Yamada
// File Name: DLPHN_Barrel.h
// Date Created: March 29 2019
//============================================================

#pragma once

//============================================================
// Includes:
//============================================================

#include <Component.h>
#include <Serializable.h>
#include <Vector2D.h>

//============================================================
// Forward Declarations:
//============================================================

class Physics;
class Animation;
class Transform;

//============================================================

namespace DLPHN
{
	// Collision handler for the barrel.
	// Params:
	//   object = The barrel.
	//   other  = The object the barrel is colliding with.
	void BarrelCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	class Barrel : public Component
	{
		public :
			//============================================================
			// Public Functions:
			//============================================================

			// Default Constructor
			Barrel();

			// Clones the component
			// Returns: A copy of the componnet
			Component* Clone() const override;

			// Loads the component
			void Load() override;

			// Initializes the component
			void Initialize() override;

			// Updates the component
			// Params:
			//   dt = The change in time since the last step.
			void Update(float dt) override;

			// Write object data to file
			// Params:
			//   parser = The parser that is writing this object to a file.
			void Serialize(Parser& parser) const override;

			// Read object data from a file
			// Params:
			//   parser = The parser that is reading this object's data from a file.
			void Deserialize(Parser& parser) override;

			// Collision handler for the barrel.
			// Params:
			//   object = The barrel.
			//   other  = The object the barrel is colliding with.
			friend void BarrelCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection);

	private:

		//============================================================
		// Private Functions:
		//============================================================

		// Moves barrel in direction
		void Roll();

		// Handles basic animation
		void Animate();

		// Destruction sequence for barrel
		void SmashBarrel();

		//============================================================
		// Private Variables:
		//============================================================

		// Components
		Physics* physics;
		Animation* animation;
		Transform* transform;

		// Animation
		unsigned rollLength;
		Vector2D originalScale;

		// Misc.
		float speed;
	};
}

//============================================================