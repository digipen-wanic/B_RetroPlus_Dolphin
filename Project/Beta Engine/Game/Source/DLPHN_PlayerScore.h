//------------------------------------------------------------------------------
//
// File Name:	PlayerScore.h
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

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class SpriteText;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	class PlayerController;

	class PlayerScore : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerScore();

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

		// Gets the current score
		unsigned getCurrentScore() const;

		// Gets the current score bonus
		unsigned getBonusScore() const;

		// Adds to the current score
		void addCurrentScore(unsigned points);

		// Adds when jumping over barrel
		void addScoreBarrelJump();

		// Adds when hammering barrel
		void addScoreBarrelHammer();

		// Adds when hammering flame
		void addScoreFlameHammer();

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		PlayerController* playerController;

		// Current scoring
		unsigned currentScore;
		unsigned bonusScore;

		// High score
		static unsigned highScore;

		// Score modifiers
		unsigned bonusScoreSubtractAmount;
		unsigned barrelJumpScore;
		unsigned barrelHammerScore;
		unsigned flameHammerScore;

		// Score display
		SpriteText* currentScoreDisplay;
		SpriteText* bonusScoreDisplay;
		SpriteText* highScoreDisplay;
	};
}

//------------------------------------------------------------------------------
