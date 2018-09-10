#include "ModelCheckSystem.hpp"

#include "ModelCheckWorldComponent.hpp"

#include <Math/Random.hpp>
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

using namespace Poly;

void ModelCheckSystem::CreateScene(Scene* scene)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	ModelCheckWorldComponent* gameMgrCmp = scene->GetWorldComponent<ModelCheckWorldComponent>();

	CreateCamera(scene);
	
	Entity* cube = CreateModel(scene, "Models/Primitives/Cube.obj");
	cube->GetTransform().SetGlobalTranslation(Vector::UNIT_X * 1.0f);
	Entity* defautCube = CreateModel(scene, "Models/MODELS_TO_CHECK/Models/DefaultCube.fbx");
	Entity* defautCubeScaled = CreateModel(scene, "Models/MODELS_TO_CHECK/Models/DefaultCubeDifferentFileScale.fbx");
	Entity* defautHuman = CreateModel(scene, "Models/MODELS_TO_CHECK/Models/DefaultHuman.fbx");

	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(scene, "HDR/HDR.hdr", eResourceSource::GAME);
}

Entity* ModelCheckSystem::CreateModel(Scene* scene, String path)
{
	Entity* model = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(scene, model, eDebugDrawPreset::STATIC);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, model, path, eResourceSource::GAME);
	return model;
}

void ModelCheckSystem::CreateCamera(Scene* scene)
{
	ModelCheckWorldComponent* gameMgrCmp = scene->GetWorldComponent<ModelCheckWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 35_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 100.0f, 0.003f, 10.0f);
	DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);

	// gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);
	// gameMgrCmp->PostCmp->Exposure = 1.0f;
	// gameMgrCmp->PostCmp->DOFSize = 1.0f;
	// gameMgrCmp->PostCmp->DOFPoint = 300.0f;
	// gameMgrCmp->PostCmp->DOFRange = 200.0f;
	// // gameMgrCmp->PostCmp->DOFShow = 1.0f;
	// gameMgrCmp->PostCmp->BloomScale = 1.0f;

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-550.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));
	// gameMgrCmp->Camera = camera;

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, keyDirLight, Color(1.0f, 1.0f, 1.0f), 5.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);
}

void ModelCheckSystem::Update(Scene* scene)
{
	Vector offset = Vector(800.0f, 0.0f, 0.0f);
	DebugDrawSystem::DrawLine(scene, offset, offset + Vector::UNIT_Y * 1000.0f, Color::RED);
	DebugDrawSystem::DrawBox(scene, offset + Vector(-100.0f, 0.0f, -100.0f), offset + Vector(100.0f, 200.0f, 100.0f), Color::RED);
}

void ModelCheckSystem::Deinit(Scene* scene)
{
	gConsole.LogInfo("GameManagerSystem::Deinit");
}