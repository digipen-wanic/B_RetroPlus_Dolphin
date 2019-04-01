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
#include <Component.h>
#include <Serializable.h>

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------
class Animation;
class Sprite;
//------------------------------------------------------------------------------
// Public Structs
//------------------------------------------------------------------------------
namespace DLPHN
{
	class Damsel : public Component, public Serializable
	{
	private:
		//------------------------------------------------------------------------------
		// Private Member Variables
		//------------------------------------------------------------------------------
		// The timer that keeps track of how long the damsel waits before doing an idle animation
		float waitCount;
		// The amount of time we wait before doing an idle animation
		float waitTime;
		// The amount of times we go through the animation
		unsigned idleCount;
		// The maximum amount of times we go through this animation
		const unsigned idleMaxCount;
		// The animation component
		Animation* animation;
		// The sprite
		Sprite* sprite;
	public:
		//------------------------------------------------------------------------------
		// Public Member Functions
		//------------------------------------------------------------------------------
		// Constuctor
		Damsel();
		// Destructor
		~Damsel();
		// Clones the component
		// Returns: A clone of the component
		Component* Clone() const override;
		// Initializes the component
		void Initialize() override;
		// Serializes the Damsel to a file
		// Params:
		//	parser: The parser we serialize to
		void Serialize(Parser& parser) const override;
		// Deserializes the Damsel from a file
		// Params:
		//	parser: The parser we deserialize from
		void Deserialize(Parser& parser) override;
		// Updates the component
		// Params:
		//	dt: The amount of time between frames
		void Update(float dt) override;
		// Shuts down the component
		void Shutdown() override;
	};

}