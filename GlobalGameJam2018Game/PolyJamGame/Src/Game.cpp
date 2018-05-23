#include "Game.hpp"

#include <ECS/DeferredTaskSystem.hpp>

#include "GameManagerSystem.hpp"
#include "ActorSystem.hpp"
#include "PlayerSystem.hpp"
#include "TransmitterSystem.hpp"
#include "EnemyAISystem.hpp"
#include "ReceiverSystem.hpp"
#include "CollisionSystem.hpp"
#include "EnemyAISystem.hpp"
#include "ResourcesManagementSystem.hpp"

#include "GameManagerWorldComponent.hpp"
#include "TransmissionStatusWorldComponent.hpp"


using namespace Poly;
using namespace GGJGame;

DEFINE_GAME(Game)

void Game::Init()
{
	gEngine->SetMouseCapture(true);
	gEngine->SetMouseVisible(false);

	gEngine->RegisterGameUpdatePhase(ReceiverSystem::Update);
	gEngine->RegisterGameUpdatePhase(PlayerSystem::Update);
	gEngine->RegisterGameUpdatePhase(ActorSystem::Update);
	gEngine->RegisterGameUpdatePhase(TransmitterSystem::Update);
	gEngine->RegisterGameUpdatePhase(EnemyAISystem::Update);
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);
	gEngine->RegisterGameUpdatePhase(CollisionSystem::Update);
	gEngine->RegisterGameUpdatePhase(EnemyAISystem::Update);

	World* world = gEngine->GetWorld();
	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(world);
	DeferredTaskSystem::AddWorldComponentImmediate<TransmissionStatusWorldComponent>(world); //GUI transmission status

	ResourcesManagementSystem::PreloadResources(world);

	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/gunfire.ogg", eResourceSource::GAME));
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/footsteps.ogg", eResourceSource::GAME));
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/bomb.ogg", eResourceSource::GAME));
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Audio/fireworks.ogg", eResourceSource::GAME));
	MeshPreload.PushBack(ResourceManager<MeshResource>::Load("Models/Robot/Robot.fbx", eResourceSource::GAME));
	MeshPreload.PushBack(ResourceManager<MeshResource>::Load("Models/Bullet.fbx", eResourceSource::GAME));

	GameManagerSystem::InitializeScene(world);
	GameManagerSystem::SpawnLevel(world, eLevels::RELEASE_LEVEL);
}

void Game::Deinit()
{
	GameManagerSystem::DenitializeScene(gEngine->GetWorld());

	for (SoundResource* res : SoundsPreload)
		ResourceManager<SoundResource>::Release(res);

	for (MeshResource* res : MeshPreload)
		ResourceManager<MeshResource>::Release(res);
}
