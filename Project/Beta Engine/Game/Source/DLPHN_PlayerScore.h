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
		unsigned getScoreBonus() const;

		// Adds to the current score
		void addCurrentScore(unsigned points);

		// Sets display spritetext for current score
		void setCurrentScoreDisplay(SpriteText* display);

		// Sets display spritetext for score bonus
		void setScoreBonusDisplay(SpriteText* display);

		// Sets display spritetext for high score
		void setHighScoreDisplay(SpriteText* display);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Current scoring
		unsigned currentScore;
		unsigned scoreBonus;

		// High score
		unsigned highScore;

		// Score modifiers
		unsigned scoreBonusSubtractAmount;
		unsigned barrelJumpScore;
		unsigned barrelHammerScore;
		unsigned flameHammerScore;

		// Score display
		SpriteText* currentScoreDisplay;
		SpriteText* scoreBonusDisplay;
		SpriteText* highScoreDisplay;
	};
}

//------------------------------------------------------------------------------
