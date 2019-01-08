#pragma once

#include <UniqueID.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>

using namespace Poly;

class ParticleTestWorldComponent : public ComponentBase
{
public:
	
	ParticleComponent* particleDefault;
	ParticleComponent* particleAmbient;
	ParticleComponent* particleAmbientWind;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;

	ParticleComponent* particleHeart;
	ParticleComponent* particleHeartImpact0;
	ParticleComponent* particleHeartImpact1;
	ParticleComponent* particleHeartImpact2;
};