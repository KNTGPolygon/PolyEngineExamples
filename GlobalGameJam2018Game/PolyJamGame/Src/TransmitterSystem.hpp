#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace TransmitterSystem
	{
		struct PercentData : public BaseObject<>
		{
			float TimeLeft;
			SafePtr<Entity> TextEnt;
		};
		// sets all TransmitterComponent properties.
		void GAME_DLLEXPORT Update(World* world);
		int GetPercent(World* world);
	}
}
