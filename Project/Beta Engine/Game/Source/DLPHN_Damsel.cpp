//------------------------------------------------------------------------------
//
// File Name:	DLPHN_Damsel.cpp
// Author(s):	David Wong Cascanate (david.wongcascante)
// Project:		Donkey Kong
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdafx.h>
#include "DLPHN_Damsel.h"

#include <GameObject.h>
#include <Animation.h>
#include <Sprite.h>
#include <Parser.h>
#include <Random.h>

//------------------------------------------------------------------------------
// Public Member Functions
//------------------------------------------------------------------------------
namespace DLPHN
{
	Damsel::Damsel()
		: Component("Damsel"), waitCount(0.0f), waitTime(5.0f), animation(nullptr),
		idleCount(0), idleMaxCount(2)
	{

	}

	Damsel::~Damsel()
	{
		Shutdown();
	}

	Component* Damsel::Clone() const
	{
		return new Damsel(*this);
	}

	void Damsel::Initialize()
	{
		// Get the animation and sprite
		sprite = GetOwner()->GetComponent<Sprite>();
		animation = GetOwner()->GetComponent<Animation>();
	}

	void Damsel::Update(float dt)
	{
		if (waitCount > waitTime)
		{
			if (!animation->IsRunning())
			{
				if (idleCount < idleMaxCount)
				{
					++idleCount;
					animation->Play(0.3f, false);
				}
				else
				{
					waitTime = RandomRange(3.0f, 10.0f);
					waitCount = 0;
					idleCount = 0;
					sprite->SetFrame(0);
				}
			}
		}
		else
		{
			waitCount += dt;
		}
	}

	void Damsel::Serialize(Parser& parser) const
	{

	}

	void Damsel::Deserialize(Parser& parser)
	{

	}

	void Damsel::Shutdown()
	{

	}
}