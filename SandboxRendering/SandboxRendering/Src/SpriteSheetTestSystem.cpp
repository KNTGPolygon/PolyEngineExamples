#include "SpriteSheetTestSystem.hpp"

#include "SpriteSheetTestWorldComponent.hpp"

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

void SpriteSheetTestSystem::Init(Scene* scene)
{
	gConsole.LogInfo("SpriteSheetTestSystem::Init");

	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();
}

void SpriteSheetTestSystem::Update(Scene* scene)
{

}

void SpriteSheetTestSystem::Deinit(Scene* scene)
{
	gConsole.LogInfo("SpriteSheetTestSystem::Deinit");
}


void SpriteSheetTestSystem::SpawnSpritesSheets(Scene* scene)
{
	Vector spritesheetsPosition = Vector(-15.0f, 0.0f, 0.0f);
	SpawnSpritesheet11(scene, spritesheetsPosition + Vector(-5.0f, 4.0f, 0.0f));
	SpawnSpritesheet22(scene, spritesheetsPosition + Vector(0.0f, 4.0f, 0.0f));
	SpawnSpritesheet44(scene, spritesheetsPosition + Vector(5.0f, 4.0f, 0.0f));
	SpawnSpritesheet42(scene, spritesheetsPosition + Vector(-5.0f, 8.0f, 0.0f));
	SpawnSpritesheet41(scene, spritesheetsPosition + Vector(0.0f, 8.0f, 0.0f));
	SpawnSpritesheet44Random(scene, spritesheetsPosition + Vector(5.0f, 8.0f, 0.0f));
	SpawnSpritesheetGandalf(scene, spritesheetsPosition + Vector(0.0f, 12.0f, 0.0f));
}

void SpriteSheetTestSystem::SpawnSpritesheet11(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(1.0f, 1.0f);
	settings.SpritePath = "Textures/test_1_1.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}

void SpriteSheetTestSystem::SpawnSpritesheet22(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(2.0f, 2.0f);
	settings.SpritePath = "Textures/test_2_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}

void SpriteSheetTestSystem::SpawnSpritesheet44(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.SpriteColor = Color(0.0f, 1.0f, 0.0f, 0.5f);
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}

void SpriteSheetTestSystem::SpawnSpritesheet42(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 2.0f);
	settings.SpritePath = "Textures/test_4_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}

void SpriteSheetTestSystem::SpawnSpritesheet41(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 1.0f);
	settings.SpritePath = "Textures/test_4_1.png";
	settings.SpriteColor = Color::RED;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}

void SpriteSheetTestSystem::SpawnSpritesheet44Random(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.IsRandomStartFrame = true;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}

void SpriteSheetTestSystem::SpawnSpritesheetGandalf(Scene* scene, Vector pos)
{
	SpriteSheetTestWorldComponent* spriteSheetCmp = scene->GetWorldComponent<SpriteSheetTestWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/gandalf_anim.png";
	settings.Speed = 0.2f;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(scene, spriteSheetEnt, settings);
	spriteSheetCmp->GameEntities.PushBack(spriteSheetEnt);
}