//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"

#include "Space.h"
#include "Transform.h"
#include <Parser.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new physics component.
// Params:
//   transform - The transform of the object.
Physics::Physics()
	: Component("Physics"), velocity(Vector2D()), angularVelocity(0.0f), inverseMass(1.0f), transform(nullptr),
	  forcesSum(Vector2D()), acceleration(Vector2D()), oldTranslation(Vector2D())
{
}

// Clone the physics component, returning a dynamically allocated copy.
Component* Physics::Clone() const
{
	return new Physics(*this);
}

// Initialize components.
void Physics::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
}

// Reset acceleration.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Physics::Update(float dt)
{	
	UNREFERENCED_PARAMETER(dt);

	acceleration = inverseMass * forcesSum;
	forcesSum = Vector2D();
}

// Update velocity and translation.
// Params:
//	 dt = Change in time (in seconds) since the last fixed update.
void Physics::FixedUpdate(float dt)
{
	// Calculate new velocity
	velocity += acceleration * dt;

	// Set old translation, and calculate new translation
	oldTranslation = transform->GetTranslation();
	transform->SetTranslation(oldTranslation + velocity * dt);

	// Set new rotation
	transform->SetRotation(transform->GetRotation() + angularVelocity * dt);
}

// Set the velocity of a physics component.
// Params:
//	 velocity = Pointer to a velocity vector.
void Physics::SetVelocity(const Vector2D& velocity_)
{
	velocity = velocity_;
}

// Get the velocity of a physics component.
// Returns:
//		A reference to the component's velocity structure.
const Vector2D& Physics::GetVelocity() const
{
	return velocity;
}

// Set the angular velocity of a physics component.
// Params:
//	 velocity = New value for the rotational velocity.
void Physics::SetAngularVelocity(float angularVelocity_)
{
	angularVelocity = angularVelocity_;
}

// Get the angular velocity of a physics component.
// Returns:
//	 A float representing the new angular velocity.
float Physics::GetAngularVelocity() const
{
	return angularVelocity;
}

// Set the mass of the physics component.
// Params:
//   mass = The new mass of the physics component.
void Physics::SetMass(float mass)
{
	inverseMass = 1.0f / mass;
}

// Add a force to the object.
// Params:
//	 force = A force vector with direction and magnitude.
void Physics::AddForce(const Vector2D& force)
{
	forcesSum += force;
}

// Get the acceleration of a physics component.
// Returns:
//	 A reference to the component's acceleration structure.
const Vector2D& Physics::GetAcceleration() const
{
	return acceleration;
}

// Get the old translation (position) of a physics component.
// Returns:
//   A reference to the component's oldTranslation structure,
const Vector2D& Physics::GetOldTranslation() const
{
	return oldTranslation;
}

// Saves object data to a file.
void Physics::Serialize(Parser& parser) const
{
	parser.WriteVariable("acceleration", acceleration);
	parser.WriteVariable("velocity", velocity);
	parser.WriteVariable("angularVelocity", angularVelocity);
	parser.WriteVariable("mass", 1.0f / inverseMass);
}

// Loads object data from a file.
void Physics::Deserialize(Parser& parser)
{
	parser.ReadVariable("acceleration", acceleration);
	parser.ReadVariable("velocity", velocity);
	parser.ReadVariable("angularVelocity", angularVelocity);
	parser.ReadVariable("mass", inverseMass);
	inverseMass = 1.0f / inverseMass;
}

//------------------------------------------------------------------------------
