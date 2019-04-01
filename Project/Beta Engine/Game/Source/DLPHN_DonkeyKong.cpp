//------------------------------------------------------------------------------
//
// File Name:	DLPHN_DonkeyKong.cpp
// Author(s):	David Wong Cascanate (david.wongcascante)
// Project:		Donkey Kong
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <stdafx.h>
#include "DLPHN_DonkeyKong.h"

#include <Space.h>
#include <GameObject.h>
#include <Transform.h>
#include <GameObjectFactory.h>
#include <GameObjectManager.h>
#include <Engine.h>
#include <SoundManager.h>
#include <Space.h>
#include <Animation.h>
#include <Sprite.h>
#include <Random.h>

//------------------------------------------------------------------------------
// Defines:
//------------------------------------------------------------------------------
#define AVOID_ANIMATIONS true

namespace DLPHN
{
	//------------------------------------------------------------------------------
	// Public Member Functions:
	//------------------------------------------------------------------------------

	DonkeyKong::DonkeyKong()
		: Component("DonkeyKong"), current(DK_AIState::GRAB_BARREL), prev(DK_AIState::IDLE),
		idleCount(0), maxIdleCount(2), maxBarrelThrows(2), barrelThrows(0),
		barrelArchetype(nullptr), animation(nullptr), throwTimer(0.0f), throwDuration(0.8f),
		idleTimer(0), idleDuration(0.4f), barrelCycleThrown(false), throwTimerOffset(0.4f)
	{
	}

	DonkeyKong::~DonkeyKong()
	{
		Shutdown();
	}

	Component* DonkeyKong::Clone() const
	{
		return new DonkeyKong(*this);
	}

	void DonkeyKong::Initialize()
	{
		// Finding the barrel archetype by name
		barrelArchetype = GameObjectFactory::GetInstance().CreateObject("Barrel");
		current = DK_AIState::GRAB_BARREL;
		// Get the DK's animations
		animation = GetOwner()->GetComponent<Animation>();
		sprite = GetOwner()->GetComponent<Sprite>();
		transform = GetOwner()->GetComponent<Transform>();
		// Play a single run throw the idle animation
		animation->Play(0.4f, 5, 6, false);
		++idleCount;
		// Set up the beating chest sound effect
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->AddEffect("DLPHN_sound_donkeyBeatingChest.wav");
	}

	void DonkeyKong::Update(float dt)
	{
		// Update DK's AI
		switch (current)
		{
		case DK_AIState::IDLE:
			if (current != prev)
			{
				idleTimer = 0;
				maxIdleCount = RandomRange(2, 4);
				++idleCount;
				// Set the beginning of his animation
				sprite->SetFrame(5);
				// Play the chest beating sound effect
				soundManager->PlaySound("DLPHN_sound_donkeyBeatingChest.wav");
			}
			prev = current;
			idleTimer += dt;
			if (idleTimer > idleDuration)
			{
				// Play his idle animation twice before changing animations
				if (idleCount < maxIdleCount)
				{
					++idleCount;
					idleTimer = 0;
					soundManager->PlaySound("DLPHN_sound_donkeyBeatingChest.wav");
					if (sprite->GetFrame() == 5)
					{
						sprite->SetFrame(6);
					}
					else
					{
						sprite->SetFrame(5);
					}
				}
				else
				{
					idleCount = 0;
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
				// Stop the animation
				animation->Stop();
			}
			break;
		case DK_AIState::THROW_BARREL:
			// TODO: Play the animation for throwing the barrel
			// When the animation is done playing then we change states back to idle
			if (current != prev)
			{
				if (RandomRange(0, 4) == 4)
				{
					sprite->SetFrame(3);
				}
				else
				{
					sprite->SetFrame(4);
				}
			}
			prev = current;
			if (throwTimer > throwDuration)
			{
				throwTimer = 0;
				barrelCycleThrown = true;
				++barrelThrows;
				// Decide whether to throw another barrel
				if (RandomRange(barrelThrows, maxBarrelThrows) != 2)
				{
					current = DK_AIState::GRAB_BARREL;
				}
				else
				{
					barrelCycleThrown = false;
					barrelThrows = 0;
					current = DK_AIState::IDLE;
				}
			}
			else
			{
				throwTimer += dt;
				if (!barrelCycleThrown && throwTimer > (throwDuration - throwTimerOffset))
				{
					ThrowBarrel(sprite->GetFrame() == 3 ? Vector2D(0, -40) : Vector2D(80, -40));
					barrelCycleThrown = true;
				}
			}
			break;
		}
	}

	void DonkeyKong::Shutdown()
	{
		delete barrelArchetype;
	}

	void DonkeyKong::ThrowBarrel(const Vector2D& throwOffset)
	{
		GameObject* barrel = new GameObject(*barrelArchetype);
		GetOwner()->GetSpace()->GetObjectManager().AddObject(*barrel);
		Transform* barrelTransform = barrel->GetComponent<Transform>();
		barrelTransform->SetTranslation(GetOwner()->GetComponent<Transform>()->GetTranslation() + throwOffset);
	}
}