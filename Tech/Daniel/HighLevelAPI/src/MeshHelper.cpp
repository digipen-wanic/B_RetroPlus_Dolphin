//------------------------------------------------------------------------------
//
// File Name:	MeshHelper.cpp
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
#include "MeshHelper.h"

#include <MeshFactory.h>
#include <Vertex.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a colored mesh consisting of a single triangle using the Beta Framework.
// Params:
//   color0 = The color of vertex 0.
//   color1 = The color of vertex 1.
//   color2 = The color of vertex 2.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateTriangleMesh(const Color& color0, const Color& color1, const Color& color2)
{
	// Set triangle points
	Vertex v1(Vector2D(-0.5f,  0.5f), color0);
	Vertex v2(Vector2D(-0.5f, -0.5f), color1);
	Vertex v3(Vector2D( 0.5f,  0.0f), color2);

	// Add/return triangle
	MeshFactory::GetInstance().AddTriangle(v1, v2, v3);
	return MeshFactory::GetInstance().EndCreate();
}

// Create a textured quad mesh using the Beta Framework.
// Params:
//   textureSize = The UV size of the mesh, in texture coordinates.
//	 extents	 = The XY distance of the vertices from the origin.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateQuadMesh(const Vector2D& textureSize, const Vector2D& extents)
{
	// Calculate vertices
	Vertex v1(Vector2D(-extents.x, -extents.y), Vector2D(0.0f, textureSize.y));	// bottom left
	Vertex v2(Vector2D( extents.x, -extents.y), textureSize);					// bottom right
	Vertex v3(Vector2D(-extents.x,  extents.y), Vector2D(0.0f, 0.0f));			// top left
	Vertex v4(Vector2D( extents.x,  extents.y), Vector2D(textureSize.x, 0.0f));	// top right

	// Add/return quad mesh using two triangles
	MeshFactory::GetInstance().AddTriangle(v3, v1, v4);
	MeshFactory::GetInstance().AddTriangle(v4, v1, v2);

	return MeshFactory::GetInstance().EndCreate();
}

//------------------------------------------------------------------------------
