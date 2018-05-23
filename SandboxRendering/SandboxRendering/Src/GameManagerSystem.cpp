#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>
#include <Resources/ResourceManager.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Time/TimeWorldComponent.hpp>
#include <Rendering/SkyboxWorldComponent.hpp>
#include <Rendering/SpritesheetComponent.hpp>
#include <Input/InputWorldComponent.hpp>

using namespace Poly;

void GameManagerSystem::CreateScene(World* world)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 50_deg, 0.1f, 3000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 1.0f, 0.003f);
	DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, Camera);
	PostprocessSettingsComponent* postCmp = world->GetComponent<PostprocessSettingsComponent>(Camera);
	postCmp->UseBgShader = false;
	postCmp->UseFgShader = false;
	postCmp->Distortion = 0.5f;
	postCmp->ColorTempValue = 6500.0f;
	postCmp->Saturation = 1.0f;
	postCmp->Grain = 1.0f;
	postCmp->Stripes = 0.0f;
	postCmp->Vignette = 1.0f;

	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetLocalTranslation(Vector(0.0f, 3.0f, 15.0f));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	EnumArray<String, eCubemapSide> stormydays{
		{eCubemapSide::RIGHT, "Cubemaps/stormydays/stormydays_rt.jpg"},
		{eCubemapSide::LEFT , "Cubemaps/stormydays/stormydays_lt.jpg"},
		{eCubemapSide::TOP  , "Cubemaps/stormydays/stormydays_up.jpg"},
		{eCubemapSide::DOWN , "Cubemaps/stormydays/stormydays_dn.jpg"},
		{eCubemapSide::BACK , "Cubemaps/stormydays/stormydays_bk.jpg"},
		{eCubemapSide::FRONT, "Cubemaps/stormydays/stormydays_ft.jpg"}
	};

	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, stormydays);

	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.0f, 0.0f, 0.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.0f);


	SpawnSpritesSheets(world);


	SpawnParticles(world);


	SpawnSponzaScene(world);
}

void GameManagerSystem::Update(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (int i = 0; i < GameMgrCmp->PointLights.GetSize(); ++i)
	{
		PointLightComponent* PointLightCmp = GameMgrCmp->PointLights[i];
		EntityTransform& TransCmp = PointLightCmp->GetTransform();
		Vector Position = GameMgrCmp->PointLightPositions[i];
		Vector Bounce = Vector(0.0f, 100.0f*Abs(Sin(1.0_rad * (time + 0.1f*(Position.X + Position.Y)))), 0.0f);
		TransCmp.SetLocalTranslation(Position + Bounce);
	}

	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	if (inputCmp->IsReleased(eKey::KEY_G))
	{
		GameMgrCmp->IsDrawingDebugMeshes = !GameMgrCmp->IsDrawingDebugMeshes;

		gConsole.LogInfo("GameManagerSystem::Update IsDrawingDebugMeshes: {}", GameMgrCmp->IsDrawingDebugMeshes);

		for (int i = 0; i < GameMgrCmp->DebugMeshes.GetSize(); ++i)
		{
			GameMgrCmp->DebugMeshes[i]->SetShadingModel(GameMgrCmp->IsDrawingDebugMeshes ? eShadingModel::UNLIT : eShadingModel::NONE);
		}
	}

	UpdateParticles(world);
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Cleanup");
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	for (SafePtr<Entity> e : GameMgrCmp->GameEntities)
	{
		DeferredTaskSystem::DestroyEntity(world, e.Get());
	}
}


