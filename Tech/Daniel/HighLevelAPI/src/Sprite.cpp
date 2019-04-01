//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
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
#include "Sprite.h"

#include <Color.h>
#include <Graphics.h>
#include <Matrix2D.h>
#include "Matrix2DStudent.h"
#include <Mesh.h>
#include "Space.h"
#include "SpriteSource.h"
#include "Transform.h"
#include <Vector2D.h>
#include <Parser.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new sprite object.
Sprite::Sprite()
	: Component("Sprite"), transform(nullptr), frameIndex(0), spriteSource(nullptr), mesh(nullptr), color(Colors::White)
{
}

// Clone the sprite, returning a dynamically allocated copy.
Component* Sprite::Clone() const
{
	return new Sprite(*this);
}

// Initialize components.
void Sprite::Initialize()
{
	transform = static_cast<Transform*>(GetOwner()->GetComponent<Transform>());
}

// Draw a sprite (Sprite can be textured or untextured).
void Sprite::Draw()
{
	Draw(Vector2D());
}

void Sprite::Draw(const Vector2D& offset)
{
	// make sure there is a mesh and transform
	if (mesh == nullptr || transform == nullptr)
		return;

	// make sure there is a spritesource
	if (spriteSource != nullptr)
	{
		// get uv coords
		Vector2D UVcoords(0.0f, 0.0f);
		spriteSource->GetUV(frameIndex, UVcoords);

		// set texture and uv coords
		Graphics::GetInstance().SetTexture(spriteSource->GetTexture(), UVcoords);
	}
	// no spritesource -> null
	else
	{
		Graphics::GetInstance().SetTexture(nullptr);
	}

	// set blend color
	Graphics::GetInstance().SetSpriteBlendColor(color);

	// Create a matrix for the offset
	Matrix2D offsetMatrix = Matrix2D::TranslationMatrix(offset.x, offset.y);

	// set transform/scale
	Graphics::GetInstance().SetTransform(offsetMatrix * reinterpret_cast<const Matrix2D&>(transform->GetMatrix()));

	// draw the mesh
	mesh->Draw();
}

// Set a sprite's transparency (between 0.0f and 1.0f).
// Params:
//   alpha = New value for the sprite's 'alpha' value.
void Sprite::SetAlpha(float alpha)
{
	color.a = alpha;
}

// Get the current value for a sprite's transparency.
float Sprite::GetAlpha() const
{
	return color.a;
}

// Set the sprite's current frame.
// (NOTE: You must validate the frame index against the sprite source's frame count.)
// Params:
//   frameIndex = New frame index for the sprite (0 .. frame count).
void Sprite::SetFrame(unsigned int frameIndex_)
{
	// make sure there is a spriteSource
	if (spriteSource == nullptr)
		return;

	// set new frame if less than frameCount
	if (frameIndex < spriteSource->GetFrameCount())
	{
		// std::cout << "Sprite::SetFrame(" << frameIndex_ << ")" << std::endl;
		frameIndex = frameIndex_;
	}
}

// Returns the index of the current frame being displayed.
unsigned Sprite::GetFrame() const
{
	return frameIndex;
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//   mesh = Pointer to a mesh created using the Alpha Engine.
void Sprite::SetMesh(Mesh* mesh_)
{
	mesh = mesh_;
}

// Set a new SpriteSource for the specified sprite.
// Params:
//	 spriteSource = A new sprite source for the sprite.
void Sprite::SetSpriteSource(SpriteSource* spriteSource_)
{
	spriteSource = spriteSource_;
}

// Set the blend color for the specified sprite.
// Params:
//	 color = A new color for the sprite.
void Sprite::SetColor(Color color_)
{
	color = color_;
}

// Retrieves the blend color for the sprite.
const Color& Sprite::GetColor() const
{
	return color;
}

// Saves object data to a file.
void Sprite::Serialize(Parser& parser) const
{
	parser.WriteVariable("frameIndex", frameIndex);
	parser.WriteVariable("color", color);
}

// Loads object data from a file.
void Sprite::Deserialize(Parser& parser)
{
	parser.ReadVariable("frameIndex", frameIndex);
	parser.ReadVariable("color", color);
}

//------------------------------------------------------------------------------
