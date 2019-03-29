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
class Sprite;

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
		// The previous state for DK
		DK_AIState prev;
		// The maximum amount of times DK goes through his idle state
		const int maxIdleCount;
		// Counts how many times DK goes through his idle state
		int idleCount;
		// The maximum amount of barrels DK is allowed to throw at a time
		const int maxBarrelThrows;
		// Counts how many times DK throws a barrel
		int barrelThrows;
		// The archetype for the barrel
		GameObject* barrelArchetype;
		// The animation that we are controlling
		Animation* animation;
		// DK's sprite
		Sprite* sprite;
		// Throw barrel timer
		float throwTimer_;
		// How long the throwing lasts
		float throwDuration_;

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