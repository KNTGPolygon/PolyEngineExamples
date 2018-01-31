#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void SpawnShaderball(World* world);
	void SpawnSponzaScene(World* world);
	void Update(World* world);
	void Deinit(World* world);

	ParticleComponent* SpawnEmitter0(World* world);
	ParticleComponent* SpawnEmitter1(World* world);
	ParticleComponent* SpawnEmitter2(World* world);
	ParticleComponent* SpawnEmitter3(World* world);
	ParticleComponent* SpawnEmitter4(World* world);
	void SpawnSpritesheet11(World* world);
	void SpawnSpritesheet22(World* world);
	void SpawnSpritesheet44(World* world);
	void SpawnSpritesheet42(World* world);
	void SpawnSpritesheet41(World* world);
	void SpawnSpritesheet44Random(World* world);
	void SpawnSpritesheetGandalf(World* world);

	void AddPointLights(World* world, int Quata);
	void CreatePointLight(World* world, float Range);
	void CreateSpotLight(World * world, float Range);
}
