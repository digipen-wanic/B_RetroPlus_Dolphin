//============================================================
// Author: Jayden Yamada
// File Name: DLPHN_Barrel.cpp
// Date Created: March 29 2019
//============================================================

// Barrels are working!

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
#include "DLPHN_PlayerController.h"
#include <Physics.h>
#include <Parser.h>
#include <Random.h>
#include <Space.h>
#include <Sprite.h>
#include <SpriteSource.h>
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
		  speed(170.0f),
		  barrelState(Left),
		  notTouchingDelay(0.1f), notTouchingTimer(0.0f),
		  grounded(false),
		  prevGrounded(false)

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

		notTouchingTimer += dt;
		if (notTouchingTimer > notTouchingDelay)
		{
			grounded = false;
			//std::cout << "Update " << grounded << std::endl;
		}
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

		unsigned hammerStatus;
		if (object.GetComponent<Sprite>()->GetSpriteSource()->GetName() == "Barrel")
		{
			hammerStatus = object.GetSpace()->GetObjectManager().GetObjectByName("Player")->GetComponent<PlayerController>()->getHammerStatus();
		}
		else
		{
			hammerStatus = object.GetSpace()->GetObjectManager().GetObjectByName("PlayerPlus")->GetComponent<PlayerController>()->getHammerStatus();
		}

		if (other.GetName() == "PlayerHammer" && hammerStatus)
		{
			barrel->SmashBarrel();
		}
		
		if (other.GetName() == "OilBarrel")
		{
			barrel->SmashBarrel();
		}
		
		if (other.GetComponent<ColliderLine>() != nullptr)
		{
			std::vector<LineSegment> segArray = other.
				GetComponent<ColliderLine>()->GetLineSegments();
			//Check which line is being collided with from
			//the list of lines in the collider using the intersection 
			for (int i = 0; i < segArray.size(); i++)
			{
				if (PointIsBetweenLines(intersection,
					segArray[i].start, segArray[i].end, segArray[i].direction))
				{
					barrel->moveDirection = segArray[i].direction;
					if (segArray[i].direction.x < 0)
						barrel->moveDirection = segArray[i].direction * -1;
					//Swap left/right direction
					if (!barrel->grounded && barrel->barrelState == barrel->Left)
						barrel->barrelState = barrel->Right;
					else if (!barrel->grounded && barrel->barrelState == barrel->Right)
						barrel->barrelState = barrel->Left;
					continue;
				}
			}
			
			barrel->grounded = true;
			barrel->notTouchingTimer = 0;

			Transform* transform = object.GetComponent<Transform>();
			Physics* physics = object.GetComponent<Physics>();
			ColliderCircle* point = object.GetComponent<ColliderCircle>();
			Vector2D currentPosition = transform->GetTranslation() + point->GetOffset();
			Vector2D displacementVector = intersection - currentPosition;

			Vector2D velocity = physics->GetVelocity();
			if (displacementVector.y > 0 && velocity.y < 0)
			{
				velocity.y = 0;

				// Solve the simple problem first and stop the player from falling vertically
				float yNudge = displacementVector.y * 0.90f - point->GetOffset().y;
				//float yNudge = 10.0f;
				//float yNudge = -displacementVector.y * 0.5;// -point->GetOffset().y;
				transform->SetTranslation(Vector2D(currentPosition.x, currentPosition.y + yNudge));
				physics->SetVelocity(velocity);
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
				dir = moveDirection * -speed;
			if (barrelState == Right)
				dir = moveDirection * speed;
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