//------------------------------------------------------------------------------
//
// File Name:	Serializable.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Serializable.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Loads object data from a file.
void Serializable::Deserialize(Parser& )
{
}

// Saves object data to a file.
void Serializable::Serialize(Parser& ) const
{
}

//------------------------------------------------------------------------------
// Protected Functions:
//------------------------------------------------------------------------------

// These functions are protected to ensure this class is effectively abstract.
Serializable::Serializable()
{
}

Serializable::~Serializable()
{

}

Serializable::Serializable(const Serializable&)
{
}

Serializable& Serializable::operator=(const Serializable&)
{
	return *this;
}

//------------------------------------------------------------------------------