void GameManagerSystem::UpdateParticles(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Vector translation = Vector::UNIT_Y * (0.2f*Abs(Sin(1.0_rad*3.1415f *time + 0.5_rad)));
	Vector scale = Vector::ONE * (1.0f + 0.1f*(0.5f + 0.5f*Sin(2.0_rad*3.1415f *time + 0.5_rad)));
	Quaternion rotation = Quaternion(Vector::UNIT_Y, 0.5_rad * time);

	if (GameMgrCmp->particleHeart)
	{
		GameMgrCmp->particleHeart->GetTransform().SetGlobalTranslation(translation);
		GameMgrCmp->particleHeart->GetTransform().SetGlobalScale(scale);
		GameMgrCmp->particleHeart->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleHeartImpact0)
	{
		GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalTranslation(translation);
		GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalScale(scale);
		GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleHeartImpact1)
	{
		GameMgrCmp->particleHeartImpact1->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleHeartImpact2)
	{
		GameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * 0.2f);
		GameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleLocalSpace)
	{
		GameMgrCmp->particleLocalSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time), 0.0f, Sin(100.0_deg * time)) * 6.0f);
	}

	if (GameMgrCmp->particleWorldSpace)
	{
		GameMgrCmp->particleWorldSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time + 180.0_deg), 0.0f, Sin(100.0_deg * time + 180.0_deg)) * 6.0f);
	}
}


void GameManagerSystem::SpawnParticles(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	Vector particlesPosition = Vector(0.0f, 0.0f, 0.0f);
	GameMgrCmp->particleDefault		= SpawnEmitterDefault(world,		particlesPosition);
	GameMgrCmp->particleWorldSpace	= SpawnEmitterWorldSpace(world,		particlesPosition);
	GameMgrCmp->particleLocalSpace	= SpawnEmitterLocalSpace(world,		particlesPosition);
	GameMgrCmp->particleAmbient		= SpawnEmitterAmbient(world,		particlesPosition);
	GameMgrCmp->particleAmbientWind = SpawnEmitterAmbientWind(world,	particlesPosition);

	// SpawnHeartSystem(world);
}

void GameManagerSystem::SpawnSpritesSheets(Poly::World * world)
{
	Vector spritesheetsPosition = Vector(-15.0f, 0.0f, 0.0f);
	SpawnSpritesheet11(world, spritesheetsPosition + Vector(-5.0f, 4.0f, 0.0f));
	SpawnSpritesheet22(world, spritesheetsPosition + Vector(0.0f, 4.0f, 0.0f));
	SpawnSpritesheet44(world, spritesheetsPosition + Vector(5.0f, 4.0f, 0.0f));
	SpawnSpritesheet42(world, spritesheetsPosition + Vector(-5.0f, 8.0f, 0.0f));
	SpawnSpritesheet41(world, spritesheetsPosition + Vector(0.0f, 8.0f, 0.0f));
	SpawnSpritesheet44Random(world, spritesheetsPosition + Vector(5.0f, 8.0f, 0.0f));
	SpawnSpritesheetGandalf(world, spritesheetsPosition + Vector(0.0f, 12.0f, 0.0f));
}

void GameManagerSystem::SpawnShaderball(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Shaderball = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& shaderballTrans = Shaderball->GetTransform();
	shaderballTrans.SetLocalTranslation(Vector(0.0f, 5.0f, 0.0f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Shaderball, "Models/shaderball/PolyEngine_shaderball.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ballMesh = world->GetComponent<MeshRenderingComponent>(Shaderball);
	ballMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));
	ballMesh->SetMaterial(1, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(0.4f, 0.4f, 0.4f), Color(1.0f, 1.0f, 0.5f), 16.0f));
	shaderballTrans.SetLocalScale(0.1f);
	GameMgrCmp->GameEntities.PushBack(Shaderball);
}

