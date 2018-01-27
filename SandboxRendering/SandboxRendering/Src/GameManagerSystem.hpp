#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void CreateShaderball(World* world);
	void CreateSponzaScene(World* world);
	void Update(World* world);
	void Deinit(World* world);

	ParticleComponent* SpawnEmitter0(World* world);
	ParticleComponent* SpawnEmitter1(World* world);
	ParticleComponent* SpawnEmitter2(World* world);

	float Random();
	float Random(float min, float max);
	Vector RandomVector(float min, float max);
	void AddPointLights(World* world, int Quata);
	void CreatePointLight(World* world, float Range);
	void CreateSpotLight(World * world, float Range);
}
