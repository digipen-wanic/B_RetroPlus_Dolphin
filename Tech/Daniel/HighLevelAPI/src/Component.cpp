//------------------------------------------------------------------------------
//
// File Name:	Component.cpp
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
#include "Component.h"

#include "Space.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor - needed to construct BetaObject.
Component::Component(const std::string& name)
	: BetaObject(name)
{
}

// Retrieve component's owner object (if any).
GameObject* Component::GetOwner() const
{
	// cast parent to GameObject*
	return static_cast<GameObject*>(BetaObject::GetParent());
}

//------------------------------------------------------------------------------
