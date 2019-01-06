#include "Game.hpp"

#include <ECS/DeferredTaskSystem.hpp>
#include "Rendering/Camera/CameraComponent.hpp"
#include "Rendering/ViewportWorldComponent.hpp"
#include "Rendering/SkyboxWorldComponent.hpp"
#include "Rendering/Lighting/LightSourceComponent.hpp"
#include "Rendering/Particles/ParticleComponent.hpp"
#include <Resources/ResourceManager.hpp>

#include "CameraManagerSystem.hpp"
#include "GameManagerWorldComponent.hpp"
#include <AnimKeys.hpp>
#include <KeyFrameAnimationSystem.hpp>
#include <KeyFrameAnimationComponent.hpp>
#include <KeyFrameCameraComponent.hpp>
#include "GameCameraComponent.hpp"

using namespace Poly;

DEFINE_GAME(Game)

const static float PLAYER_CAMERA_HEIGHT = 1.25f;
const static float PLAYER_CAMERA_NEAR = 0.1f;
const static float PLAYER_CAMERA_FAR = 300.f;
const static Angle PLAYER_CAMERA_FOV = 60_deg;

void Game::Init()
{
	InitDebugAnimation();

}

void Game::InitDebugAnimation()
{
	Scene* world = gEngine->GetActiveScene();
	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(gEngine->GetActiveScene());
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ship = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponent<KeyFrameAnimationComponent>(world, ship, "SuperFinalShip");
	MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, ship, "Models/Latacz3.fbx", eResourceSource::GAME);
	meshCmp->SetBlendingMode(eBlendingMode::NONE);
	
	Entity* shipModel = DeferredTaskSystem::SpawnEntityImmediate(world);
	shipModel->SetParent(ship);
	shipModel->GetTransform().SetLocalRotation(Quaternion(Vector::UNIT_X, 90.0_deg) * Quaternion(Vector::UNIT_Y, 180.0_deg));
	MeshRenderingComponent* shipMeshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, shipModel, "Models/Ship/SGJ18_Ship.obj", eResourceSource::GAME);
	shipMeshCmp->SetMaterial(0, Material(Color::BLACK, Color::WHITE, 1.0f, 0.9f, 0.5f));

	SpawnParticles(world, ship);


	Entity* track = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, track, "Models/Track/FinalSuperTrackEXTRAGandalf.obj", eResourceSource::GAME);

	gEngine->RegisterGameUpdatePhase(SGJJam::CameraManagerSystem::Update);
	gEngine->RegisterGameUpdatePhase(KeyFrameAnimationSystem::KeyFrameAnimationUpdatePhase);

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	camera->SetParent(ship);
	camera->GetTransform().SetLocalRotation(EulerAngles(65_deg, 0_deg, -90_deg));
	camera->GetTransform().SetLocalTranslation(Vector(-10, 0, 10));
	ship->GetTransform().SetLocalTranslation(Vector(0, 0, -5));

	DeferredTaskSystem::AddComponentImmediate<SoundListenerComponent>(world, camera);
	PostprocessSettingsComponent* postCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(gEngine->GetActiveScene(), camera);
	postCmp->MotionBlurScale = 0.01f;
	postCmp->DOFSize = 0.2f;
	postCmp->DOFPoint = 10.0f;
	postCmp->DOFRange = 20.0f;
	postCmp->DOFShow = 0.0f;
	postCmp->Exposure = 2.0f;

	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(gEngine->GetActiveScene(), camera, PLAYER_CAMERA_FOV, PLAYER_CAMERA_NEAR, PLAYER_CAMERA_FAR);
	//DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 15.0f, 0.01f, 0.1f);
	DeferredTaskSystem::AddComponentImmediate<GameCameraComponent>(world, camera, eGameCameraPolicy::FOLLOW, ship);
	gEngine->GetActiveScene()->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, gEngine->GetActiveScene()->GetComponent<CameraComponent>(camera));

	//camera->GetTransform().SetLocalRotation(EulerAngles(65_deg, 0_deg, -90_deg));
	//camera->GetTransform().SetLocalTranslation(Vector(-10, 0, 10));
	//ship->GetTransform().SetGlobalTranslation(Vector(0, 0, -5));
	
	Entity* camera2 = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<GameCameraComponent>(world, camera2, eGameCameraPolicy::LOOK_AT, ship);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(gEngine->GetActiveScene(), camera2, 30_deg, PLAYER_CAMERA_NEAR, PLAYER_CAMERA_FAR);
	PostprocessSettingsComponent* postCmp2 = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(gEngine->GetActiveScene(), camera2);
	postCmp2->MotionBlurScale = 0.01f;
	postCmp2->DOFSize = 0.4f;
	postCmp2->DOFPoint = 70.0f;
	postCmp2->DOFRange = 50.0f;
	// postCmp2->DOFShow = 1.0f;
	postCmp2->Exposure = 2.0f;

	Entity* camera3 = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<GameCameraComponent>(world, camera3, eGameCameraPolicy::ORBIT, ship);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(gEngine->GetActiveScene(), camera3, PLAYER_CAMERA_FOV, PLAYER_CAMERA_NEAR, PLAYER_CAMERA_FAR);
	PostprocessSettingsComponent* postCmp3 = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(gEngine->GetActiveScene(), camera3);
	postCmp3->MotionBlurScale = 0.01f;
	postCmp3->DOFSize = 0.2f;
	postCmp3->DOFPoint = 10.0f;
	postCmp3->DOFRange = 20.0f;
	postCmp3->DOFShow = 0.0f;
	postCmp3->Exposure = 2.0f;

	// Debuging camera
	//Entity* camera4 = DeferredTaskSystem::SpawnEntityImmediate(world);
	//DeferredTaskSystem::AddComponentImmediate<GameCameraComponent>(world, camera4, eGameCameraPolicy::FREE, shipRoot);
	//DeferredTaskSystem::AddComponentImmediate<CameraComponent>(gEngine->GetActiveScene(), camera4, PLAYER_CAMERA_FOV, PLAYER_CAMERA_NEAR, PLAYER_CAMERA_FAR);

	//Camera postprocesses
	//gameMgrCmp->CameraAnimations = KeyFrameAnimationSystem::LoadAnimTracks("Animations/Camera.x");
	//DeferredTaskSystem::AddComponentImmediate<KeyFrameCameraComponent>(world, camera, "Camera");

	//camera switching
	//gameMgrCmp->CameraSwitches = KeyFrameAnimationSystem::LoadAnimTracks("Animations/CameraSwitches.x").Get("Camera").Value();

	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, "HDR/HDR.hdr", eResourceSource::GAME);

	Quaternion DirLightRot = Quaternion(Vector::UNIT_Y, 80_deg) * Quaternion(Vector::UNIT_X, -80_deg);

	Entity* directionalLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<Poly::DirectionalLightComponent>(world, directionalLight, Color(1.0f, 0.9f, 0.8f), 0.8f);
	Poly::EntityTransform& dirLightTrans = directionalLight->GetTransform();
	dirLightTrans.SetLocalRotation(DirLightRot);

	//music
	Entity* music = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<SoundEmitterComponent>(world, music, "Audio/neocrey-System_Shock.ogg", eResourceSource::GAME);
	SoundSystem::PlayEmitter(world, music);
	SoundSystem::SetEmitterGain(world, music, 0.15f);
	SoundSystem::LoopEmitter(world, music);

	//ship sounds
	Entity* sound = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<SoundEmitterComponent>(world, sound, "Audio/constantcarsound.ogg", eResourceSource::GAME);
	SoundSystem::PlayEmitter(world, sound);
	SoundSystem::SetEmitterGain(world, sound, 8.0f);
	SoundSystem::LoopEmitter(world, sound);
}

