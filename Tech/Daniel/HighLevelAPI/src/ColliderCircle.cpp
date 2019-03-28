//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.cpp
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
#include "ColliderCircle.h"

#include <DebugDraw.h>
#include <Graphics.h>
#include <Shapes2D.h>

#include "ColliderPoint.h"
#include "ColliderRectangle.h"
#include "Intersection2D.h"
#include "Parser.h"
#include "Space.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for circle collider.
// Params:
//   radius = The radius of the circle.
ColliderCircle::ColliderCircle(float radius)
	: Collider(ColliderTypeCircle), radius(radius)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderCircle::Clone() const
{
	return new ColliderCircle(*this);
}

// Debug drawing for colliders.
void ColliderCircle::Draw()
{
	DebugDraw::GetInstance().AddCircle(transform->GetTranslation(), radius, Graphics::GetInstance().GetCurrentCamera(), Colors::White);
	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

// Get the circle collider's radius.
// Returns:
//	 The circle collider's radius.
float ColliderCircle::GetRadius() const
{
	return radius;
}

// Set the circle collider's radius.
// Params:
//   radius = The circle collider's new radius.
void ColliderCircle::SetRadius(float radius_)
{
	radius = radius_;
}

// Check for collision between a circle and another arbitrary collider.
// Params:
//	 other = Reference to the second circle collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderCircle::IsCollidingWith(const Collider& other) const
{
	// Get transform of other collider
	Vector2D otherTranslation(other.GetOwner()->GetComponent<Transform>()->GetTranslation());

	// Make this a circle
	Circle thisCircle(transform->GetTranslation(), radius);
	
	// check for collision based on collider type
	switch (other.GetType())
	{
	case ColliderTypePoint:
		return PointCircleIntersection(otherTranslation, thisCircle);

	case ColliderTypeCircle:
	{
		// cast other to circle collider, and use properties
		Circle otherCircle(otherTranslation, static_cast<const ColliderCircle&>(other).GetRadius());
		return CircleCircleIntersection(thisCircle, otherCircle);
	}

	case ColliderTypeRectangle:
	{
		// cast other to rectangle collider, and use properties
		BoundingRectangle rectangle(otherTranslation, static_cast<const ColliderRectangle&>(other).GetExtents());
		return RectangleCircleIntersection(rectangle, thisCircle);
	}
	case ColliderTypeLines:
		return other.IsCollidingWith(*this);
	}

	return false;
}

// Saves object data to a file.
void ColliderCircle::Serialize(Parser& parser) const
{
	parser.WriteVariable("radius", radius);
}

// Loads object data from a file.
void ColliderCircle::Deserialize(Parser& parser)
{
	parser.ReadVariable("radius", radius);
}

//------------------------------------------------------------------------------
