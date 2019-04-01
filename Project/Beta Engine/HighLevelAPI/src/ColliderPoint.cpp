//------------------------------------------------------------------------------
//
// File Name:	ColliderPoint.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderPoint.h"
#include "Transform.h"
#include "Physics.h"
#include "Intersection2D.h"
#include "ColliderRectangle.h"
#include "ColliderCircle.h"
#include "GameObject.h"
#include <Graphics.h>
#include <DebugDraw.h>
#include <Parser.h>


ColliderPoint::ColliderPoint() : Collider(ColliderType::ColliderTypePoint)
{
}

Component * ColliderPoint::Clone() const
{
	return new ColliderPoint();
}

void ColliderPoint::Serialize(Parser & parser) const
{
	BaseSerialize(parser);
	parser.WriteVariable("offset", offset);
}

void ColliderPoint::Deserialize(Parser & parser)
{
	BaseDeserialize(parser);
	parser.ReadVariable("offset", offset);
}

void ColliderPoint::Draw()
{
	DebugDraw::GetInstance().AddCircle(transform->GetTranslation() + offset, 10.0f, Graphics::GetInstance().GetCurrentCamera(), Color(0,1,0,1));
}

const Vector2D& ColliderPoint::GetOffset() const
{
	return offset;
}

bool ColliderPoint::IsCollidingWith(const Collider & other, Vector2D* intersection) const
{
	//if the other collider is a point they just didn't collide (they missed just barely)
	if (other.GetType() == ColliderType::ColliderTypePoint) {
		return false;
	}
	//if the other collider is a rectangle then test if they collided
	if (other.GetType() == ColliderType::ColliderTypeRectangle) {
		const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
		Transform* rectTransform = other.GetOwner()->GetComponent<Transform>();
		return PointRectangleIntersection(transform->GetTranslation() + offset, BoundingRectangle(rectTransform->GetTranslation(), rect.GetExtents()));
	}
	//if the other collider is circle then check if they collided
	if (other.GetType() == ColliderType::ColliderTypeCircle) {
		const ColliderCircle& circ = static_cast<const ColliderCircle&>(other);
		Transform* circTransform = other.GetOwner()->GetComponent<Transform>();
		return PointCircleIntersection(transform->GetTranslation() + offset, Circle(circTransform->GetTranslation(), circ.GetRadius()));
	}
	//other wise they didn't collide
	return false;
}
