#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(Scene* scene);


	Entity* CreateModel(Scene* scene, String path);
	
	void CreateTextUI(Scene* scene);
	void CreateCamera(Scene* scene);
	void CreateRandomCubes(Scene* scene, int size, Vector pos, Vector originScatter);
	void CreatePBRShpereGrid(Scene* scene, Vector pos, Color albedo);
	void CreateTranslucent(Scene* scene);
	void CreateSponza(Scene* scene);
	void CreateSponzaSample(Scene* scene);

	void PostFancy(PostprocessSettingsComponent* postCmp);
	void PostReset(PostprocessSettingsComponent* postCmp);
	void PostMinimal(PostprocessSettingsComponent* postCmp);
	
	void CreatePointLights(Scene* scene, int quota);
	Entity* CreatePointLight(Scene* scene, Vector& position, float Range);
	void CreateSpotLight(Scene* scene, float Range);
	void CreateParticles(Scene* scene);
	void SpawnSpritesSheets(Scene* scene);
	void SpawnShaderball(Scene* scene);

	void SpawnSpritesheet11(Scene* scene, Vector pos);
	void SpawnSpritesheet22(Scene* scene, Vector pos);
	void SpawnSpritesheet44(Scene* scene, Vector pos);
	void SpawnSpritesheet42(Scene* scene, Vector pos);
	void SpawnSpritesheet41(Scene* scene, Vector pos);
	void SpawnSpritesheet44Random(Scene* scene, Vector pos);
	void SpawnSpritesheetGandalf(Scene* scene, Vector pos);

	ParticleComponent* SpawnEmitterDefault(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterWorldSpace(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterAmbient(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterAmbientWind(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterHeart(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact(Scene* scene, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact2(Scene* scene, Vector pos);
	void SpawnHeartSystem(Scene* scene);

	void Update(Scene* scene);
	void UpdateDirLight(Poly::Scene * scene);
	void UpdateParticles(Scene* scene);
	void UpdateLights(Scene* scene);
	void UpdateModel(Scene* scene);
	void UpdatePostProcess(Scene* scene);
	
	void Deinit(Scene* scene);
}
