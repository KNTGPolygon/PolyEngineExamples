#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace ParticleTestSystem
{		
	void Init(Scene* world);

	ParticleComponent* SpawnEmitterDefault(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterWorldSpace(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbient(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbientFireflies(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterAmbientWind(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterHeart(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact(Scene* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact2(Scene* world, Vector pos);
	
	void SpawnHeartSystem(Scene* world);
	
	void Update(Scene* world);
	
	void Deinit(Scene* world);
}
