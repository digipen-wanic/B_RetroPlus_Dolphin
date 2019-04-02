//============================================================
// Author: Jayden Yamada
// File Name: DLPHN_Barrel.cpp
// Date Created: March 29 2019
//============================================================

//============================================================
// Includes:
//============================================================

#include "stdafx.h"
#include "DLPHN_Barrel.h"

#include <Animation.h>
#include <ColliderCircle.h>
#include <ColliderLine.h>
#include <Intersection2D.h>
#include <GameObject.h>
#include <Physics.h>
#include <Parser.h>
#include <Transform.h>

//============================================================

namespace DLPHN
{
	//============================================================
	// Public Functions:
	//============================================================

	// Default Constructor
	Barrel::Barrel()
		: Component("Barrel"),
		  physics(nullptr), animation(nullptr), transform(nullptr),
		  rollLength(3),
		  speed(170.0f)
	{
	}

	// Clones the component
	// Returns: A copy of the componnet
	Component* Barrel::Clone() const
	{
		return new Barrel(*this);
	}

	// Loads the component
	void Barrel::Load()
	{
	}

	// Initializes the component
	void Barrel::Initialize()
	{
		// Get component(s)
		physics = GetOwner()->GetComponent<Physics>();
		animation = GetOwner()->GetComponent<Animation>();
		transform = GetOwner()->GetComponent<Transform>();
		originalScale = transform->GetScale();


		// set player collision handler
		GetOwner()->GetComponent<Collider>()->SetCollisionHandler(&BarrelCollisionHandler);
	}

	// Updates the component
	void Barrel::Update(float dt)
	{
		Roll();
		Animate();
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Barrel::Serialize(Parser& parser) const
	{
		parser.WriteVariable("speed", speed);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Barrel::Deserialize(Parser& parser)
	{
		parser.ReadVariable("speed", speed);
	}

	// Collision handler for the barrel.
	// Params:
	//   object = The barrel.
	//   other  = The object the barrel is colliding with.
	void BarrelCollisionHandler(GameObject& object, GameObject& other, const Vector2D& intersection)
	{
		Barrel* barrel = object.GetComponent<Barrel>();

		if (other.GetName() == "PlayerHammer")
		{
			barrel->SmashBarrel();
		}

		if (other.GetName() == "OilBarrel")
		{

		}
		
		if (other.GetComponent<ColliderLine>() != nullptr)
		{
			std::vector<LineSegment> segArray = other.
				GetComponent<ColliderLine>()->GetLineSegments();
			//Check which line is being collided with from
			//the list of lines in the collider using the intersection point
			for (int i = 0; i < segArray.size(); i++)
			{
				if (PointIsBetweenLines(intersection,
					segArray[i].start, segArray[i].end, segArray[i].direction))
				{
					barrel->moveDirection = segArray[i].direction;
					barrel->grounded = true;
					//Swap left/right direction
					if (barrel->barrelState == barrel->Left)
						barrel->barrelState = barrel->Right;
					if (barrel->barrelState == barrel->Right)
						barrel->barrelState = barrel->Left;
					continue;
				}
			}
		}
	}

	//============================================================
	// Private Functions:
	//============================================================

	// Moves barrel in direction
	void Barrel::Roll()
	{
		if (grounded)
		{
			Vector2D dir;
			if (barrelState == Left)
				dir = Vector2D(moveDirection.x * -speed, 0);
			if (barrelState == Right)
				dir = Vector2D(moveDirection.x * speed, 0);
			physics->SetVelocity(dir);
		}
		else
		{
			physics->SetVelocity(Vector2D(0, -speed));
		}
	}

	// Handles basic animation
	void Barrel::Animate()
	{
		// Flip sprite depending on direction of travel
		if (physics->GetVelocity().x < 0.0f)
		{
			transform->SetScale(Vector2D(-originalScale.x, originalScale.y));
		}
		else
		{
			transform->SetScale(originalScale);
		}

		if (grounded != prevGrounded)
		{
			if (grounded)
			{
				// Start animation
				animation->Play(0.2f, 0, rollLength, true);
			}
			else
			{
				animation->Play(0.2f, 4, 5, true);
			}
			prevGrounded = grounded;
		}
	}

	// Destruction sequence for barrel
	void Barrel::SmashBarrel()
	{
		// Destroy this object
		GetOwner()->Destroy();
	}
}

//============================================================