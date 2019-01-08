#include "ParticleTestSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Debugging/DebugDrawComponents.hpp>
#include <Debugging/DebugDrawSystem.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Input/InputWorldComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/SkyboxWorldComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Resources/ResourceManager.hpp>
#include <Time/TimeWorldComponent.hpp>
#include <UI/ScreenSpaceTextComponent.hpp>

using namespace Poly;

void ParticleTestSystem::Init(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	Vector particlesPosition = Vector(0.0f, 0.0f, 0.0f);
	// gameMgrCmp->particleDefault = SpawnEmitterDefault(scene, particlesPosition);
	// gameMgrCmp->particleWorldSpace = SpawnEmitterWorldSpace(scene, particlesPosition);
	// gameMgrCmp->particleLocalSpace = SpawnEmitterLocalSpace(scene, particlesPosition);
	// gameMgrCmp->particleAmbient = SpawnEmitterAmbient(scene, particlesPosition);
	// gameMgrCmp->particleAmbientWind = SpawnEmitterAmbientWind(scene, particlesPosition);

	// SpawnHeartSystem(scene);
}

void ParticleTestSystem::Update(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	// float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(scene, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Vector translation = Vector::UNIT_Y * (0.2f*Abs(Sin(1.0_rad*3.1415f *time + 0.5_rad)));
	Vector scale = Vector::ONE * (1.0f + 0.1f*(0.5f + 0.5f*Sin(2.0_rad*3.1415f *time + 0.5_rad)));
	Quaternion rotation = Quaternion(Vector::UNIT_Y, 0.5_rad * time);

	if (gameMgrCmp->particleHeart)
	{
		gameMgrCmp->particleHeart->GetTransform().SetGlobalTranslation(translation);
		gameMgrCmp->particleHeart->GetTransform().SetGlobalScale(scale);
		gameMgrCmp->particleHeart->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleHeartImpact0)
	{
		gameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalTranslation(translation);
		gameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalScale(scale);
		gameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleHeartImpact1)
	{
		gameMgrCmp->particleHeartImpact1->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleHeartImpact2)
	{
		gameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * 0.2f);
		gameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleLocalSpace)
	{
		gameMgrCmp->particleLocalSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time), 0.0f, Sin(100.0_deg * time)) * 6.0f);
	}

	if (gameMgrCmp->particleWorldSpace)
	{
		gameMgrCmp->particleWorldSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time + 180.0_deg), 0.0f, Sin(100.0_deg * time + 180.0_deg)) * 6.0f);
	}
}

void ParticleTestSystem::Deinit(Scene* scene)
{
	gConsole.LogInfo("ParticleTestSystem::Deinit");
}

// #pragma region Particle Examples

