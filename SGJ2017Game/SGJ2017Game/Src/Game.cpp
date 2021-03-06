#include "Game.hpp"

#include <Time/TimeWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/SkyboxWorldComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Physics2D/Physics2DColliders.hpp>
#include <Physics2D/Rigidbody2DComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Resources/SoundResource.hpp>
#include "Rendering/ViewportWorldComponent.hpp"

#include "GameManagerWorldComponent.hpp"
#include "GameManagerSystem.hpp"
#include "Level.hpp"
#include "PlayerControllerComponent.hpp"
#include "PlayerUpdateSystem.hpp"
#include "CameraMovementSystem.hpp"
#include "GameDebugWorldComponent.hpp"
#include "GameDebugSystem.hpp"
#include "TileComponent.hpp"
#include "CameraMovementComponent.hpp"


using namespace Poly;

DEFINE_GAME(SGJGame)

void SGJGame::Init()
{
	gEngine->RegisterGameUpdatePhase(SGJ::GameManagerSystem::Update);
	gEngine->RegisterGameUpdatePhase(SGJ::PlayerUpdateSystem::Update);
	gEngine->RegisterGameUpdatePhase(SGJ::CameraMovementSystem::CameraMovementUpdatePhase);
	//gEngine->RegisterGameUpdatePhase(SGJ::Debug::DebugSystemPhase);
	
	DeferredTaskSystem::AddWorldComponentImmediate<SGJ::GameManagerWorldComponent>(gEngine->GetActiveScene());
	DeferredTaskSystem::AddWorldComponentImmediate<SGJ::GameDebugWorldComponent>(gEngine->GetActiveScene());

	SGJ::GameManagerSystem::PrepareNonlevelObjects(gEngine->GetActiveScene());
		
	Entity* Camera = gEngine->GetActiveScene()->GetWorldComponent<SGJ::GameManagerWorldComponent>()->Camera.Get();
	gEngine->GetActiveScene()->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, gEngine->GetActiveScene()->GetComponent<Poly::CameraComponent>(Camera));
	DeferredTaskSystem::AddComponent<PostprocessSettingsComponent>(gEngine->GetActiveScene(), Camera);

	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(gEngine->GetActiveScene(), "HDR/HDR.hdr", eResourceSource::GAME);
	// load levels
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level0.csv");
	//SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level0_1.csv");
	//SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level0_2.csv"); // gut
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level0_3.csv"); // gut
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level0_2.csv");
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level1.csv");
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level2.csv");
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level3.csv");
	SGJ::GameManagerSystem::LoadLevel(gEngine->GetActiveScene(), "Levels/Level4.csv");
	SGJ::GameManagerSystem::SpawnLevel(gEngine->GetActiveScene(), 0);

	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/jump-sound.ogg", eResourceSource::GAME));
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/death-sound.ogg", eResourceSource::GAME));
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/powerup.ogg", eResourceSource::GAME));
};

void SGJGame::Deinit()
{
	SGJ::GameManagerSystem::Cleanup(gEngine->GetActiveScene());
	for(SoundResource* res : SoundsPreload)
		ResourceManager<SoundResource>::Release(res);
}
