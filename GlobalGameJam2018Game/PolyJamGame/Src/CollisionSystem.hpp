#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace CollisionSystem
	{
		void GAME_DLLEXPORT Update(World* world);
	}
}

void Kaboom(Poly::World * &world);
