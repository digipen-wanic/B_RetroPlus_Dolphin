//------------------------------------------------------------------------------
//
// File Name:	GlobalTime.h
// Author(s):	David Wong Cascante (david.wongcascante)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structs
//------------------------------------------------------------------------------
class GlobalTime
{
private:
	//------------------------------------------------------------------------------
	// Private Member Variables
	//------------------------------------------------------------------------------
	// The amount of time that has passed between frames real time
	float realDt;
	// The time scale
	float timeScale;
	// The amount of time that has passed between frames scaled
	float scaledDt;
	// The amount of time that has passed since the beginning of the program
	float timeSinceStart;

	//------------------------------------------------------------------------------
	// Private Member Functions
	//------------------------------------------------------------------------------
	// Hide the constructor and copy constructor
	GlobalTime();
	GlobalTime(const GlobalTime& global);
	//------------------------------------------------------------------------------
	// Note: The management of dt and time since start is done inside theh Game Object Manager
	//------------------------------------------------------------------------------
	friend class GameObjectManager;
public:
	//------------------------------------------------------------------------------
	// Public Member Functions
	//------------------------------------------------------------------------------
	// Setters
	// Sets the time scale
	// Params:
	//	scale: The time scale we are setting
	void SetTimeScale(float scale);
	// Getters
	// Returns: The real dt
	float GetRealDt() const;
	// Returns: The time scale
	float GetTimeScale() const;
	// Returns: The scaled dt
	float GetScaledDt() const;
	// Returns: The time since the start of the program
	float GetTimeSinceStart() const;
	//Returns: The static instance of this class
	//------------------------------------------------------------------------------
	// Public Static Functions
	//------------------------------------------------------------------------------
	static GlobalTime& GetInstance();
};