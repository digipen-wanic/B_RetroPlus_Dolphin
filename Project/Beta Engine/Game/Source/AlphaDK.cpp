//------------------------------------------------------------------------------
//
// File Name:	AlphaDK.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "AlphaDK.h"

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

FMOD::Channel* Levels::AlphaDK::musicChannel = nullptr;

Levels::AlphaDK::AlphaDK()
	: Level("AlphaDK")
{
	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::AlphaDK::Load()
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

	SetFileLocation("Assets/25metersnew.lvl");
	GlobalTime& globalTime = GlobalTime::GetInstance();
	globalTime.SetTimeScale(1.0f);
}

void Levels::AlphaDK::Initialize()
{
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

	std::cout << GetName() << "::Initialize" << std::endl;
	musicChannel = soundManager->PlaySound("DLPHN_music_theme.wav");
	LoadLevel();
}

void Levels::AlphaDK::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Levels::AlphaDK::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	//SaveLevel();
	musicChannel->stop();
	soundManager->Shutdown();
}

void Levels::AlphaDK::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;

	
}