ParticleComponent* ParticleTestSystem::SpawnEmitterDefault(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/none.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.BurstSizeMin = 2;
	settings.BurstSizeMax = 5;
	settings.BurstTimeMax = 1.0f;
	settings.BurstTimeMin = 5.0f;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-10.0f, 10.0f);
		p->Velocity = Vector(0.0f, 0.0f, 0.0f);
		p->LifeTime = RandomRange(1.0f, 1.0f);
		p->Scale = Vector::ONE * RandomRange(10.0f, 10.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterWorldSpace(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 50;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.05f;
	settings.BurstTimeMax = 0.20f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(0.5f, 2.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterLocalSpace(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterAmbient(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";
	spriteSettings.Speed = 0.0f;

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.InitialSize = 500;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 50;
	settings.Albedo = Color(1.0f, 0.1f, 0.1f, 0.2f);
	settings.Emissive = Color::BLACK;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rndSpawn = RandomVectorRange(-1.0f, 1.0f);
		p->Position += Vector(500.0f * rndSpawn.X, 200.0f * rndSpawn.Y, 300.0f * rndSpawn.Z);
		p->Velocity = RandomVectorRange(-1.0f, 1.0f) * 0.1f;
		p->LifeTime = RandomRange(5.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.5f, 1.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterAmbientFireflies(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.InitialSize = 500;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 50;
	settings.Albedo = Color(0.5f, 0.5f, 1.0f, 0.5f);
	settings.Emissive = Color(100.0f, 0.5f, 1.0f, 1.0f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 1000.0f;
		p->Velocity = RandomVectorRange(-1.0f, 1.0f) * 10.0f;
		p->LifeTime = RandomRange(5.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.5f, 2.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterAmbientWind(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 200;
	settings.InitialSize = 0;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 200;
	settings.BurstSizeMax = 400;
	settings.Albedo = Color(1.0f, 1.0f, 1.0f, 0.1f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += Vector(-20.0f, 2.0f, 0.0f) + RandomVectorRange(-1.0f, 1.0f) * 10.0f;
		p->Velocity = Vector(RandomRange(0.75f, 1.0f) * 0.5f, 0.0f, 0.0f);
		p->LifeTime = RandomRange(0.75f, 1.0f) * 2.0f;
		p->Scale = Vector::ONE * RandomRange(0.75f, 1.0f) * 4.0f;
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterHeart(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// EntityTransform& particlesTrans = particlesEnt->GetTransform();
	// particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";
	spriteSettings.Speed = 0.0f;

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Albedo = Color(1.2f, 0.8f, 0.8f, 0.5f);
	settings.BurstTimeMin = 0.01f;
	settings.BurstTimeMax = 0.05f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 30;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rnd = RandomVectorRange(-1.0f, 1.0f);
		rnd.Normalize();
		rnd.Z = rnd.Z * 0.5f;
		rnd.Z = rnd.Z * (1.0f - rnd.Y * 1.0f);
		rnd.Y = 0.9f*rnd.Y - Abs(rnd.X) * sqrt((20.0f - Abs(rnd.X)) / 15.0f);
		rnd.Y *= -0.8f;
		p->Position += rnd;
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(3.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(0.02f, 0.2f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterHeartImpact(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// EntityTransform& particlesTrans = particlesEnt->GetTransform();
	// particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Albedo = Color(1.5f, 1.0f, 1.0f, 0.95f);
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.5f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 30;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rndPos = RandomVectorRange(-1.0f, 1.0f);
		rndPos.Normalize();
		rndPos.Z = rndPos.Z * 0.5f;
		rndPos.Z = rndPos.Z * (1.0f - rndPos.Y * 1.0f);
		rndPos.Y = 0.9f*rndPos.Y - Abs(rndPos.X) * sqrt((20.0f - Abs(rndPos.X)) / 15.0f);
		rndPos.Y *= -0.8f;
		p->Position += rndPos;
		Vector rndVel = RandomVectorRange(-1.0f, 1.0f);
		rndVel.Normalize();
		p->Velocity = rndVel * 0.01f;
		p->Acceleration = rndVel * 0.0001f;
		p->LifeTime = RandomRange(8.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.01f, 0.02f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Scale = Vector::ONE * Lerp(0.04f, 0.01f, pow(p->Age / p->LifeTime, 16.0f));
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* ParticleTestSystem::SpawnEmitterHeartImpact2(Scene* scene, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// EntityTransform& particlesTrans = particlesEnt->GetTransform();
	// particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Albedo = Color(2.0f, 0.5f, 0.5f, 0.2f);
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 1.0f;
	settings.BurstSizeMin = 200;
	settings.BurstSizeMax = 200;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rndPos = RandomVectorRange(-1.0f, 1.0f);
		rndPos.Normalize();
		rndPos.Z = rndPos.Z * 0.5f;
		rndPos.Z = rndPos.Z * (1.0f - rndPos.Y * 1.0f);
		rndPos.Y = 0.9f*rndPos.Y - Abs(rndPos.X) * sqrt((20.0f - Abs(rndPos.X)) / 15.0f);
		rndPos.Y *= -0.8f;
		rndPos.Y += 0.02f;
		// rndPos *= 1.2f;
		Vector rndVel = rndPos;
		Vector rndAccel = RandomVectorRange(0.5f, 1.0f);
		p->Position += rndPos;
		p->Velocity = rndVel * 0.01f;
		p->Acceleration = rndVel * 0.001f + Vector(-1.5f * rndAccel.X, 1.0f * rndAccel.Y, 0.5f * rndAccel.Z) * 0.001f;
		p->LifeTime = RandomRange(0.2f, 0.75f);
		p->Scale = Vector::ONE * RandomRange(0.04f, 0.05f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Scale = Vector::ONE * Lerp(0.4f, 0.001f, pow(p->Age / p->LifeTime, 8.0f));
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(scene, particlesEnt, settings);
	return particleCmp;
}

void ParticleTestSystem::SpawnHeartSystem(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	gameMgrCmp->particleHeart = SpawnEmitterHeart(scene, Vector(0.0f, 4.0f, 0.0f));
	gameMgrCmp->particleHeartImpact0 = SpawnEmitterHeartImpact(scene, Vector(0.0f, 4.0f, 0.0f));
	gameMgrCmp->particleHeartImpact1 = SpawnEmitterHeartImpact(scene, Vector(0.0f, 4.0f, 0.0f));
	gameMgrCmp->particleHeartImpact2 = SpawnEmitterHeartImpact2(scene, Vector(0.0f, 4.0f, 0.0f));
}

// #pragma endregion