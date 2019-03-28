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

// Public Member Functions
namespace DLPHN
{
	DonkeyKong::DonkeyKong()
		: Component("DonkeyKong"), current(DK_AIState::START_BARREL), idleTime(2.0f), idleTimer(0.0f), barrelArchetype(nullptr), animation(nullptr)
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
		// Get the DK's animations
		animation = GetOwner()->GetComponent<Animation>();
	}

	void DonkeyKong::Initialize()
	{
		current = DK_AIState::START_BARREL;
	}

	void DonkeyKong::Update(float dt)
	{
		// Update DK's AI
		switch (current)
		{
		case DK_AIState::START_BARREL:
			// TODO: Add animation for throw the barrel
			ThrowBarrel();
			// ... and immediately switch to the idle state
			current = DK_AIState::IDLE;
			break;
		case DK_AIState::IDLE:
			// Stay in the idle state through two iterations of the his idle animation
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
			if (animation->IsDone())
			{
				// Switch states when this animation is done playing the grabbing animation
				current = DK_AIState::THROW_BARREL;
			}
			break;
		case DK_AIState::THROW_BARREL:
			// TODO: Play the animation for throwing the barrel
			// When the animation is done playing then we change states back to idle
			if (animation->IsDone())
			{
				current = DK_AIState::IDLE;
			}
			break;
		}
	}

	void DonkeyKong::ThrowBarrel() const
	{

	}
}