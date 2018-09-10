#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace ModelCheckSystem
{
	void CreateScene(Scene* world);

	void Update(Scene* world);

	void Deinit(Scene* world);

	Entity* CreateModel(Scene* world, String path);
	
	void CreateCamera(Scene* world);
	
}