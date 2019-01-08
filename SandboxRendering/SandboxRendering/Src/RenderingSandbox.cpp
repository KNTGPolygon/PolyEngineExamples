#include "stdlib.h" // srand rand

#include "RenderingSandbox.hpp"
#include "GameManagerSystem.hpp"
#include "ParticleTestSystem.hpp"
#include "SpriteSheetTestSystem.hpp"
#include "GameManagerWorldComponent.hpp"
#include "ParticleTestWorldComponent.hpp"
#include "SpriteSheetTestWorldComponent.hpp"

#include <Audio/SoundSystem.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Resources/ResourceManager.hpp>
#include <Time/TimeWorldComponent.hpp>
#include <UI/ScreenSpaceTextComponent.hpp>

using namespace Poly;

DEFINE_GAME(RenderingSandbox)

void RenderingSandbox::Init()
{
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);
	// gEngine->RegisterGameUpdatePhase(ParticleTestSystem::Update);
	// gEngine->RegisterGameUpdatePhase(SpriteSheetTestSystem::Update);

	Scene* scene = gEngine->GetActiveScene();
	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(scene);
	// DeferredTaskSystem::AddWorldComponentImmediate<ParticleTestWorldComponent>(scene);
	// DeferredTaskSystem::AddWorldComponentImmediate<SpriteSheetTestWorldComponent>(scene);
	
	GameManagerSystem::Init(scene);
	// ParticleTestSystem::Init(scene);
	// SpriteSheetTestSystem::Init(scene);
};

void RenderingSandbox::Deinit()
{
	Scene* scene = gEngine->GetActiveScene();
	GameManagerSystem::Deinit(scene);
	// ParticleTestSystem::Deinit(scene);
	// SpriteSheetTestSystem::Deinit(scene);
};