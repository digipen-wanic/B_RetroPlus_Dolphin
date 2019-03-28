//------------------------------------------------------------------------------
//
// File Name:	ColliderPoint.cpp
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
#include "ColliderPoint.h"

#include <DebugDraw.h>
#include <Graphics.h>
#include <Shapes2D.h>

#include "ColliderCircle.h"
#include "ColliderRectangle.h"
#include "Intersection2D.h"
#include "Space.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for point collider.
ColliderPoint::ColliderPoint()
	: Collider(ColliderTypePoint)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderPoint::Clone() const
{
	return new ColliderPoint(*this);
}

// Debug drawing for colliders.
void ColliderPoint::Draw()
{
	DebugDraw::GetInstance().AddCircle(transform->GetTranslation(), 10.0f, Graphics::GetInstance().GetCurrentCamera(), Colors::White);
	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

// Check for collision between a point and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderPoint::IsCollidingWith(const Collider& other) const
{
	Vector2D point = transform->GetTranslation();

	// Get transform of other collider
	Transform* otherTransform = other.GetOwner()->GetComponent<Transform>();

	// check for collision based on collider type
	switch (other.GetType())
	{
	case ColliderTypeCircle:
	{
		// cast other to circle collider, and use properties
		Circle circle(otherTransform->GetTranslation(), static_cast<const ColliderCircle&>(other).GetRadius());
		return PointCircleIntersection(point, circle);
	}

	case ColliderTypeRectangle:
	{
		// cast other to rectangle collider, and use properties
		BoundingRectangle rectangle(otherTransform->GetTranslation(), static_cast<const ColliderRectangle&>(other).GetExtents());
		return PointRectangleIntersection(point, rectangle);
	}
	}

	return false;
}

//------------------------------------------------------------------------------
