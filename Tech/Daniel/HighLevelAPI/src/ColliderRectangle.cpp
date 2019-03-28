//------------------------------------------------------------------------------
//
// File Name:	ColliderRectangle.cpp
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
#include "ColliderRectangle.h"

#include <DebugDraw.h>
#include <Graphics.h>
#include <Shapes2D.h>

#include "Intersection2D.h"
#include "Parser.h"
#include "Space.h"
#include "Transform.h"

#include "ColliderCircle.h"
#include "ColliderPoint.h"
#include "ColliderTilemap.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for rectangle collider.
// Params:
//   extents = The rectangle's extents (half-width, half-height).
ColliderRectangle::ColliderRectangle(const Vector2D& extents)
	: Collider(ColliderTypeRectangle), extents(extents)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderRectangle::Clone() const
{
	return new ColliderRectangle(*this);
}

// Debug drawing for colliders.
void ColliderRectangle::Draw()
{
	DebugDraw::GetInstance().AddRectangle(transform->GetTranslation(), extents, Graphics::GetInstance().GetCurrentCamera());
	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

// Get the rectangle collider's extents (half-width, half-height).
// Returns:
//	 The extents of the rectangle.
const Vector2D& ColliderRectangle::GetExtents() const
{
	return extents;
}

// Set the rectangle collider's extents (half-width, half-height).
// Params:
//   extents = The new extents of the rectangle.
void ColliderRectangle::SetExtents(const Vector2D& extents_)
{
	extents = extents_;
}

// Check for collision between a circle and another arbitrary collider.
// Params:
//	 other = Reference to the second circle collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderRectangle::IsCollidingWith(const Collider& other) const
{
	// Get translation of other collider
	Vector2D otherTranslation(other.GetOwner()->GetComponent<Transform>()->GetTranslation());

	// Make this a rectangle
	BoundingRectangle thisRectangle(transform->GetTranslation(), extents);

	// check for collision based on collider type
	switch (other.GetType())
	{
	case ColliderTypePoint:
		return PointRectangleIntersection(otherTranslation, thisRectangle);

	case ColliderTypeCircle:
	{
		// cast other to circle collider, and use properties
		Circle otherCircle(otherTranslation, static_cast<const ColliderCircle&>(other).GetRadius());
		return RectangleCircleIntersection(thisRectangle, otherCircle);
	}

	case ColliderTypeRectangle:
	{
		// cast other to rectangle collider, and use properties
		BoundingRectangle rectangle(otherTranslation, static_cast<const ColliderRectangle&>(other).GetExtents());
		return RectangleRectangleIntersection(rectangle, thisRectangle);
	}
	}

	// default collision
	return other.IsCollidingWith(*this);
}

// Saves object data to a file.
void ColliderRectangle::Serialize(Parser& parser) const
{
	parser.WriteVariable("extents", extents);
}

// Loads object data from a file.
void ColliderRectangle::Deserialize(Parser& parser)
{
	parser.ReadVariable("extents", extents);
}

//------------------------------------------------------------------------------
