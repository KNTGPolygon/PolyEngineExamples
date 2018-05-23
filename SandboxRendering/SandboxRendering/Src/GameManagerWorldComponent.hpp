#pragma once

#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <ECS/ComponentBase.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>
#include <Collections/Dynarray.hpp>
#include <UniqueID.hpp>

using namespace Poly;

class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase
{
public:
	SafePtr<Entity> Camera;
	SafePtr<Entity> KeyDirLight;
	Dynarray<SafePtr<Entity>> DirLights;
	Dynarray<PointLightComponent*> PointLights;
	Dynarray<Vector> PointLightPositions;
	Dynarray<SafePtr<Entity>> SpotLights;
	Dynarray<MeshRenderingComponent*> DebugMeshes;
	bool IsDrawingDebugMeshes = true;

	Dynarray<SafePtr<Entity>> GameEntities;

	ParticleComponent* particleDefault;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;
	ParticleComponent* particleAmbient;
	ParticleComponent* particleAmbientWind;

	ParticleComponent* particleHeart;
	ParticleComponent* particleHeartImpact0;
	ParticleComponent* particleHeartImpact1;
	ParticleComponent* particleHeartImpact2;
};
