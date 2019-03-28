//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.cpp
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
#include "Matrix2DStudent.h"

#include <math.h>

//------------------------------------------------------------------------------

namespace CS230
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for matrix. Sets all data in matrix to 0.
	Matrix2D::Matrix2D()
	{
		memset(m, 0, sizeof(int) * 9);
	}

	// This function creates an identity matrix and returns it.
	Matrix2D Matrix2D::IdentityMatrix()
	{
		Matrix2D iM;

		// loop through and set diagonal to 1s
		for (unsigned i = 0; i < 3; i++)
			iM.m[i][i] = 1.0f;

		return iM;
	}

	// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
	// (NOTE: Care must be taken when pResult = pMtx.)
	Matrix2D Matrix2D::Transposed() const
	{
		Matrix2D t;
		
		// loop through and flip rows/columns
		for (unsigned row = 0; row < 3; row++)
			for (unsigned col = 0; col < 3; col++)
				t.m[row][col] = m[col][row];

		return t;
	}

	// This function creates a translation matrix from x & y and returns it.
	Matrix2D Matrix2D::TranslationMatrix(float x, float y)
	{
		// start with identity matrix
		Matrix2D tM(IdentityMatrix());
		
		// set new x/y translations
		tM.m[0][2] = x;
		tM.m[1][2] = y;

		return tM;
	}

	// This function creates a scaling matrix from x & y and returns it.
	Matrix2D Matrix2D::ScalingMatrix(float x, float y)
	{
		// start with identity matrix
		Matrix2D sM(IdentityMatrix());

		// set new x/y scale
		sM.m[0][0] = x;
		sM.m[1][1] = y;

		return sM;
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
	// Return the resulting matrix.
	// Converting from degrees to radians can be performed as follows:
	//	 radians = (angle * M_PI) / 180.0f
	Matrix2D Matrix2D::RotationMatrixDegrees(float angle)
	{
		// convert to rads
		angle = (angle * static_cast<float>(M_PI)) / 180.0f;

		return RotationMatrixRadians(angle);
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in radians.
	// Return the resulting matrix.
	Matrix2D Matrix2D::RotationMatrixRadians(float angle)
	{
		// start with identity matrix
		Matrix2D rM(IdentityMatrix());

		// set new x/y scale
		rM.m[0][0] = cos(angle);
		rM.m[1][0] = sin(angle);

		rM.m[0][1] = -rM.m[1][0];
		rM.m[1][1] =  rM.m[0][0];

		return rM;
	}

	// This function multiplies the current matrix with another and returns the result as a new matrix.
	// HINT: Use the RowColumnMultiply function to help write this function.
	Matrix2D Matrix2D::operator*(const Matrix2D& other) const
	{
		Matrix2D product;

		// loop through all indices and multiply with each other
		for (unsigned row = 0; row < 3; row++)
			for (unsigned col = 0; col < 3; col++)
				product.m[row][col] = RowColumnMultiply(other, row, col);
		
		return product;
	}

	// This function multiplies the current matrix with another, storing the result in the current matrix.
	// HINT: Use the RowColumnMultiply function to help write this function.
	Matrix2D& Matrix2D::operator*=(const Matrix2D& other)
	{
		// multiply using operator*
		*this = *this * other;

		return *this;
	}

	// This function multiplies a matrix with a vector and returns the result as a new vector.
	// Result = Mtx * Vec.
	Vector2D Matrix2D::operator*(const Vector2D& vec) const
	{
		// Convert Vector to translation matrix and multiply
		Matrix2D product = *this * Matrix2D(TranslationMatrix(vec.x, vec.y));

		return Vector2D(product.m[0][2], product.m[1][2]);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Multiplies a row of the current matrix by a column in another and returns the result.
	float Matrix2D::RowColumnMultiply(const Matrix2D& other, unsigned row, unsigned col) const
	{
		float product = 0.0f;

		// loop-multiply and add to the product
		for (unsigned i = 0; i < 3; i++)
			product += m[row][i] * other.m[i][col];

		return product;
	}
}

//------------------------------------------------------------------------------
