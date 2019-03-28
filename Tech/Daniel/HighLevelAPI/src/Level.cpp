//------------------------------------------------------------------------------
//
// File Name:	Level.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Level.h"

#include "Space.h"

//------------------------------------------------------------------------------

// Creates a game state.
// Params:
//   name = Name to use for this state.
Level::Level(const std::string& name)
	: BetaObject(name)
{
}

// Retrieve the space in which the level is running.
Space* Level::GetSpace() const
{
	// cast parent to Space*
	return static_cast<Space*>(BetaObject::GetParent());
}

//------------------------------------------------------------------------------
