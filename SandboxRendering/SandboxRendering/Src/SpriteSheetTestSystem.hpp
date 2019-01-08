#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace SpriteSheetTestSystem
{
	void Init(Scene* world);

	void Update(Scene* world);
	
	void Deinit(Scene* world);


	void SpawnSpritesSheets(Scene* world);

	void SpawnSpritesheet11(Scene* world, Vector pos);
	void SpawnSpritesheet22(Scene* world, Vector pos);
	void SpawnSpritesheet44(Scene* world, Vector pos);
	void SpawnSpritesheet42(Scene* world, Vector pos);
	void SpawnSpritesheet41(Scene* world, Vector pos);
	void SpawnSpritesheet44Random(Scene* world, Vector pos);
	void SpawnSpritesheetGandalf(Scene* world, Vector pos);

}
