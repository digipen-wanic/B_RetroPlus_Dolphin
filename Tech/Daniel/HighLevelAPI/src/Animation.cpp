//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
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
#include "Animation.h"

#include "Space.h"
#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Construct a new animation object.
// Params:
//   sprite = The sprite this animation will be manipulating.
Animation::Animation()
	: Component("Animation"), frameIndex(0), frameCount(0), frameStart(0), frameDelay(0.0f), frameDuration(0.0f),
	  isRunning(false), isLooping(false), isDone(false),
	  sprite(nullptr)
{
}

// Clone an animation, returning a dynamically allocated copy.
Component* Animation::Clone() const
{
	return new Animation(*this);
}

// Initialize components.
void Animation::Initialize()
{
	sprite = GetOwner()->GetComponent<Sprite>();
}

// Play a simple animation sequence (0 .. frameCount).
// Params:
//   frameStart = The starting frame for the sequence.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(unsigned frameStart_, unsigned frameCount_, float frameDuration_, bool isLooping_)
{
	// set initial values
	frameStart    = frameStart_;
	frameCount    = frameCount_;
	frameDuration = frameDuration_;
	isLooping     = isLooping_;

	// more initial values
	frameDelay = frameDuration;
	frameIndex = frameStart;
	isRunning  = true;
	isDone     = false;

	// Set current frame
	sprite->SetFrame(frameIndex);
}

// Update the animation.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update(float dt)
{
	// Make sure animation is running
	if (!isRunning)
		return;

	frameDelay -= dt;
	isDone = false;

	// Go to next frame/finish
	if (frameDelay <= 0)
	{
		frameDelay = frameDuration;

		// Make sure animation is not done
		if (!isDone)
		{
			frameIndex++;

			// Make sure frameIndex doesn't exceed limit
			if (frameIndex >= frameCount)
			{
				// If the animation loops, set to start
				if (isLooping)
					frameIndex = frameStart;

				// End when finished
				isDone = true;
			}

			// Set new frame
			if (!isDone || isLooping)
				sprite->SetFrame(frameIndex);
		}
	}
}

// Determine if the animation has reached the end of its sequence.
// Returns:
//	 The value in isDone.
bool Animation::IsDone() const
{
	return isDone;
}

//----------------------------------------------------------------------------
