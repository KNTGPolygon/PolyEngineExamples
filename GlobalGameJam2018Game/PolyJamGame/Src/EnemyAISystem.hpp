#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace EnemyAISystem
	{
		void GAME_DLLEXPORT Update(World* world);
		void DeathParticle(Poly::Entity * ParticlesEnt, Poly::World * &world);
	}
}
