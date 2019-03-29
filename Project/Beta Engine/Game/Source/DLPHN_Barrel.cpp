//============================================================
// Author: Jayden Yamada
// File Name: DLPHN_Barrel.cpp
// Date Created: March 29 2019
//============================================================
#include "stdafx.h"
#include "DLPHN_Barrel.h"
#include <GameObject.h>
#include <Physics.h>

DLPHN::Barrel::Barrel(float speed_) : Component("Barrel"), speed(speed_)
{

}

Component * DLPHN::Barrel::Clone() const
{
	return new Barrel(*this);
}

void DLPHN::Barrel::Load()
{
}

void DLPHN::Barrel::Initialize()
{
	phys = GetOwner()->GetComponent<Physics>();
	phys->SetGravity(Vector2D(0, -1));
}

void DLPHN::Barrel::Update(float dt)
{
}
