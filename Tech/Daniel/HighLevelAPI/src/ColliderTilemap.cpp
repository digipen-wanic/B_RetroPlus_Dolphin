//------------------------------------------------------------------------------
//
// File Name:	ColliderTilemap.cpp
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
#include "ColliderTilemap.h"

#include "ColliderTilemap.h"
#include "ColliderRectangle.h"
#include <Shapes2D.h>

#include "Collider.h"
#include "Matrix2DStudent.h"
#include "Physics.h"
#include "Space.h"
#include "Transform.h"
#include "Tilemap.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for Tilemap collider.
ColliderTilemap::ColliderTilemap()
	: Collider(ColliderTypeTilemap), map(nullptr)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderTilemap::Clone() const
{
	return new ColliderTilemap(*this);
}

// Debug drawing for colliders.
void ColliderTilemap::Draw()
{
}

// Check for collision between a tilemap and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderTilemap::IsCollidingWith(const Collider& other) const
{
	// make sure collider is a rectangle
	if (other.GetType() != ColliderTypeRectangle)
		return false;

	// make sure object has a physics component
	if (other.GetOwner()->GetComponent<Physics>() == nullptr)
		return false;

	// get properties of other object
	Transform*		  otherTransform = other.GetOwner()->GetComponent<Transform>();
	Physics*		  otherPhysics = other.GetOwner()->GetComponent<Physics>();
	ColliderRectangle otherRectangle = static_cast<const ColliderRectangle&>(other);

	BoundingRectangle otherBounds(otherTransform->GetTranslation(), otherRectangle.GetExtents());
	//Vector2D		  otherOldTranslation(otherPhysics->GetOldTranslation());

	// check collisions and store in mapcollision
	MapCollision mapcollision
	(
		IsSideColliding(otherBounds, SideBottom),
		IsSideColliding(otherBounds, SideTop   ),
		IsSideColliding(otherBounds, SideLeft  ),
		IsSideColliding(otherBounds, SideRight )
	);

	// resolve collisions
	ResolveCollisions(otherBounds, otherTransform, otherPhysics, mapcollision);

	// any collision
	if (mapcollision.top || mapcollision.bottom || mapcollision.right || mapcollision.left)
	{
		// call collisionhandler on other object
		if (otherRectangle.GetMapCollisionHandler() != nullptr)
			otherRectangle.GetMapCollisionHandler()(*other.GetOwner(), mapcollision);

		return true;
	}

	return false;
}

// Sets the tilemap to use for this collider.
// Params:
//   map = A pointer to the tilemap resource.
void ColliderTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Checks whether the specified side of a given rectangle is colliding with the tilemap.
// Params:
//   rectangle = The bounding rectangle for an object.
//   side = The side that needs to be tested for collision.
// Returns:
//   True if there is a collision, false otherwise.
bool ColliderTilemap::IsSideColliding(const BoundingRectangle& rectangle, RectangleSide side) const
{
	switch (side)
	{
	case SideBottom:
	{
		// get quarter of rectangle width
		float offset = rectangle.extents.x * 0.7f;

		// Check both points
		if (IsCollidingAtPosition(rectangle.left + offset, rectangle.bottom))
			return true;

		if (IsCollidingAtPosition(rectangle.right - offset, rectangle.bottom))
			return true;

		return false;
	}

	case SideTop:
	{
		// get quarter of rectangle width
		float offset = rectangle.extents.x * 0.7f;

		// Check both points
		if (IsCollidingAtPosition(rectangle.left + offset, rectangle.top))
			return true;

		if (IsCollidingAtPosition(rectangle.right - offset, rectangle.top))
			return true;

		return false;
	}
	case SideLeft:
	{
		// get quarter of rectangle width
		float offset = rectangle.extents.y * 0.7f;

		// Check both points
		if (IsCollidingAtPosition(rectangle.left, rectangle.bottom + offset))
			return true;

		if (IsCollidingAtPosition(rectangle.left, rectangle.top - offset))
			return true;

		return false;
	}
	case SideRight:
	{
		// get quarter of rectangle width
		float offset = rectangle.extents.y * 0.7f;

		// Check both points
		if (IsCollidingAtPosition(rectangle.right, rectangle.bottom + offset))
			return true;

		if (IsCollidingAtPosition(rectangle.right, rectangle.top - offset))
			return true;

		return false;
	}
	}

	return false;
}

