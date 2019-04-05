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
#include <sstream>
#include "DLPHN_PlayerScore.h"
#include "DLPHN_PlayerController.h"

#include <Parser.h>
#include <Space.h>
#include <SpriteText.h>

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Static Variables:
	//------------------------------------------------------------------------------

	unsigned PlayerScore::highScore = 0;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PlayerScore::PlayerScore()
		: Component("PlayerScore"), currentScore(0),
		playerController(nullptr),
		currentScoreDisplay(nullptr), bonusScoreDisplay(nullptr), highScoreDisplay(nullptr)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerScore::Clone() const
	{
		return new PlayerScore(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerScore::Initialize()
	{
		// Fetch gameobjects
		currentScoreDisplay = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("CurrentScoreText")->GetComponent<SpriteText>();
		highScoreDisplay = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("HighScoreText")->GetComponent<SpriteText>();
		bonusScoreDisplay = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("BonusText")->GetComponent<SpriteText>();
		
		// Fetch components
		playerController = GetOwner()->GetComponent<PlayerController>();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerScore::Update(float dt)
	{
		static float bonusTimer = 0.0f;
		bonusTimer += dt;

		// Subtract scoreBonusSubtractAmount from bonus every 2 seconds
		if (bonusTimer >= 2.0f && !playerController->getDeathStatus() && !playerController->getWinStatus())
		{
			bonusTimer = 0.0f;
			bonusScore -= bonusScoreSubtractAmount;
		}

		// Update high score if needed
		if (currentScore > highScore)
		{
			highScore = currentScore;
		}

		// Update current score
		std::stringstream cs;
		// Make sure string takes up 6 spaces
		cs.fill('0');
		cs.width(6);
		cs << currentScore;
		currentScoreDisplay->SetText("I." + cs.str());

		// Update high score
		std::stringstream hs;
		// Make sure string takes up 6 spaces
		hs.fill('0');
		hs.width(6);
		hs << highScore;
		highScoreDisplay->SetText("TOP." + hs.str());

		// Update bonus score
		std::stringstream bs;
		// Make sure string takes up 6 spaces
		bs.fill('0');
		bs.width(4);
		bs << bonusScore;
		bonusScoreDisplay->SetText(bs.str());
	}
	
	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void PlayerScore::Serialize(Parser& parser) const
	{
		parser.WriteVariable("bonusScore", bonusScore);
		parser.WriteVariable("bonusScoreSubtractAmount", bonusScoreSubtractAmount);
		parser.WriteVariable("barrelJumpScore", barrelJumpScore);
		parser.WriteVariable("barrelHammerScore", barrelHammerScore);
		parser.WriteVariable("flameHammerScore", flameHammerScore);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void PlayerScore::Deserialize(Parser& parser)
	{
		parser.ReadVariable("bonusScore", bonusScore);
		parser.ReadVariable("bonusScoreSubtractAmount", bonusScoreSubtractAmount);
		parser.ReadVariable("barrelJumpScore", barrelJumpScore);
		parser.ReadVariable("barrelHammerScore", barrelHammerScore);
		parser.ReadVariable("flameHammerScore", flameHammerScore);
	}

	// Gets the current score
	unsigned PlayerScore::getCurrentScore() const
	{
		return currentScore;
	}

	// Gets the current score bonus
	unsigned PlayerScore::getBonusScore() const
	{
		return bonusScore;
	}

	// Adds to the current score
	void PlayerScore::addCurrentScore(unsigned points)
	{
		currentScore += points;
	}

	// Adds when jumping over barrel
	void PlayerScore::addScoreBarrelJump()
	{
		currentScore += barrelJumpScore;
	}

	// Adds when hammering barrel
	void PlayerScore::addScoreBarrelHammer()
	{
		currentScore += barrelHammerScore;
	}

	// Adds when hammering flame
	void PlayerScore::addScoreFlameHammer()
	{
		currentScore += flameHammerScore;
	}
}

//------------------------------------------------------------------------------
