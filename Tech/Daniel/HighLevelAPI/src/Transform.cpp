//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
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
#include "Transform.h"

#include "Matrix2DStudent.h"
#include <Parser.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Transform constructor.
// Params:
//	 x = Initial world position on the x-axis.
//	 y = Initial world position on the y-axis.
Transform::Transform(float x, float y)
	: Component("Transform"), translation(x, y), rotation(0.0f), scale(50.0f, 50.0f), isDirty(true)
{
}

// Transform constructor.
// Params:
//	 translation = World position of the object.
//   scale		 = Width and height of the object.
//   rotation	 = Rotation of the object about the z-axis.
Transform::Transform(Vector2D translation, Vector2D scale, float rotation)
	: Component("Transform"), translation(translation), rotation(rotation), scale(scale), isDirty(true)
{
}

// Clone the transform, returning a dynamically allocated copy.
Component* Transform::Clone() const
{
	return new Transform(*this);
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// Returns:
//	 A reference to the component's matrix structure
const CS230::Matrix2D& Transform::GetMatrix()
{
	CalculateMatrices();

	return matrix;
}

// Get the inverse of the transform matrix.
// Returns:
//	 A reference to the component's inverse matrix.
const CS230::Matrix2D& Transform::GetInverseMatrix()
{
	CalculateMatrices();

	return inverseMatrix;
}

// Set the translation of a transform component.
// Params:
//	 translation = Reference to a translation vector.
void Transform::SetTranslation(const Vector2D& translation_)
{
	// make sure translation is different
	if (translation.x != translation_.x || translation.y != translation_.y)
	{
		translation = translation_;
		isDirty = true;
	}
}

// Get the translation of a transform component.
// Returns:
//	 A reference to the component's translation structure.
const Vector2D& Transform::GetTranslation() const
{
	return translation;
}

// Set the rotation of a transform component.
// Params:
//	 rotation = The rotation value (in radians).
void Transform::SetRotation(float rotation_)
{
	// make sure rotation is different
	if (rotation != rotation_)
	{
		rotation = rotation_;
		isDirty = true;
	}
}

// Get the rotation value of a transform component.
// Returns:
//	 The component's rotation value (in radians).
float Transform::GetRotation() const
{
	return rotation;
}

// Set the scale of a transform component.
// Params:
//	 translation = Reference to a scale vector.
void Transform::SetScale(const Vector2D& scale_)
{
	// make sure scale is different
	if (scale.x != scale_.x || scale.y != scale_.y)
	{
		scale = scale_;
		isDirty = true;
	}
}

// Get the scale of a transform component.
// Returns:
//	 A reference to the component's scale structure.
const Vector2D& Transform::GetScale() const
{
	return scale;
}

// Saves object data to a file.
void Transform::Serialize(Parser& parser) const
{
	parser.WriteVariable("translation", translation);
	parser.WriteVariable("rotation", rotation);
	parser.WriteVariable("scale", scale);
}

// Loads object data from a file.
void Transform::Deserialize(Parser& parser)
{
	parser.ReadVariable("translation", translation);
	parser.ReadVariable("rotation", rotation);
	parser.ReadVariable("scale", scale);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Calculates the transform matrix and its inverse using 
// translation, rotation, and scale.
void Transform::CalculateMatrices()
{
	// Check if there are changes
	if (isDirty == true)
	{
		// Create matrices for translation, scale, and rotation
		CS230::Matrix2D matrixTranslation(CS230::Matrix2D::TranslationMatrix(translation.x, translation.y));
		CS230::Matrix2D matrixRotation(CS230::Matrix2D::RotationMatrixRadians(rotation));
		CS230::Matrix2D matrixScale(CS230::Matrix2D::ScalingMatrix(scale.x, scale.y));

		// Create transform matrix
		matrix = matrixTranslation * matrixRotation * matrixScale;

		// Calculate matrices for inverse translation, inverse scale, and inverse rotation
		matrixTranslation = CS230::Matrix2D::TranslationMatrix(-translation.x, -translation.y);
		matrixRotation = CS230::Matrix2D::RotationMatrixRadians(-rotation);
		matrixScale = CS230::Matrix2D::ScalingMatrix(1.0f / scale.x, 1.0f / scale.y);

		// Create inverse matrix
		inverseMatrix = matrixScale * matrixRotation * matrixTranslation;

		isDirty = false;
	}
}

//------------------------------------------------------------------------------
