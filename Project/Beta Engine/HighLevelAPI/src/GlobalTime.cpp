//------------------------------------------------------------------------------
//
// File Name:	GlobalTime.cpp
// Author(s):	David Wong Cascante (david.wongcascante)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdafx.h>
#include "GlobalTime.h"

//------------------------------------------------------------------------------
// Private Member Variables
//------------------------------------------------------------------------------

GlobalTime::GlobalTime()
	: realDt(0.0f), scaledDt(0.0f), timeScale(1.0f), timeSinceStart(0.0f)
{
}

GlobalTime::GlobalTime(const GlobalTime& other)
	: realDt(0.0f), scaledDt(0.0f), timeScale(other.timeScale), timeSinceStart(other.timeSinceStart)
{
}

//------------------------------------------------------------------------------
// Public Member Functions
//------------------------------------------------------------------------------

void GlobalTime::SetTimeScale(float scale)
{
	timeScale = scale;
}

float GlobalTime::GetRealDt() const
{
	return realDt;
}

float GlobalTime::GetScaledDt() const
{
	return scaledDt;
}

float GlobalTime::GetTimeScale() const
{
	return timeScale;
}

float GlobalTime::GetTimeSinceStart() const
{
	return timeSinceStart;
}

//------------------------------------------------------------------------------
// Public Static Functions
//------------------------------------------------------------------------------


GlobalTime& GlobalTime::GetInstance()
{
	static GlobalTime global;
	return global;
}