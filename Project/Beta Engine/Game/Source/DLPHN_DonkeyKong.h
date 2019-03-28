#pragma once
//============================================================
// Author: David Wong Cascante
// File Name: DLPHN_DonkeyKong.h
// Date Created: March 27 2019
//============================================================

// Includes
#include <Component.h>
#include <Serializable.h>

// Forward Declarations
class GameObject;
class Animation;

namespace DLPHN
{
	// The AI states for Donkey Kong
	enum DK_AIState
	{
		IDLE, GRAB_BARREL, THROW_BARREL
	};

	// Controls Donkey Kong's behavior
	class DonkeyKong : public Component, public Serializable
	{
	private:
		// The current AI state for Donkey Kong
		DK_AIState current;
		// The amount of time DK stays in his idle state
		const float idleTime;
		// The timer keeping track of how much time DK is in his idle animation
		float idleTimer;
		// The archetype for the barrel
		GameObject* barrelArchetype;
		// The animation that we are controlling
		Animation* animation;

		// Spawns a barrel and throws it down the beams
		void ThrowBarrel() const;
	public:
		// Constructor
		DonkeyKong();
		// Clones the component
		// Returns: A copy of the componnet
		Component* Clone() const override;
		// Loads the component
		void Load() override;
		// Initializes the component
		void Initialize() override;
		// Updates the component
		void Update(float dt) override;
	};
}