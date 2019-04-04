//------------------------------------------------------------------------------
//
// File Name:	PlayerScore.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DLPHN_PlayerScore.h"

#include <Parser.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PlayerScore::PlayerScore()
		: Component("PlayerScore"), currentScore(0),
		currentScoreDisplay(nullptr), scoreBonusDisplay(nullptr), highScoreDisplay(nullptr)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerScore::Clone() const
	{
		return new PlayerScore(*this);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerScore::Update(float dt)
	{
		static float bonusTimer = 0.0f;
		bonusTimer += dt;

		// Subtract scoreBonusSubtractAmount from bonus every second
		if (bonusTimer >= 1.0f)
		{
			bonusTimer = 0.0f;
			scoreBonus -= scoreBonusSubtractAmount;
		}

		// Update high score if needed
		if (currentScore > highScore)
		{
			highScore = currentScore;
		}
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void PlayerScore::Serialize(Parser& parser) const
	{
		parser.WriteVariable("scoreBonus", scoreBonus);
		parser.WriteVariable("scoreBonusSubtractAmount", scoreBonusSubtractAmount);
		parser.WriteVariable("barrelJumpScore", barrelJumpScore);
		parser.WriteVariable("barrelHammerScore", barrelHammerScore);
		parser.WriteVariable("flameHammerScore", flameHammerScore);
		parser.WriteVariable("highScore", highScore);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void PlayerScore::Deserialize(Parser& parser)
	{
		parser.ReadVariable("scoreBonus", scoreBonus);
		parser.ReadVariable("scoreBonusSubtractAmount", scoreBonusSubtractAmount);
		parser.ReadVariable("barrelJumpScore", barrelJumpScore);
		parser.ReadVariable("barrelHammerScore", barrelHammerScore);
		parser.ReadVariable("flameHammerScore", flameHammerScore);
		parser.ReadVariable("highScore", highScore);
	}

	// Gets the current score
	unsigned PlayerScore::getCurrentScore() const
	{
		return currentScore;
	}

	// Gets the current score bonus
	unsigned PlayerScore::getScoreBonus() const
	{
		return scoreBonus;
	}

	// Adds to the current score
	void PlayerScore::addCurrentScore(unsigned points)
	{
		currentScore += points;
	}

	// Sets display spritetext for current score
	void PlayerScore::setCurrentScoreDisplay(SpriteText* display)
	{
		currentScoreDisplay = display;
	}

	// Sets display spritetext for score bonus
	void PlayerScore::setScoreBonusDisplay(SpriteText* display)
	{
		scoreBonusDisplay = display;
	}

	// Sets display spritetext for high score
	void PlayerScore::setHighScoreDisplay(SpriteText* display)
	{
		highScoreDisplay = display;
	}
}

//------------------------------------------------------------------------------
