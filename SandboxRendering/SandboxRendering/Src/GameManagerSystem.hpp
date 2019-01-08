#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void Init(Scene* world);

	void CreateShadowsTestScene(Scene* scene);
	void CreateShadingTestScene(Scene* scene);

	void PostProcessNone(PostprocessSettingsComponent* postCmp);
	void PostProcessFancyCold(PostprocessSettingsComponent* postCmp);
	void PostProcessFancyWarm(PostprocessSettingsComponent* postCmp);

	Entity* CreateModel(Scene* world, String path);
	
	void CreateTextUI(Scene* world);
	void CreateCamera(Scene* world);
	void CreatePBRShpereGrid(Scene* world, Vector pos, Color albedo);
	void CreateRandomCubes(Scene* world);
	void CreateTranslucent(Scene* world);
	void CreateSponza(Scene* world);
	void CreateSponzaSample(Scene* world);
	
	void CreatePointLights(Scene* world, int quota);
	Entity* CreatePointLight(Scene* world, Vector& position, float Range);

	void SpawnShaderball(Scene* world);

	void Update(Scene* world);
	void UpdateCameraAspect(Scene* scene);
	void UpdateLights(Scene* world);
	void UpdateModel(Scene* world);
	void UpdatePostProcess(Scene* world);
	
	void Deinit(Scene* world);
}
