//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
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
#include "SpriteSource.h"

#include <Vector2D.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new sprite source object.
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 texture = A pointer to a texture that has been loaded by the Alpha Engine.
SpriteSource::SpriteSource(int numCols, int numRows, Texture* texture)
	: numRows(numRows), numCols(numCols), texture(texture)
{
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Returns:
//	 A pointer to the sprite source texture.
Texture* SpriteSource::GetTexture() const
{
	return texture;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// Returns:
//	 The calculated frame count.
unsigned SpriteSource::GetFrameCount() const
{
	return numCols * numRows;
}

// Stores the UV coordinates of the specified frame in the given vector.
// Params:
//	 frameIndex = A frame index within a sprite sheet.
//   textureCoords = Vector that will contain the UV coordinates.
void SpriteSource::GetUV(unsigned int frameIndex, Vector2D& textureCoords) const
{	
	// Calculate U/V sizes
	float USize = 1.0f / numCols;
	float VSize = 1.0f / numRows;

	// Calculate row/column of provided index
	int frameRow = frameIndex / numCols;
	int frameCol = frameIndex % numCols;

	// Set new texture coordinates based on UVsizes and indices
	textureCoords.x = USize * frameCol;
	textureCoords.y = VSize * frameRow;
}

//----------------------------------------------------------------------------
