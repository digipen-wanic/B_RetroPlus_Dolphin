//------------------------------------------------------------------------------
//
// File Name:	ColliderPoint.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Collider.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ColliderPoint : public Collider
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for point collider.
	ColliderPoint();

	// Clone an collider and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a collider.
	Component* Clone() const;

	// Save object data to file.
	// Params:
	//   parser = The parser object used to save the object's data.
	void Serialize(Parser& parser) const;

	// Load object data from file
	// Params:
	//   parser = The parser object used to load the object's data.
	void Deserialize(Parser & parser);

	// Debug drawing for colliders.
	void Draw() override;

	// Gets the point's offset
	// Returns: The collider's offset
	const Vector2D& GetOffset() const;

	// Check for collision between a point and another arbitrary collider.
	// Params:
	//	 other = Reference to the second point collider component.
	//	 intersection = Returns null because this functionality only works with line collisions
	// Returns:
	//	 Return the results of the collision check.
	bool IsCollidingWith(const Collider& other, Vector2D* intersection = nullptr) const override;
private:
	//------------------------------------------------------------------------------
	// Private Variables 
	//------------------------------------------------------------------------------
	Vector2D offset;
};

//------------------------------------------------------------------------------
