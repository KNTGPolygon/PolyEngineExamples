#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void Update(World* world);
	void Deinit(World* world);

	void UpdateParticles(World* world);

	void SpawnParticles(World* world);
	void SpawnSpritesSheets(World* world);
	void SpawnShaderball(World* world);
	void SpawnSponzaScene(World* world);
	
	void SpawnSpritesheet11(World* world, Vector pos);
	void SpawnSpritesheet22(World* world, Vector pos);
	void SpawnSpritesheet44(World* world, Vector pos);
	void SpawnSpritesheet42(World* world, Vector pos);
	void SpawnSpritesheet41(World* world, Vector pos);
	void SpawnSpritesheet44Random(World* world, Vector pos);
	void SpawnSpritesheetGandalf(World* world, Vector pos);

	ParticleComponent* SpawnEmitterDefault(World* world, Vector pos);
	ParticleComponent* SpawnEmitterWorldSpace(World* world, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(World* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbient(World* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbientWind(World* world, Vector pos);
	void SpawnHeartSystem(World* world);
	ParticleComponent* SpawnEmitterHeart(World* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact(World* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact2(World* world, Vector pos);

	void AddPointLights(World* world, int Quata);
	void CreatePointLight(World* world, float Range);
	void CreateSpotLight(World * world, float Range);
}
