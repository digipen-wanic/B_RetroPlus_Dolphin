//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"

#include "Physics.h"
#include "Space.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor
// Params:
//   bottom = Whether collision is occurring on the bottom
//   top    = Whether collision is occurring on the top
//   left   = Whether collision is occurring on the left
//   right  = Whether collision is occurring on the right
MapCollision::MapCollision(bool bottom, bool top, bool left, bool right)
	: bottom(bottom), top(top), left(left), right(right)
{
}

// Allocate a new collider component.
// Params:
//   owner = Reference to the object that owns this component.
Collider::Collider(ColliderType type)
	: Component("Collider"), type(type), handler(nullptr),
	  transform(nullptr), physics(nullptr), mapHandler(nullptr)
{
}

// Set component pointers.
void Collider::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
}

// Check if two objects are colliding and send collision events.
// Params:
//	 other = Reference to the second collider component.
void Collider::CheckCollision(const Collider& other)
{
	if (IsCollidingWith(other))
	{
		// Call event handlers
		if (handler != nullptr)
			handler(*GetOwner(), *other.GetOwner());

		if (other.handler != nullptr)
			other.handler(*other.GetOwner(), *GetOwner());
	}
}

// Get the type of this component.
ColliderType Collider::GetType() const
{
	return type;
}

// Sets the collision handler function for the collider.
// Params:
//   handler = A pointer to the collision handler function.
void Collider::SetCollisionHandler(CollisionEventHandler handler_)
{
	handler = handler_;
}

// Sets the map collision handler function for the collider.
// Params:
//   handler = A pointer to the collision handler function.
void Collider::SetMapCollisionHandler(MapCollisionEventHandler mapHandler_)
{
	mapHandler = mapHandler_;
}

// Get the map collision handler function pointer.
MapCollisionEventHandler Collider::GetMapCollisionHandler() const
{
	return mapHandler;
}

//------------------------------------------------------------------------------
