#pragma once

#include <ECS/ComponentBase.hpp>
#include <Math/Vector2f.hpp>
#include <AI/PathfindingComponent.hpp>
#include <Memory/SafePtr.hpp>
#include "NavGrid.hpp"

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT LevelComponent : public ComponentBase
	{
	public:
		LevelComponent() = default;

		float TimeSinceLastEnemySpawn = 295.0f;

		Vector2f Size;
		NavGrid* NavigationGrid;
		SafePtr<Entity> Ground;
		Dynarray<SafePtr<Entity>> Enemy;
		Entity* Beacon;
		Dynarray<SafePtr<Entity>> Beacons;
	};
}