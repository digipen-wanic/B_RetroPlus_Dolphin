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

// Defines 
#define AVOID_ANIMATIONS true

// Public Member Functions
namespace DLPHN
{
	DonkeyKong::DonkeyKong()
		: Component("DonkeyKong"), current(DK_AIState::THROW_BARREL), idleTime(2.0f), idleTimer(0.0f), barrelArchetype(nullptr), animation(nullptr)
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
		current = DK_AIState::THROW_BARREL;
		// Get the DK's animations
		animation = GetOwner()->GetComponent<Animation>();
	}

	void DonkeyKong::Update(float dt)
	{
		// See the transitions between DK's AI states
		DK_AIState prev = current;
		// Update DK's AI
		switch (current)
		{
		case DK_AIState::IDLE:
			// TODO: Play the idle animation
			idleTimer += dt;
			if (idleTimer >= idleTime)
			{
				// Switch states
				current = DK_AIState::GRAB_BARREL;
				idleTimer = 0.0f;
			}
			break;
		case DK_AIState::GRAB_BARREL:
			// Play the animation of grabbing a barrel
			if (animation->IsDone() || AVOID_ANIMATIONS)
			{
				// Switch states when this animation is done playing the grabbing animation
				current = DK_AIState::THROW_BARREL;
			}
			break;
		case DK_AIState::THROW_BARREL:
			// TODO: Play the animation for throwing the barrel
			// When the animation is done playing then we change states back to idle
			if (animation->IsDone() || AVOID_ANIMATIONS)
			{
				ThrowBarrel();
				current = DK_AIState::IDLE;
			}
			break;
		}

		// See the change betweens states
		if (prev != current)
		{
			std::cout << "[DLPHN::DonkeyKong] State change from " << prev << " to current " << current << std::endl;
		}
	}

	void DonkeyKong::ThrowBarrel() const
	{

	}
}