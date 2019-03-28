//------------------------------------------------------------------------------
//
// File Name:	SpriteTilemap.cpp
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
#include "SpriteTilemap.h"

#include "Space.h"
#include "Tilemap.h"
#include "Transform.h"
#include <Vector2D.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite for use with a tilemap.
SpriteTilemap::SpriteTilemap()
	: map(nullptr)
{
}

// Clone the sprite, returning a dynamically allocated copy.
Component* SpriteTilemap::Clone() const
{
	return new SpriteTilemap(*this);
}

// Draw a sprite (Sprite can be textured or untextured).
void SpriteTilemap::Draw()
{
	if (map == nullptr)
		return;

	int columns = map->GetWidth();
	int rows = map->GetHeight();
	Vector2D scale = transform->GetScale();

	// loop through values in map
	for (int c = 0; c < columns; c++)
	{
		for (int r = 0; r < rows; r++)
		{
			int value = map->GetCellValue(c, r);

			// skip empty slots
			if (value < 1)
				continue;

			// set frame and offset, then draw
			SetFrame(value - 1);
			Vector2D pos(scale.x * c, -scale.y * r);
			Sprite::Draw(pos);
		}
	}
}

// Sets the tilemap data that will be used by the sprite.
// Params:
//   map = A pointer to the tilemap resource.
void SpriteTilemap::SetTilemap(const Tilemap* map_)
{
	map = map_;
}

//------------------------------------------------------------------------------
