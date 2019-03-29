#pragma once
//============================================================
// Author: Jayden Yamada
// File Name: DLPHN_Barrel.h
// Date Created: March 29 2019
//============================================================

#include <Component.h>
#include <Serializable.h>

//Forward
class Physics;

namespace DLPHN
{



	class Barrel : public Component, public Serializable
	{
		private :
			Physics* phys;

			float speed;

		public :
			// Constructor
			Barrel(float speed_);
			// Clones the component
			// Returns: A copy of the componnet
			Component* Clone() const override;
			// Loads the component
			void Load() override;
			// Initializes the component
			void Initialize() override;
			// Updates the component
			void Update(float dt) override;
	};
}



