//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.cpp
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
#include "ColliderLine.h"

#include "DebugDraw.h"
#include "Graphics.h"
#include "Parser.h"
#include "Physics.h"
#include "Space.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new (line) collider component.
ColliderLine::ColliderLine(bool reflection)
	: Collider(ColliderTypeLines),
	  reflection(reflection)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderLine::Clone() const
{
	return new ColliderLine(*this);
}

// Debug drawing for colliders.
void ColliderLine::Draw()
{
	DebugDraw& draw = DebugDraw::GetInstance();

	// Loop through line segments
	for (unsigned i = 0; i < lineSegments.size(); ++i)
	{
		LineSegment transformedSeg = GetLineWithTransform(i);

		// Draw line (green)
		draw.AddLineToStrip(transformedSeg.start, transformedSeg.end, Colors::Green);

		// Draw normal (red)
		Vector2D midpoint = (transformedSeg.start + transformedSeg.end) / 2.0f;
		draw.AddLineToStrip(midpoint, midpoint + transformedSeg.normal * 20.0f, Colors::Red);
	}

	// End lines and draw
	draw.EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLine::AddLineSegment(const Vector2D& p0, const Vector2D& p1)
{
	// Make a new line segment and add to vector
	lineSegments.push_back(LineSegment(p0, p1));
}

// Check for collision between a line collider and another arbitrary collider.
// Params:
//	 other = Reference to the other collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderLine::IsCollidingWith(const Collider& other) const
{
	// Only check point-line collisions
	if (other.GetType() != ColliderTypePoint && other.GetType() != ColliderTypeCircle)
		return false;

	// Hold onto transform and physics of object
	Transform& objTransform = *other.GetOwner()->GetComponent<Transform>();
	Physics& objPhysics = *other.GetOwner()->GetComponent<Physics>();

	// Store the "line" of the moving object
	LineSegment objectLine(objPhysics.GetOldTranslation(), objTransform.GetTranslation());

	// Loop through all line segments
	for (unsigned i = 0; i < lineSegments.size(); ++i)
	{
		// Temp vars to store intersection point and t-value at intersection
		Vector2D intersection;
		float t_val;
		
		// Scale linesegment by transform size
		LineSegment transformedSeg = GetLineWithTransform(i);

		// Determine if there is an intersection
		if (MovingPointLineIntersection(transformedSeg, objectLine, intersection, t_val))
		{
			// If we should reflect, then do so
			if (reflection)
			{
				MovingPointLineReflection(objTransform, objPhysics, transformedSeg, objectLine, intersection);
				return true;
			}
		}
	}
	
	// No intersections
	return false;
}

// Save object data to file.
// Params:
//   parser = The parser object used to save the object's data.
void ColliderLine::Serialize(Parser& parser) const
{
	parser.WriteVariable("lineCount", lineSegments.size());
	parser.WriteVariable("lines", "");
	parser.BeginScope();

	for (auto it = lineSegments.cbegin(); it != lineSegments.cend(); ++it)
	{
		parser.WriteValue(*it);
	}

	parser.EndScope();
}

// Load object data from file
// Params:
//   parser = The parser object used to load the object's data.
void ColliderLine::Deserialize(Parser & parser)
{
	// Reserve memory ahead of time
	size_t size;
	parser.ReadVariable("lineCount", size);
	lineSegments.reserve(size);

	parser.ReadSkip("lines");
	parser.ReadSkip(':');
	parser.ReadSkip('{');

	for (size_t i = 0; i < lineSegments.capacity(); ++i)
	{
		LineSegment segment;
		parser.ReadValue(segment);
		lineSegments.push_back(segment);
	}

	parser.ReadSkip('}');
}

// Gets a line segment that incorporates the transform of the object
// Params:
//   index = The index of the line within the array of line segments.
LineSegment ColliderLine::GetLineWithTransform(unsigned index) const
{
	Vector2D start = transform->GetMatrix() * lineSegments[index].start;
	Vector2D end = transform->GetMatrix() * lineSegments[index].end;

	return LineSegment(start, end);
}

//------------------------------------------------------------------------------