void GameManagerSystem::SpawnSponzaScene(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.2f, 0.5f, 1.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.05f);

	// Dir Light 0
	Quaternion DirLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	Entity* KeyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, KeyDirLight, Color(1.0f, 0.9f, 0.8f), 0.8f);
	EntityTransform& dirLightTrans = KeyDirLight->GetTransform();
	dirLightTrans.SetLocalRotation(DirLightRot);
	GameMgrCmp->KeyDirLight = KeyDirLight;

	// Point Lights
	// CreatePointLight(world, 100.0f);

	// AddPointLights(world, 7);

	// CreateSpotLight(world, 200.0f);


	Entity* Ground = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Ground, "Models/Sponza/sponza.obj", eResourceSource::GAME);
	EntityTransform& groundTrans = Ground->GetTransform();
	MeshRenderingComponent* sponzaMesh = world->GetComponent<MeshRenderingComponent>(Ground);
	for (int i = 0; i < sponzaMesh->GetMesh()->GetSubMeshes().GetSize(); ++i)
	{
		sponzaMesh->SetMaterial(i, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), 8.0f));
	}
	GameMgrCmp->GameEntities.PushBack(Ground);

	// CreateShaderball(world, GameMgrCmp);
}


#pragma region Spritesheet examples

void GameManagerSystem::SpawnSpritesheet11(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(1.0f, 1.0f);
	settings.SpritePath = "Textures/test_1_1.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet22(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(2.0f, 2.0f);
	settings.SpritePath = "Textures/test_2_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.SpriteColor = Color(0.0f, 1.0f, 0.0f, 0.5f);
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet42(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 2.0f);
	settings.SpritePath = "Textures/test_4_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet41(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 1.0f);
	settings.SpritePath = "Textures/test_4_1.png";
	settings.SpriteColor = Color::RED;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44Random(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.IsRandomStartFrame = true;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheetGandalf(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/gandalf_anim.png";
	settings.Speed = 0.2f;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

#pragma endregion


#pragma region Particle Examples

ParticleComponent* GameManagerSystem::SpawnEmitterDefault(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.SprsheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterWorldSpace(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 50;
	settings.SprsheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.05f;
	settings.BurstTimeMax = 0.20f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(0.5f, 2.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterLocalSpace(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.SprsheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterAmbient(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 500;
	settings.InitialSize = 500;
	settings.SprsheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 20;
	settings.BaseColor = Color(1.0f, 1.0f, 1.0f, 0.5f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 10.0f;
		p->Velocity = RandomVectorRange(-1.0f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(5.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.025f, 0.05f);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterAmbientWind(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 200;
	settings.InitialSize = 0;
	settings.SprsheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 200;
	settings.BurstSizeMax = 400;
	settings.BaseColor = Color(1.0f, 1.0f, 1.0f, 0.1f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += Vector(-20.0f, 2.0f, 0.0f) + RandomVectorRange(-1.0f, 1.0f) * 10.0f;
		p->Velocity = Vector(RandomRange(0.75f, 1.0f) * 0.5f, 0.0f, 0.0f);
		p->LifeTime = RandomRange(0.75f, 1.0f) * 2.0f;
		p->Scale = Vector::ONE * RandomRange(0.75f, 1.0f) * 4.0f;
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeart(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	// ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";
	spriteSettings.Speed = 0.0f;

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.BaseColor = Color(1.2f, 0.8f, 0.8f, 0.5f);
	settings.BurstTimeMin = 0.01f;
	settings.BurstTimeMax = 0.05f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 30;
	settings.SprsheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rnd = RandomVectorRange(-1.0f, 1.0f);
		rnd.Normalize();
		rnd.Z = rnd.Z * 0.5f;
		rnd.Z = rnd.Z * (1.0f - rnd.Y * 1.0f);
		rnd.Y = 0.9f*rnd.Y - Abs(rnd.X) * sqrt((20.0f - Abs(rnd.X))/15.0f);
		rnd.Y *= -0.8f;
		p->Position += rnd;
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(3.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(0.02f, 0.2f);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeartImpact(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	// ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.BaseColor = Color(1.5f, 1.0f, 1.0f, 0.95f);
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.5f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 30;
	settings.SprsheetSettings = spriteSettings;
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

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeartImpact2(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	// ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.BaseColor = Color(2.0f, 0.5f, 0.5f, 0.2f);
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 1.0f;
	settings.BurstSizeMin = 200;
	settings.BurstSizeMax = 200;
	settings.SprsheetSettings = spriteSettings;
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

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

void GameManagerSystem::SpawnHeartSystem(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	GameMgrCmp->particleHeart = SpawnEmitterHeart(world,				Vector(0.0f, 4.0f, 0.0f));
	GameMgrCmp->particleHeartImpact0 = SpawnEmitterHeartImpact(world,	Vector(0.0f, 4.0f, 0.0f));
	GameMgrCmp->particleHeartImpact1 = SpawnEmitterHeartImpact(world,	Vector(0.0f, 4.0f, 0.0f));
	GameMgrCmp->particleHeartImpact2 = SpawnEmitterHeartImpact2(world,	Vector(0.0f, 4.0f, 0.0f));
}

#pragma endregion


#pragma region Lights

void GameManagerSystem::AddPointLights(World* world, int Quota)
{
	for (int i = 0; i < Quota; ++i)
	{
		CreatePointLight(world, 200.0f);
	}
}

void GameManagerSystem::CreatePointLight(World* world, float Range)
{
	Vector PointLightPos = Vector(RandomRange(-1.0f, 1.0f), RandomRange(0.0f, 0.2f), RandomRange(-0.5f, 0.5f)) * 1000.0f;
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 0.5f), RandomRange(0.0f, 0.2f));

	Entity* PointLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, PointLight, LightColor, 1.0f, Range);
	EntityTransform& PointLightTrans = PointLight->GetTransform();
	PointLightTrans.SetLocalTranslation(PointLightPos);

	Entity* PointLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, PointLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* PointLightMesh = world->GetComponent<MeshRenderingComponent>(PointLightDebugSource);
	PointLightMesh->SetShadingModel(eShadingModel::UNLIT);
	PointLightMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& PointLightDebugSourceTrans = PointLightDebugSource->GetTransform();
	PointLightDebugSource->SetParent(PointLight);
	PointLightDebugSourceTrans.SetLocalScale(10.0f);
	PointLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));

	Entity* PointLightDebugRange = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, PointLightDebugRange, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* PointLightRangeMesh = world->GetComponent<MeshRenderingComponent>(PointLightDebugRange);
	PointLightRangeMesh->SetShadingModel(eShadingModel::UNLIT);
	PointLightRangeMesh->SetIsWireframe(true);
	PointLightRangeMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& PointLightRangeTrans = PointLightDebugRange->GetTransform();
	PointLightDebugRange->SetParent(PointLight);
	PointLightRangeTrans.SetLocalScale(Range);
	PointLightRangeTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));

	PointLightComponent* PointLightCmp = world->GetComponent<PointLightComponent>(PointLight);
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	GameMgrCmp->PointLights.PushBack(PointLightCmp);
	GameMgrCmp->PointLightPositions.PushBack(PointLightPos);
	GameMgrCmp->DebugMeshes.PushBack(PointLightRangeMesh);
}

void GameManagerSystem::CreateSpotLight(World* world, float Range)
{
	Vector SpotLightPos = Vector(50.0f, 50.0f, 0.0f);
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 0.5f), RandomRange(0.0f, 0.2f));
	Quaternion SpotLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	// float PointLightRange = 100.0f;
	Entity* SpotLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<SpotLightComponent>(world, SpotLight, LightColor, 1.0f, Range, 5.0f, 17.0f);
	EntityTransform& SpotLightTrans = SpotLight->GetTransform();
	SpotLightTrans.SetLocalTranslation(SpotLightPos);
	SpotLightTrans.SetLocalRotation(SpotLightRot);

	Entity* SpotLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, SpotLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* SpotLightMesh = world->GetComponent<MeshRenderingComponent>(SpotLightDebugSource);
	SpotLightMesh->SetShadingModel(eShadingModel::UNLIT);
	SpotLightMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& SpotLightDebugSourceTrans = SpotLightDebugSource->GetTransform();
	SpotLightDebugSource->SetParent(SpotLight);
	SpotLightDebugSourceTrans.SetLocalScale(2.0f);
	SpotLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
}

#pragma endregion