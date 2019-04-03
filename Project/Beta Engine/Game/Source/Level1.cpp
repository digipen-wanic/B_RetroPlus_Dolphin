//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "Level1.h"

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

Levels::Level1::Level1() : Level("LevelEditor")
{
	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::Level1::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	System::GetInstance().SetWindowTitle(WindowTitle);

	Graphics::GetInstance().GetCurrentCamera().Reset();

	//Setup Sounds
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddMusic("Asteroid_Field.mp3");
	soundManager->AddMusic("DLPHN_music_main.wav");
	soundManager->AddMusic("DLPHN_music_startJingle.wav");
	soundManager->AddMusic("DLPHN_music_theme.wav");
	soundManager->AddEffect("DLPHN_sound_death.wav");
	soundManager->AddEffect("DLPHN_sound_jumpstandin.wav");
	soundManager->AddEffect("DLPHN_sound_jumpSuccess.wav");
	soundManager->AddEffect("DLPHN_sound_LevelStartFinal.wav");
	soundManager->AddEffect("DLPHN_sound_win.wav");
	soundManager->AddEffect("teleport.wav");

	soundManager->AddBank("Master Bank.strings.bank");
	soundManager->AddBank("Master Bank.bank");

	SetFileLocation("Assets/demo.lvl");
}

void Levels::Level1::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;
	musicChannel =  soundManager->PlaySound("DLPHN_music_theme.wav");
	LoadLevel();
}

void Levels::Level1::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Levels::Level1::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	//SaveLevel();
	musicChannel->stop();
	musicChannel = nullptr;
}

void Levels::Level1::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;

	soundManager->Shutdown();
}