//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Intersection2D.h"

#include "Physics.h"
#include "Transform.h"
#include <Vector2D.h>

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Check whether a point and circle intersect.
// Params:
//  point = The point in the intersection.
//  circle = The circle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointCircleIntersection(const Vector2D& point, const Circle& circle)
{
	if (point.DistanceSquared(circle.center) < circle.radius * circle.radius)
		return true;
	return false;
}

// Check whether a point and rectangle intersect.
// Params:
//  point = The point in the intersection.
//  rect  = The rectangle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect)
{
	if (point.x < rect.left || point.x > rect.right || point.y < rect.bottom || point.y > rect.top)
		return false;
	return true;
}

// Check whether two circles intersect.
// Params:
//  circle1 = The first circle.
//  circle2 = The second circle.
// Returns:
//   True if intersection, false otherwise.
bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2)
{
	if (circle1.center.DistanceSquared(circle2.center) < (circle1.radius + circle2.radius) * (circle1.radius + circle2.radius))
		return true;
	return false;
}

// Check whether two rectangles intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2)
{
	if (rect1.right < rect2.left || rect1.left > rect2.right || rect1.top < rect2.bottom || rect1.bottom > rect2.top)
		return false;
	return true;
}

// Check whether a rectangle and circle intersect.
// Params:
//  rect = The rectangle.
//  circle  = The circle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle)
{
	Vector2D closest = Vector2D(max(rect.left, min(rect.right, circle.center.x)), max(rect.bottom, min(rect.top, circle.center.y)));
	if (closest.DistanceSquared(circle.center) <= circle.radius * circle.radius)
		return true;
	return false;
}

// Check whether a moving point and line intersect.
// Params:
//  staticLine   = Start and end of first line segment.
//  movingPoint  = Start and end of second line segment (usually a moving point).
//  intersection = Intersection point, if any.
//  t = The t value from the intersection calculation.
// Returns:
//   True if intersection, false otherwise.
bool MovingPointLineIntersection(const LineSegment& staticLine,
	const LineSegment& movingPoint, Vector2D& intersection, float& t)
{
	// 1. Calculate the second object's change in translation during the current frame. Store this in your velocity variable.
	Vector2D velocity(movingPoint.end - movingPoint.start);

	// 2. Return false if the second object is stationary or is moving parallel to the line segment. 
	// (HINT: Use dot product with the velocity and line's normal!)
	if (AlmostEqual(velocity.MagnitudeSquared(), 0.0f) || AlmostEqual(velocity.DotProduct(staticLine.normal), 0.0f))
		return false;

	//////////////////////////////////////////////////////////////////
	//																//
	// Now we calculate the time of intersection.					//
	// Get ready for lots of dot products!							//
	//																//
	// FIRST LINE - NORMAL EQ (static line)							//
	// NOTE: a = normal.x, b = normal.y, p = start					//
	// a * x + b * y = c											//
	// c = a * p.x + b * p.y = normal dot p							//
	// Thus, normal dot xy = normal dot p							//
	//																//
	// SECOND LINE - VECTOR EQ (moving point)						//
	// xy = start + (start - end) * t								//
	// From earlier, we know that start - end = velocity			//
	// Thus, xy = start + velocity * t								//
	//																//
	// COMBINED EQs													//
	// normal dot (start + velocity * t) = normal dot p				//
	//																//
	//////////////////////////////////////////////////////////////////

	// Determine dot products
	float nDotP = staticLine.normal.DotProduct(staticLine.start);
	float nDotS = staticLine.normal.DotProduct(movingPoint.start);
	float nDotV = staticLine.normal.DotProduct(velocity);

	// 3. Using the above information, solve for t.
	t = (nDotP - nDotS) / nDotV;

	// 4. Check if intersection is between moving point start and end (if t is < 0 or > 1)
	// If not between start and end, return false
	if (t < 0 || t > 1)
		return false;

	// 5. Calculate the point of intersection using the start, velocity, and t.
	intersection = movingPoint.start + velocity * t;

	// 6. Verify intersection point is on static segment (using static line direction as normal)
	// If intersection point is not between static line start and static line end
	//	return false
	float lineDisSqrd = intersection.DistanceSquared(staticLine.start) + intersection.DistanceSquared(staticLine.end);
	float lineLenSqrd = staticLine.start.DistanceSquared(staticLine.end);

	// Make sure distance is greater than length
	if (lineDisSqrd > lineLenSqrd)
		return false;

	// 7. Other possibilities have been eliminated, so
	// this must be an intersection. Return true!
	return true;
}

// Modifies object's position, velocity, and rotation using simple point-line reflection.
// Params:
//  transform = Transform of the object that is being reflected.
//  physics   = Physics of the object being reflected.
//  staticLine   = Start and end of first line segment.
//  movingPoint = Start and end of second line segment (usually a moving point).
//  intersection = Intersection point of the line and circle. 
void MovingPointLineReflection(Transform& transform, Physics& physics,
	const LineSegment& staticLine,const LineSegment& movingPoint,const Vector2D& intersection)
{
	// 1. Find correct position of object by reflecting its end point over the line.
	// (Use the ReflectPointOverLine function!)
	// Once found, set the object's translation to this reflected point.
	transform.SetTranslation(ReflectPointOverLine(movingPoint.end, staticLine));

	// 2. Use the reflected position and intersection point to find the
	// direction of the reflected velocity. Make sure to normalize this vector!
	Vector2D newDirection = Vector2D(transform.GetTranslation() - intersection).Normalized();

	// 3. Set the velocity of the object to this direction times
	// the magnitude of the current velocity.
	physics.SetVelocity(newDirection * physics.GetVelocity().Magnitude());

	// 4. Find the object's new rotation by using atan2 with the reflected direction.
	// Once found, set the rotation of the object to this angle.
	transform.SetRotation(atan2(newDirection.y, newDirection.x));
}

// Reflects a point over a line.
// Params:
//   point = The point being reflected.
//   line = The line the point will be reflected over.
// Returns:
//   The reflected point.
Vector2D ReflectPointOverLine(const Vector2D& point, const LineSegment& line)
{
	// 1. Pretend everything is at the origin by subtracting one of the line's 
	// points from the point we are reflecting.
	Vector2D originP = point - line.start;

	//////////////////////////////////////////////////////////////////////////////////
	//																				//
	// Equation for reflected point R is as follows, given point P,					//
	// and a line with normal n:													//
	//		R = P - 2 * projection of P onto n										//
	//																				//
	// Equation for projected point J (projection of P onto n) is as follows:		//
	//																				//
	//		J = (P dot n) / (magnitude(n) squared) * n								//
	//																				//
	// However, since our normal is normalized (magnitude = 1), this simplifies		//
	// the projection (and hence the reflection) significantly:						//
	//																				//
	//		J = (P dot n) * n														//
	//																				//
	//////////////////////////////////////////////////////////////////////////////////

	// 2. Calculate the reflected point and store it in a variable.
	// Reflected point = P - 2 * (P dot n) * n
	Vector2D reflectedP = originP - 2 * (originP.DotProduct(line.normal)) * line.normal;

	// 3. We need to remember to move things back away from the origin before returning.
	// Return the reflected point + line.start (or line.end, if you used that at the beginning)
	return reflectedP += line.start;
}

//------------------------------------------------------------------------------
