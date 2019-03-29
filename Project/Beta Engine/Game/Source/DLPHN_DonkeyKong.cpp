//============================================================
// Author: David Wong Cascante
// File Name: DLPHN_DonkeyKong.cpp
// Date Created: March 27 2019
//============================================================

// Includes
#include <stdafx.h>
#include "DLPHN_DonkeyKong.h"

#include <GameObject.h>
#include <GameObjectManager.h>
#include <Space.h>
#include <Animation.h>
#include <Sprite.h>
#include <Random.h>

// Defines 
#define AVOID_ANIMATIONS true

// Public Member Functions
namespace DLPHN
{
	DonkeyKong::DonkeyKong()
		: Component("DonkeyKong"), current(DK_AIState::GRAB_BARREL), prev(DK_AIState::IDLE),
		idleCount(0), maxIdleCount(2), maxBarrelThrows(2), barrelThrows(0), 
		barrelArchetype(nullptr), animation(nullptr)
	{
	}

	Component* DonkeyKong::Clone() const
	{
		return new DonkeyKong(*this);
	}

	void DonkeyKong::Load()
	{
		// Finding the barrel archetype by name
		barrelArchetype = GetOwner()->GetSpace()->GetObjectManager().GetArchetypeByName("Barrel");
	}

	void DonkeyKong::Initialize()
	{
		current = DK_AIState::GRAB_BARREL;
		// Get the DK's animations
		animation = GetOwner()->GetComponent<Animation>();
		sprite = GetOwner()->GetComponent<Sprite>();
	}

	void DonkeyKong::Update(float dt)
	{
		// Update DK's AI
		switch (current)
		{
		case DK_AIState::IDLE:
			if (prev != current)
			{
				idleCount = 0;
			}
			prev = current;

			if (!animation->IsRunning())
			{
				// Play his idle animation twice before changing animations
				if (idleCount < maxIdleCount)
				{
					++idleCount;
					animation->Play(0.4f, 4, 5, false);
				}
				else
				{
					current = DK_AIState::GRAB_BARREL;
				}
			}
			break;
		case DK_AIState::GRAB_BARREL:
			// Play the animation of grabbing a barrel
			if (current != prev)
			{
				animation->Play(0.4f, 1, 2, false);
			}
			prev = current;
			if (animation->IsDone())
			{
				// Switch states when this animation is done playing the grabbing animation
				current = DK_AIState::THROW_BARREL;
			}
			break;
		case DK_AIState::THROW_BARREL:
			// TODO: Play the animation for throwing the barrel
			// When the animation is done playing then we change states back to idle
			if (current != prev)
			{
				sprite->SetFrame(2);
				if (RandomRange(0, 4) == 4)
				{
					animation->Play(0.4f, 6, 6, false);
				}
				else
				{
					animation->Play(0.4f, 3, 3, false);
				}
			}
			prev = current;
			if (animation->IsDone())
			{
				ThrowBarrel();
				++barrelThrows;
				// Decide whether to throw another barrel
				if (RandomRange(barrelThrows, maxBarrelThrows) != 2)
				{
					current = DK_AIState::GRAB_BARREL;
				}
				else
				{
					barrelThrows = 0;
					current = DK_AIState::IDLE;
				}
			}
			break;
		}
	}

	void DonkeyKong::ThrowBarrel() const
	{

	}
}