// Determines whether a point is within a collidable cell in the tilemap.
// Params:
//   x = The x component of the point, in world coordinates.
//   y = The y component of the point, in world coordinates.
// Returns:
//   -1 if the point is outside the map, 0 if the map is empty at that position, 
//   or a positive integer if there is a tile at that position.
bool ColliderTilemap::IsCollidingAtPosition(float x, float y) const
{
	Vector2D point(x, y);
	
	// Convert point to normalized coordinate system
	point = GetOwner()->GetComponent<Transform>()->GetInverseMatrix() * point;

	// get column and row equivalents for normalized points x & y
	unsigned column = static_cast<unsigned>(floor(point.x + 0.5f));
	unsigned row    = static_cast<unsigned>(floor(-point.y + 0.5f));

	if (map->GetCellValue(column, row))
		return true;

	return false;
}

// Moves an object and sets its velocity based on where it collided with the tilemap.
// Params:
//   objectRectangle = A bounding rectangle that encompasses the object.
//   objectTransform = Pointer to the object's transform component.
//   objectPhysics = Pointer to the object's physics component.
//   collisions = True/false values from map collision checks.
// Returns:
//   False if the point is outside the map or the map is empty at that position, 
//   or true if there is a tile at that position.
void ColliderTilemap::ResolveCollisions(const BoundingRectangle& objectRectangle, Transform* objectTransform,
	Physics* objectPhysics, const MapCollision& collisions) const
{
	// Get the translation and velocity from the object and store them in variables
	Vector2D translation = objectTransform->GetTranslation();
	Vector2D velocity = objectPhysics->GetVelocity();

	// determine how much to move the object
	float nudgeAmount;

	// find the center of the next tile, then find the distance from the corresponding side to that position
	float nextTileCenter;

	if (collisions.bottom || collisions.top)
	{
		if (collisions.bottom)
		{
			// Find tile above the object's bottom side
			nextTileCenter = GetNextTileCenter(SideBottom, objectRectangle.bottom);

			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.bottom;
		}
		else
		{
			// Find tile below object's top side
			nextTileCenter = GetNextTileCenter(SideTop, objectRectangle.top);

			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.top;
		}

		// Nudge object up/down
		translation.y += nudgeAmount;

		// Stop velocity in y direction
		velocity.y = 0;
	}

	if (collisions.left || collisions.right)
	{
		if (collisions.left)
		{
			// Find tile to the right the object's left side

			nextTileCenter = GetNextTileCenter(SideLeft, objectRectangle.left);

			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.left;
		}
		else
		{
			// Find tile to the left the object's right side
			nextTileCenter = GetNextTileCenter(SideRight, objectRectangle.right);

			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.right;
		}

		// Nudge object left/right
		translation.x += nudgeAmount;

		// Stop velocity in x direction
		velocity.x = 0;
	}

	// Modify object's actual translation and velocity
	objectTransform->SetTranslation(translation);
	objectPhysics->SetVelocity(velocity);
}

// Returns the center of the next tile on the x-axis or y-axis.
// Used for assisting in collision resolution on a particular side.
// Params:
//   side = Which side the collision is occurring on.
//   sidePosition = The x or y value of that side.
// Returns:
//   The center of the next tile for the given position on the given side.
float ColliderTilemap::GetNextTileCenter(RectangleSide side, float sidePosition) const
{
	// Create a world space point
	Vector2D point;

	// Create a reference variable to store the result
	float* result = &point.x;

	if (side == RectangleSide::SideTop || side == RectangleSide::SideBottom)
	{
		// Use the given position for the y-axis
		// Since we only care about one axis, the other will be 0
		point = Vector2D(0.0f, sidePosition);

		// Remember which part we want to modify
		result = &point.y;
	}
	else
	{
		// Use the given position for the x-axis
		// Since we only care about one axis, the other will be 0
		point = Vector2D(sidePosition, 0.0f);

		// Remember which part we want to modify
		result = &point.x;
	}

	// Transform the world space point into tile space
	point = transform->GetInverseMatrix() * point;

	// Flip the y-axis
	point.y = -point.y;

	// Move completely into cell
	point += Vector2D(0.5, 0.5);

	// Find the next tile index (higher or lower depending on the side)
	if (side == RectangleSide::SideLeft || side == RectangleSide::SideTop)
	{
		*result = ceil(*result);
	}
	else
	{
		*result = floor(*result);
	}

	// Re-adjust object position
	point -= Vector2D(0.5, 0.5);

	// Flip the y-axis
	point.y = -point.y;

	// Transform point back into world space
	point = transform->GetMatrix() * point;

	return *result;
}

//------------------------------------------------------------------------------
