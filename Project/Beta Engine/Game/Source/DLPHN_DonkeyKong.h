#pragma once
//------------------------------------------------------------------------------
//
// File Name:	DLPHN_DonkeyKong.cpp
// Author(s):	David Wong Cascanate (david.wongcascante)
// Project:		Donkey Kong
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


// Includes
#include <Component.h>
#include <Serializable.h>
#include <Vector2D.h>

// Forward Declarations
class GameObject;
class Animation;
class Sprite;
class SoundManager;
class Transform;

namespace DLPHN
{
	// The AI states for Donkey Kong
	enum DK_AIState
	{
		IDLE, GRAB_BARREL, THROW_BARREL
	};

	// Controls Donkey Kong's behavior
	class DonkeyKong : public Component
	{
	private:
		// Whether the AI is disabled or not
		const bool aiDisabled;
		// The current AI state for Donkey Kong
		DK_AIState current;
		// The previous state for DK
		DK_AIState prev;
		// How much time has passed for every frame of animation in DK's idle state
		float idleTimer;
		// How much time should each of DK's frames go through
		float idleDuration;
		// The maximum amount of times DK goes through his idle state
		int maxIdleCount;
		// Counts how many times DK goes through his idle state frames
		int idleCount;
		// The maximum amount of barrels DK is allowed to throw at a time
		const int maxBarrelThrows;
		// Counts how many times DK throws a barrel
		int barrelThrows;
		// Throw barrel timer
		float throwTimer;
		// How long the throwing lasts
		float throwDuration;
		// Whether DK has thrown a barrel since the cycle began
		bool barrelCycleThrown;
		// The offset from the timer where DK actually throws the barrel, and not just pose
		float throwTimerOffset;
		// The frame DK displays when throwing the barrel downward
		int downThrowFrame;
		// The amount of frames DK has during his idle animation
		int idleFrameCount;
		// The beginning of the idle animation
		int idleFrameStart;
		// The sound file to play when DK beats his chest
		std::string monkeySoundFile;
		// The archetype for the barrel
		GameObject* barrelArchetype;
		// The animation that we are controlling
		Animation* animation;
		// DK's sprite
		Sprite* sprite;
		// The tranform
		Transform* transform;
		// The sound manager
		SoundManager* soundManager;

		// Spawns a barrel and throws it down the beams
		// Params:
		//	throwOffset: The offset from the middle of DK where the barrel will spawn
		void ThrowBarrel(const Vector2D& throwOffset);
	public:
		// Constructor
		DonkeyKong();
		// Destructor
		~DonkeyKong();
		// Clones the component
		// Returns: A copy of the componnet
		Component* Clone() const override;
		// Initializes the component
		void Initialize() override;
		// Updates the component
		void Update(float dt) override;
		// Shuts down the component
		void Shutdown() override;
	};
}