void Game::SpawnParticles(Poly::Scene * world, Poly::Entity * ship)
{
	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	particlesEnt->SetParent(ship);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.Emissive = Color::RED * 10.0f;
	settings.BurstTimeMin = 0.01f;
	settings.BurstTimeMax = 0.05f;	
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.01f;
		p->Acceleration = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 2.0f);
		p->Scale = Vector::ONE * RandomRange(0.01f, 0.2f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp(0.0f, 0.2f, SmoothStep(0.0f, 1.0f, t));
	};
	
	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);

	Entity* particlesEnt2 = DeferredTaskSystem::SpawnEntityImmediate(world);
	particlesEnt->SetParent(ship);

	SpritesheetSettings spriteSettings2;
	spriteSettings2.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings2.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings2;
	settings2.MaxSize = 1000;
	settings2.Spritesheet = spriteSettings;
	settings2.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings2.Albedo = Color(0.0f, 0.0f, 0.0f, 0.1f);
	settings2.BurstTimeMin = 0.01f;
	settings2.BurstTimeMax = 0.05f;
	settings2.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.01f;
		p->Acceleration = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 2.0f);
		p->Scale = Vector::ONE * RandomRange(0.01f, 0.2f);
	};
	settings2.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp(0.4f, 0.0f, SmoothStep(0.0f, 1.0f, t));
	};

	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt2, settings2);
}

void Game::Deinit()
{

}