#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace GGJGame
{
	enum class eProjectileType;

	namespace ActorSystem
	{
		void GAME_DLLEXPORT Update(World* world);
		
		void Jump(Poly::Entity* actor);
		void Shoot(Poly::World* world, Poly::Entity* actor, const Vector& localOffset, const Vector& direction);
		void Move(Poly::Entity* actor, Vector direction, float speedMult = 1.0f);
		void GiveDamage(Poly::Entity* actor, float damage);
	}
}