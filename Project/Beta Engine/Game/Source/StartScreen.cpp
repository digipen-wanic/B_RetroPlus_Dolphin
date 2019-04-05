//------------------------------------------------------------------------------
//
// File Name:	StartScreen.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "StartScreen.h"

#include "AlphaDK.h"
#include "PlusDK.h"

// Components
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>
#include <SpriteTilemap.h>

// Resources
#include <Mesh.h>
#include <Color.h>
#include <Tilemap.h>
#include <SpriteSource.h>

// Systems
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <System.h>
#include <Input.h>
#include <MeshHelper.h>
#include <Engine.h>
#include <SoundManager.h>
#include <Random.h>
#include <SpaceManager.h>
#include <Graphics.h>
#include <Parser.h>
#include <GlobalTime.h>

Levels::StartScreen::StartScreen()
	: Level("StartScreen")
{
	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::StartScreen::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	Graphics::GetInstance().GetCurrentCamera().Reset();

	SetFileLocation("Assets/startscreen.lvl");
	GlobalTime& globalTime = GlobalTime::GetInstance();
	globalTime.SetTimeScale(1.0f);

	SetFileLocation("Assets/startscreen.lvl");
}

void Levels::StartScreen::Initialize()
{
	//Setup Sounds
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddEffect("DLPHN_music_startJingle.wav");

	std::cout << GetName() << "::Initialize" << std::endl;
	musicChannel = soundManager->PlaySound("DLPHN_music_startJingle.wav");
	LoadLevel();
}

void Levels::StartScreen::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Input& input = Input::GetInstance();

	if (input.CheckTriggered(VK_SPACE))
	{
		if (currentSelection)
			GetSpace()->SetLevel<PlusDK>();
		else
			GetSpace()->SetLevel<AlphaDK>();
	}

	if (input.CheckTriggered(VK_UP) || input.CheckTriggered(VK_DOWN))
		currentSelection = !currentSelection;

	Transform* select = GetSpace()->GetObjectManager().GetObjectByName("SelectionAsterisk")->GetComponent<Transform>();

	if (currentSelection)
		select->SetTranslation(Vector2D(-280.0f, -110.0f));
	else
		select->SetTranslation(Vector2D(-280.0f, -45.0f));
}

void Levels::StartScreen::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	//SaveLevel();
	musicChannel->stop();
	soundManager->Shutdown();
}

void Levels::StartScreen::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;
}