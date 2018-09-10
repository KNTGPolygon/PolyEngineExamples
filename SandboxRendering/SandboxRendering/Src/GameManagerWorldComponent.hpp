#pragma once

#include <UniqueID.hpp>
#include <Memory/SafePtr.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>

using namespace Poly;

class GameManagerWorldComponent : public ComponentBase
{
	RTTI_DECLARE_COMPONENT(GameManagerWorldComponent)
	{
		NO_RTTI_PROPERTY();
	}
public:
	SafePtr<Entity> Camera;
	CameraComponent* CameraStaticCmp;
	SafePtr<Entity> KeyDirLight;
	PostprocessSettingsComponent* PostCmp;
	SafePtr<Entity> Model;
	bool IsDrawingDebugMeshes = true;
	Dynarray<SafePtr<Entity>> GameEntities;

	ParticleComponent* particleDefault;
	ParticleComponent* particleAmbient;
	ParticleComponent* particleAmbientWind;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;

	ParticleComponent* particleHeart;
	ParticleComponent* particleHeartImpact0;
	ParticleComponent* particleHeartImpact1;
	ParticleComponent* particleHeartImpact2;

	Dynarray<Vector> LightsStartPositions;
	Dynarray<Entity*> PointLightEntities;
};