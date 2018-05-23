#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace PlayerSystem
	{
		void GAME_DLLEXPORT Update(World* world);

		void SpawnMuzzleFlash(Poly::World * world, Poly::EntityTransform & gunTransform, const Poly::Vector &cameraGlobFwd);

		Entity* GetPlayerCamera(Entity* player);
		Entity* GetPlayerGun(Entity* player);
		Entity* GetPlayerSoundEmmiter(Entity* player);
	}
}